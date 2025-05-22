/* osgEarth
 * Copyright 2025 Pelican Mapping
 * MIT License
 */
#include <osgEarth/PMTilesElevationLayer>
#include <osgEarth/Profile>
#include <osgEarth/Registry>
#include <osgEarth/ImageUtils>
#include <osgEarth/FileUtils>
#include <osgEarth/Notify>
#include <osgEarth/HeightFieldUtils>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>
#include <osg/io_utils>
#include <sstream>

using namespace osgEarth;

#define LC "[PMTilesElevationLayer] "

REGISTER_OSGEARTH_LAYER(pmtiles_elevation, PMTilesElevationLayer);

//------------------------------------------------------------------------

Config PMTilesElevationLayer::Options::getConfig() const
{
    Config conf = ElevationLayer::Options::getConfig();
    conf.set("url", _url);
    conf.set("encoding", _encoding);
    conf.set("vertical_scale", _verticalScale);
    conf.set("vertical_offset", _verticalOffset);
    return conf;
}

void PMTilesElevationLayer::Options::fromConfig(const Config& conf)
{
    conf.get("url", _url);
    conf.get("encoding", _encoding);
    conf.get("vertical_scale", _verticalScale);
    conf.get("vertical_offset", _verticalOffset);
}

//------------------------------------------------------------------------

OE_LAYER_PROPERTY_IMPL(PMTilesElevationLayer, URI, URL, url);
OE_LAYER_PROPERTY_IMPL(PMTilesElevationLayer, std::string, Encoding, encoding);
OE_LAYER_PROPERTY_IMPL(PMTilesElevationLayer, float, VerticalScale, verticalScale);
OE_LAYER_PROPERTY_IMPL(PMTilesElevationLayer, float, VerticalOffset, verticalOffset);

PMTilesElevationLayer::~PMTilesElevationLayer()
{
    //nop
}

void PMTilesElevationLayer::init()
{
    ElevationLayer::init();
    
    // PMTiles files have fixed tile size
    options().tileSize() = 256u;
}

Status PMTilesElevationLayer::openImplementation()
{
    Status parent = ElevationLayer::openImplementation();
    if (parent.isError())
        return parent;

    if (!options().url().isSet())
    {
        return Status(Status::ConfigurationError, "Missing required 'url' parameter");
    }

    // Create PMTiles reader
    std::lock_guard<std::mutex> lock(_pmtilesMutex);
    _pmtiles = std::make_unique<PMTiles>(options().url().get());
    
    Status status = _pmtiles->open();
    if (status.isError())
    {
        return Status(Status::ResourceUnavailable, 
                     Stringify() << "Failed to open PMTiles: " << status.message());
    }

    // Validate that this PMTiles contains data suitable for elevation
    PMTiles::TileType tileType = _pmtiles->getTileType();
    std::string encoding = getEncoding();
    
    if (encoding == "terrain-rgb")
    {
        if (tileType != PMTiles::TILE_PNG && tileType != PMTiles::TILE_WEBP)
        {
            return Status(Status::ConfigurationError,
                         "Terrain-RGB encoding requires PNG or WebP tile format");
        }
    }
    else if (encoding == "quantized-mesh")
    {
        // Quantized mesh can be stored as binary data
        if (tileType != PMTiles::TILE_UNKNOWN)
        {
            OE_WARN << LC << "Quantized mesh typically uses unknown tile type" << std::endl;
        }
    }
    else
    {
        return Status(Status::ConfigurationError,
                     Stringify() << "Unsupported elevation encoding: " << encoding);
    }

    // Create profile from bounds
    double minLon, minLat, maxLon, maxLat;
    _pmtiles->getBounds(minLon, minLat, maxLon, maxLat);
    
    // Use spherical mercator profile (most common for PMTiles)
    setProfile(Profile::create(Profile::SPHERICAL_MERCATOR));

    // Set min/max levels
    DataExtentList extents;
    extents.push_back(DataExtent(
        GeoExtent(getProfile()->getSRS(), minLon, minLat, maxLon, maxLat),
        _pmtiles->getMinZoom(),
        _pmtiles->getMaxZoom()));
    setDataExtents(extents);

    OE_INFO << LC << "Opened PMTiles elevation layer: "
            << "bounds=(" << minLon << "," << minLat << "," << maxLon << "," << maxLat << "), "
            << "zoom=" << _pmtiles->getMinZoom() << "-" << _pmtiles->getMaxZoom() << ", "
            << "encoding=" << encoding
            << std::endl;

    return Status::OK();
}

Status PMTilesElevationLayer::closeImplementation()
{
    std::lock_guard<std::mutex> lock(_pmtilesMutex);
    _pmtiles.reset();
    return ElevationLayer::closeImplementation();
}

GeoHeightField PMTilesElevationLayer::createHeightFieldImplementation(const TileKey& key, ProgressCallback* progress) const
{
    if (progress && progress->isCanceled())
        return GeoHeightField::INVALID;

    std::lock_guard<std::mutex> lock(_pmtilesMutex);
    
    if (!_pmtiles || !_pmtiles->isOpen())
        return GeoHeightField::INVALID;

    // Check if tile exists
    if (!_pmtiles->hasTile(key.getLevelOfDetail(), key.getTileX(), key.getTileY()))
        return GeoHeightField::INVALID;

    // Get tile data
    std::vector<uint8_t> tileData = _pmtiles->getTile(key, progress);
    if (tileData.empty())
        return GeoHeightField::INVALID;

    if (progress && progress->isCanceled())
        return GeoHeightField::INVALID;

    osg::ref_ptr<osg::HeightField> hf;
    std::string encoding = getEncoding();

    if (encoding == "terrain-rgb")
    {
        // Decode terrain-RGB data
        std::stringstream stream(std::string(tileData.begin(), tileData.end()));
        
        // Determine format
        std::string format;
        switch (_pmtiles->getTileType())
        {
        case PMTiles::TILE_PNG:
            format = "png";
            break;
        case PMTiles::TILE_WEBP:
            format = "webp";
            break;
        default:
            OE_WARN << LC << "Unsupported tile format for terrain-RGB" << std::endl;
            return GeoHeightField::INVALID;
        }

        // Read image using OSG
        osgDB::ReaderWriter* rw = osgDB::Registry::instance()->getReaderWriterForExtension(format);
        if (rw)
        {
            osgDB::ReaderWriter::ReadResult result = rw->readImage(stream);
            if (result.success())
            {
                hf = decodeTerrainRGB(result.getImage());
            }
            else
            {
                OE_WARN << LC << "Failed to read elevation image: " << result.message() << std::endl;
                return GeoHeightField::INVALID;
            }
        }
        else
        {
            OE_WARN << LC << "No reader found for format: " << format << std::endl;
            return GeoHeightField::INVALID;
        }
    }
    else if (encoding == "quantized-mesh")
    {
        // Decode quantized mesh data (placeholder for future implementation)
        hf = decodeQuantizedMesh(tileData);
    }

    if (!hf.valid())
        return GeoHeightField::INVALID;

    return GeoHeightField(hf.get(), key.getExtent());
}

osg::HeightField* PMTilesElevationLayer::decodeTerrainRGB(osg::Image* image) const
{
    if (!image)
        return nullptr;

    // Terrain-RGB encoding: height = -10000 + ((R * 256 * 256 + G * 256 + B) * 0.1)
    // This is the Mapbox terrain-RGB specification
    
    int width = image->s();
    int height = image->t();
    
    osg::ref_ptr<osg::HeightField> hf = new osg::HeightField();
    hf->allocate(width, height);
    
    float verticalScale = getVerticalScale();
    float verticalOffset = getVerticalOffset();
    
    for (int t = 0; t < height; ++t)
    {
        for (int s = 0; s < width; ++s)
        {
            osg::Vec4 color = image->getColor(s, t);
            
            // Convert normalized color to 0-255 range
            unsigned char r = static_cast<unsigned char>(color.r() * 255.0f);
            unsigned char g = static_cast<unsigned char>(color.g() * 255.0f);
            unsigned char b = static_cast<unsigned char>(color.b() * 255.0f);
            
            // Decode terrain-RGB
            float elevation = -10000.0f + ((r * 256 * 256 + g * 256 + b) * 0.1f);
            
            // Apply scale and offset
            elevation = elevation * verticalScale + verticalOffset;
            
            // HeightField origin is lower-left, image origin is typically upper-left
            hf->setHeight(s, height - 1 - t, elevation);
        }
    }
    
    return hf.release();
}

osg::HeightField* PMTilesElevationLayer::decodeQuantizedMesh(const std::vector<uint8_t>& data) const
{
    // Placeholder for quantized mesh decoding
    // This would require implementing the quantized mesh specification
    // which is more complex than terrain-RGB
    
    OE_WARN << LC << "Quantized mesh decoding not yet implemented" << std::endl;
    return nullptr;
}