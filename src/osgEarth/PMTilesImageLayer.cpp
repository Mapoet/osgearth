/* osgEarth
 * Copyright 2025 Pelican Mapping
 * MIT License
 */
#include <osgEarth/PMTilesImageLayer>
#include <osgEarth/Profile>
#include <osgEarth/Registry>
#include <osgEarth/ImageUtils>
#include <osgEarth/FileUtils>
#include <osgEarth/Notify>
#include <osgDB/ReadFile>
#include <osgDB/ReaderWriter>
#include <osg/io_utils>
#include <sstream>

using namespace osgEarth;

#define LC "[PMTilesImageLayer] "

REGISTER_OSGEARTH_LAYER(pmtiles_image, PMTilesImageLayer);

//------------------------------------------------------------------------

Config PMTilesImageLayer::Options::getConfig() const
{
    Config conf = ImageLayer::Options::getConfig();
    conf.set("url", _url);
    conf.set("format", _format);
    return conf;
}

void PMTilesImageLayer::Options::fromConfig(const Config& conf)
{
    conf.get("url", _url);
    conf.get("format", _format);
}

//------------------------------------------------------------------------

OE_LAYER_PROPERTY_IMPL(PMTilesImageLayer, URI, URL, url);
OE_LAYER_PROPERTY_IMPL(PMTilesImageLayer, std::string, Format, format);

PMTilesImageLayer::~PMTilesImageLayer()
{
    //nop
}

void PMTilesImageLayer::init()
{
    ImageLayer::init();
    
    // PMTiles files have fixed tile size
    options().tileSize() = 256u;
}

Status PMTilesImageLayer::openImplementation()
{
    Status parent = ImageLayer::openImplementation();
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

    // Validate tile type for image data
    PMTiles::TileType tileType = _pmtiles->getTileType();
    if (tileType != PMTiles::TILE_PNG && 
        tileType != PMTiles::TILE_JPEG && 
        tileType != PMTiles::TILE_WEBP)
    {
        return Status(Status::ConfigurationError,
                     Stringify() << "PMTiles contains unsupported tile type: " << (int)tileType);
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

    OE_INFO << LC << "Opened PMTiles image layer: "
            << "bounds=(" << minLon << "," << minLat << "," << maxLon << "," << maxLat << "), "
            << "zoom=" << _pmtiles->getMinZoom() << "-" << _pmtiles->getMaxZoom() << ", "
            << "type=" << (int)tileType
            << std::endl;

    return Status::OK();
}

Status PMTilesImageLayer::closeImplementation()
{
    std::lock_guard<std::mutex> lock(_pmtilesMutex);
    _pmtiles.reset();
    return ImageLayer::closeImplementation();
}

GeoImage PMTilesImageLayer::createImageImplementation(const TileKey& key, ProgressCallback* progress) const
{
    if (progress && progress->isCanceled())
        return GeoImage::INVALID;

    std::lock_guard<std::mutex> lock(_pmtilesMutex);
    
    if (!_pmtiles || !_pmtiles->isOpen())
        return GeoImage::INVALID;

    // Check if tile exists
    if (!_pmtiles->hasTile(key.getLevelOfDetail(), key.getTileX(), key.getTileY()))
        return GeoImage::INVALID;

    // Get tile data
    std::vector<uint8_t> tileData = _pmtiles->getTile(key, progress);
    if (tileData.empty())
        return GeoImage::INVALID;

    if (progress && progress->isCanceled())
        return GeoImage::INVALID;

    // Create an osg::Image from the tile data
    osg::ref_ptr<osg::Image> image;
    
    // Create a memory stream from the tile data
    std::stringstream stream(std::string(tileData.begin(), tileData.end()));
    
    // Determine format and use appropriate OSG reader
    std::string format = getFormat();
    if (format.empty())
    {
        // Auto-detect format based on PMTiles tile type
        switch (_pmtiles->getTileType())
        {
        case PMTiles::TILE_PNG:
            format = "png";
            break;
        case PMTiles::TILE_JPEG:
            format = "jpg";
            break;
        case PMTiles::TILE_WEBP:
            format = "webp";
            break;
        default:
            OE_WARN << LC << "Unknown tile format" << std::endl;
            return GeoImage::INVALID;
        }
    }

    // Use OSG to read the image data
    osgDB::ReaderWriter* rw = osgDB::Registry::instance()->getReaderWriterForExtension(format);
    if (rw)
    {
        osgDB::ReaderWriter::ReadResult result = rw->readImage(stream);
        if (result.success())
        {
            image = result.getImage();
        }
        else
        {
            OE_WARN << LC << "Failed to read image: " << result.message() << std::endl;
            return GeoImage::INVALID;
        }
    }
    else
    {
        OE_WARN << LC << "No reader found for format: " << format << std::endl;
        return GeoImage::INVALID;
    }

    if (!image.valid())
        return GeoImage::INVALID;

    // Return the image
    return GeoImage(image.get(), key.getExtent());
}