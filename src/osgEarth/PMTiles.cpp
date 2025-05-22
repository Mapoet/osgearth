/* osgEarth
 * Copyright 2025 Pelican Mapping
 * MIT License
 */
#include <osgEarth/PMTiles>
#include <osgEarth/HTTPClient>
#include <osgEarth/FileUtils>
#include <osgEarth/Notify>
#include <osgEarth/JsonUtils>
#include <osgEarth/StringUtils>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <fstream>
#include <sstream>
#include <cstring>

#ifdef OSGEARTH_HAVE_ZLIB
#include <zlib.h>
#endif

#define LC "[PMTiles] "

using namespace osgEarth;
using namespace osgEarth::Util;

namespace
{
    // Hilbert curve implementation for tile ID conversion
    // Based on the PMTiles reference implementation
    
    uint64_t hilbertCurve(unsigned z, unsigned x, unsigned y)
    {
        unsigned n = 1U << z;
        uint64_t t = 0;
        
        for (unsigned s = n / 2; s > 0; s /= 2) {
            unsigned rx = (x & s) > 0 ? 1 : 0;
            unsigned ry = (y & s) > 0 ? 1 : 0;
            t += s * s * ((3 * rx) ^ ry);
            
            if (ry == 0) {
                if (rx == 1) {
                    x = n - 1 - x;
                    y = n - 1 - y;
                }
                unsigned temp = x;
                x = y;
                y = temp;
            }
        }
        return t;
    }

    void hilbertCurveInverse(uint64_t t, unsigned z, unsigned& x, unsigned& y)
    {
        unsigned n = 1U << z;
        x = y = 0;
        
        for (unsigned s = 1; s < n; s *= 2) {
            unsigned rx = 1 & (t / 2);
            unsigned ry = 1 & (t ^ rx);
            
            if (ry == 0) {
                if (rx == 1) {
                    x = s - 1 - x;
                    y = s - 1 - y;
                }
                unsigned temp = x;
                x = y;
                y = temp;
            }
            x += s * rx;
            y += s * ry;
            t /= 4;
        }
    }

    // Enhanced HTTP request function with range support
    std::vector<uint8_t> httpRangeRequest(const std::string& url, uint64_t offset, uint64_t length, ProgressCallback* progress)
    {
        HTTPRequest request(url);
        
        // Add Range header for partial content requests
        std::ostringstream rangeHeader;
        rangeHeader << "bytes=" << offset << "-" << (offset + length - 1);
        request.addHeader("Range", rangeHeader.str());
        
        HTTPResponse response = HTTPClient::get(request, nullptr, progress);
        
        if (response.isOK() || response.getCode() == 206) // 206 = Partial Content
        {
            if (response.getNumParts() > 0)
            {
                std::string data = response.getPartAsString(0);
                return std::vector<uint8_t>(data.begin(), data.end());
            }
        }
        
        OE_WARN << LC << "HTTP range request failed: " << response.getCode() << std::endl;
        return std::vector<uint8_t>();
    }
}

//------------------------------------------------------------------------

PMTiles::Header::Header()
{
    memset(this, 0, sizeof(Header));
    strcpy(magic, "PMTiles");
    version = 3;
}

bool PMTiles::Header::isValid() const
{
    return strncmp(magic, "PMTiles", 7) == 0 && version == 3;
}

//------------------------------------------------------------------------

PMTiles::PMTiles(const URI& uri) :
    _uri(uri),
    _isOpen(false)
{
    //nop
}

PMTiles::~PMTiles()
{
    //nop
}

Status PMTiles::open(ProgressCallback* progress)
{
    if (_isOpen)
        return Status::OK();

    // Read and validate header
    Status status = readHeader(progress);
    if (status.isError())
        return status;

    // Read root directory
    status = readRootDirectory(progress);
    if (status.isError())
        return status;

    // Read metadata
    status = readMetadata(progress);
    if (status.isError())
        return status;

    _isOpen = true;
    return Status::OK();
}

std::vector<uint8_t> PMTiles::getTile(unsigned z, unsigned x, unsigned y, ProgressCallback* progress)
{
    if (!_isOpen)
        return std::vector<uint8_t>();

    uint64_t tileId = zxyToTileId(z, x, y);
    const Entry* entry = findTileEntry(tileId);
    
    if (!entry)
        return std::vector<uint8_t>();

    std::vector<uint8_t> compressedData = readData(entry->offset, entry->length, progress);
    if (compressedData.empty())
        return std::vector<uint8_t>();

    return decompressTile(compressedData);
}

std::vector<uint8_t> PMTiles::getTile(const TileKey& key, ProgressCallback* progress)
{
    return getTile(key.getLevelOfDetail(), key.getTileX(), key.getTileY(), progress);
}

bool PMTiles::hasTile(unsigned z, unsigned x, unsigned y)
{
    if (!_isOpen)
        return false;

    uint64_t tileId = zxyToTileId(z, x, y);
    return findTileEntry(tileId) != nullptr;
}

void PMTiles::getBounds(double& minLon, double& minLat, double& maxLon, double& maxLat) const
{
    minLon = _header.minLon;
    minLat = _header.minLat;
    maxLon = _header.maxLon;
    maxLat = _header.maxLat;
}

uint64_t PMTiles::zxyToTileId(unsigned z, unsigned x, unsigned y)
{
    if (z == 0)
        return 0;
    
    return hilbertCurve(z, x, y) + ((1ULL << (2 * z)) - 1) / 3;
}

void PMTiles::tileIdToZxy(uint64_t tileId, unsigned& z, unsigned& x, unsigned& y)
{
    if (tileId == 0) {
        z = x = y = 0;
        return;
    }
    
    // Find zoom level
    z = 0;
    uint64_t acc = 0;
    while (acc + ((1ULL << (2 * (z + 1))) - 1) / 3 <= tileId) {
        acc += ((1ULL << (2 * z)) - 1) / 3;
        z++;
    }
    
    uint64_t offset = tileId - acc;
    hilbertCurveInverse(offset, z, x, y);
}

std::vector<uint8_t> PMTiles::readData(uint64_t offset, uint64_t length, ProgressCallback* progress)
{
    if (_uri.isRemote())
    {
        return httpRangeRequest(_uri.full(), offset, length, progress);
    }
    else
    {
        // Local file access
        std::ifstream file(_uri.full(), std::ios::binary);
        if (!file.is_open())
        {
            OE_WARN << LC << "Cannot open local file: " << _uri.full() << std::endl;
            return std::vector<uint8_t>();
        }

        file.seekg(offset);
        if (file.fail())
        {
            OE_WARN << LC << "Cannot seek to offset " << offset << std::endl;
            return std::vector<uint8_t>();
        }

        std::vector<uint8_t> data(length);
        file.read(reinterpret_cast<char*>(data.data()), length);
        
        if (file.gcount() != static_cast<std::streamsize>(length))
        {
            OE_WARN << LC << "Read " << file.gcount() << " bytes, expected " << length << std::endl;
        }

        return data;
    }
}

Status PMTiles::readHeader(ProgressCallback* progress)
{
    std::vector<uint8_t> headerData = readData(0, 127, progress);
    if (headerData.size() != 127)
    {
        return Status(Status::GeneralError, "Failed to read PMTiles header");
    }

    const uint8_t* data = headerData.data();
    
    // Parse header
    memcpy(_header.magic, data, 7);
    data += 7;
    
    _header.version = *data++;
    
    // Read 64-bit values in little-endian format
    auto read64 = [&data]() -> uint64_t {
        uint64_t value = 0;
        for (int i = 0; i < 8; ++i) {
            value |= static_cast<uint64_t>(*data++) << (i * 8);
        }
        return value;
    };
    
    auto readDouble = [&data]() -> double {
        uint64_t raw = 0;
        for (int i = 0; i < 8; ++i) {
            raw |= static_cast<uint64_t>(*data++) << (i * 8);
        }
        return *reinterpret_cast<double*>(&raw);
    };

    _header.rootDirOffset = read64();
    _header.rootDirLength = read64();
    _header.metadataOffset = read64();
    _header.metadataLength = read64();
    
    _header.compression = *data++;
    _header.tileType = *data++;
    _header.minZoom = *data++;
    _header.maxZoom = *data++;
    
    _header.minLon = readDouble();
    _header.minLat = readDouble();
    _header.maxLon = readDouble();
    _header.maxLat = readDouble();
    
    _header.centerZoom = *data++;
    _header.centerLon = readDouble();
    _header.centerLat = readDouble();

    if (!_header.isValid())
    {
        return Status(Status::GeneralError, "Invalid PMTiles header");
    }

    OE_INFO << LC << "Opened PMTiles: "
            << "version=" << (int)_header.version
            << ", type=" << (int)_header.tileType
            << ", compression=" << (int)_header.compression
            << ", zoom=" << (int)_header.minZoom << "-" << (int)_header.maxZoom
            << std::endl;

    return Status::OK();
}

Status PMTiles::readRootDirectory(ProgressCallback* progress)
{
    if (_header.rootDirLength == 0)
        return Status::OK();

    std::vector<uint8_t> dirData = readData(_header.rootDirOffset, _header.rootDirLength, progress);
    if (dirData.empty())
    {
        return Status(Status::GeneralError, "Failed to read root directory");
    }

    _rootDirectory = parseDirectory(dirData);
    
    OE_INFO << LC << "Read root directory with " << _rootDirectory.size() << " entries" << std::endl;
    
    return Status::OK();
}

Status PMTiles::readMetadata(ProgressCallback* progress)
{
    if (_header.metadataLength == 0)
        return Status::OK();

    std::vector<uint8_t> metaData = readData(_header.metadataOffset, _header.metadataLength, progress);
    if (metaData.empty())
    {
        return Status(Status::GeneralError, "Failed to read metadata");
    }

    _metadata = std::string(metaData.begin(), metaData.end());
    
    OE_INFO << LC << "Read metadata: " << _metadata.length() << " bytes" << std::endl;
    
    return Status::OK();
}

const PMTiles::Entry* PMTiles::findTileEntry(uint64_t tileId)
{
    // Binary search in root directory
    auto it = std::lower_bound(_rootDirectory.begin(), _rootDirectory.end(), tileId,
        [](const Entry& entry, uint64_t id) { return entry.tileId < id; });
        
    if (it != _rootDirectory.end() && it->tileId == tileId)
        return &(*it);

    // TODO: Implement hierarchical directory search for large archives
    return nullptr;
}

std::vector<uint8_t> PMTiles::decompressTile(const std::vector<uint8_t>& compressedData)
{
    switch (getCompression())
    {
    case COMPRESSION_NONE:
        return compressedData;
        
    case COMPRESSION_GZIP:
#ifdef OSGEARTH_HAVE_ZLIB
        {
            z_stream strm = {};
            if (inflateInit2(&strm, 15 + 16) != Z_OK) // 15+16 for gzip format
                return std::vector<uint8_t>();

            strm.avail_in = compressedData.size();
            strm.next_in = const_cast<uint8_t*>(compressedData.data());

            std::vector<uint8_t> result;
            uint8_t buffer[8192];
            
            int ret;
            do {
                strm.avail_out = sizeof(buffer);
                strm.next_out = buffer;
                
                ret = inflate(&strm, Z_NO_FLUSH);
                if (ret == Z_STREAM_ERROR)
                    break;
                    
                unsigned have = sizeof(buffer) - strm.avail_out;
                result.insert(result.end(), buffer, buffer + have);
            } while (ret != Z_STREAM_END);
            
            inflateEnd(&strm);
            return (ret == Z_STREAM_END) ? result : std::vector<uint8_t>();
        }
#else
        OE_WARN << LC << "GZIP decompression not available (zlib not found)" << std::endl;
        return std::vector<uint8_t>();
#endif
        
    case COMPRESSION_BROTLI:
    case COMPRESSION_ZSTD:
        OE_WARN << LC << "Compression type " << (int)getCompression() << " not yet supported" << std::endl;
        return std::vector<uint8_t>();
        
    default:
        OE_WARN << LC << "Unknown compression type: " << (int)getCompression() << std::endl;
        return std::vector<uint8_t>();
    }
}

PMTiles::Directory PMTiles::parseDirectory(const std::vector<uint8_t>& data)
{
    Directory directory;
    
    if (data.empty())
        return directory;

    const uint8_t* ptr = data.data();
    const uint8_t* end = ptr + data.size();
    
    try {
        // Read number of entries
        uint64_t numEntries = readVarint(ptr, end);
        
        // Read tile IDs
        std::vector<uint64_t> tileIds;
        tileIds.reserve(numEntries);
        
        uint64_t lastTileId = 0;
        for (uint64_t i = 0; i < numEntries; ++i) {
            uint64_t delta = readVarint(ptr, end);
            lastTileId += delta;
            tileIds.push_back(lastTileId);
        }
        
        // Read run lengths
        std::vector<uint32_t> runLengths;
        runLengths.reserve(numEntries);
        
        for (uint64_t i = 0; i < numEntries; ++i) {
            uint64_t runLength = readVarint(ptr, end);
            runLengths.push_back(static_cast<uint32_t>(runLength));
        }
        
        // Read lengths
        std::vector<uint32_t> lengths;
        lengths.reserve(numEntries);
        
        for (uint64_t i = 0; i < numEntries; ++i) {
            uint64_t length = readVarint(ptr, end);
            lengths.push_back(static_cast<uint32_t>(length));
        }
        
        // Read offsets and build entries
        uint64_t lastOffset = 0;
        for (uint64_t i = 0; i < numEntries; ++i) {
            uint64_t delta = readVarint(ptr, end);
            lastOffset += delta;
            
            directory.emplace_back(tileIds[i], lastOffset, lengths[i], runLengths[i]);
        }
    }
    catch (const std::exception& e) {
        OE_WARN << LC << "Error parsing directory: " << e.what() << std::endl;
        directory.clear();
    }
    
    return directory;
}

uint64_t PMTiles::readVarint(const uint8_t*& data, const uint8_t* end)
{
    uint64_t result = 0;
    int shift = 0;
    
    while (data < end) {
        uint8_t byte = *data++;
        result |= static_cast<uint64_t>(byte & 0x7F) << shift;
        
        if ((byte & 0x80) == 0)
            break;
            
        shift += 7;
        if (shift >= 64)
            throw std::runtime_error("Varint too long");
    }
    
    return result;
}