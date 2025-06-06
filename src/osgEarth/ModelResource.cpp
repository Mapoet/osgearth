/* osgEarth
 * Copyright 2025 Pelican Mapping
 * MIT License
 */
#include <osgEarth/ModelResource>
#include <osgEarth/StringUtils>
#include <osgEarth/ImageUtils>
#include <osgEarth/Metrics>
#include <osgUtil/Optimizer>
#include <osg/ComputeBoundsVisitor>

#define LC "[ModelResource] "

using namespace osgEarth;

//---------------------------------------------------------------------------

ModelResource::ModelResource(const Config& conf) :
    InstanceResource(conf),
    _canScaleToFitXY(true),
    _canScaleToFitZ(true)
{
    mergeConfig(conf);
}

void
ModelResource::mergeConfig( const Config& conf )
{
    conf.get("can_scale_to_fit_xy", _canScaleToFitXY);
    conf.get("can_scale_to_fit_z",  _canScaleToFitZ);
}

Config
ModelResource::getConfig() const
{
    Config conf = InstanceResource::getConfig();
    conf.key() = "model";
    conf.set("can_scale_to_fit_xy", _canScaleToFitXY);
    conf.set("can_scale_to_fit_z",  _canScaleToFitZ);
    return conf;
}

const osg::BoundingBox&
ModelResource::getBoundingBox(const osgDB::Options* dbo)
{
    if ( !_bbox.valid() && _status.isOK() )
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if ( !_bbox.valid() )
        {
            osg::ref_ptr<osg::Node> node = createNodeFromURI( uri().get(), dbo );
            if ( node.valid() )
            {
                osg::ComputeBoundsVisitor cbv;
                node->accept(cbv);
                _bbox = cbv.getBoundingBox();
            }
        }
    }
    return _bbox;
}

namespace
{
    struct SetUnRefPolicyToFalse : public TextureAndImageVisitor
    {
        void apply(osg::Texture& texture)
        {
            texture.setUnRefImageDataAfterApply(false);
        }
    };
}

osg::Node*
ModelResource::createNodeFromURI( const URI& uri, const osgDB::Options* dbOptions ) const
{
    if (_status.isError())
        return 0L;

    OE_PROFILING_ZONE;

    osg::ref_ptr< osgDB::Options > options = dbOptions ? new osgDB::Options( *dbOptions ) : 0L;

    // Explicitly cache images so that models that share images will only load one copy.
    // If the options struture doesn't contain an object cache, OSG will use the global
    // object cache stored in the Registry. Without this, models that share textures or
    // use an atlas will duplicate memory usage.
    //
    // I don't like having this here - it seems like it belongs elsewhere and the caller
    // should be passing it in. But it needs to be set, so keep for now.
    if ( options.valid() )
    {
        options->setObjectCacheHint( osgDB::Options::CACHE_IMAGES );
    }

    osg::Node* node = 0L;

    ReadResult r = uri.readNode( options.get() );
    if ( r.succeeded() )
    {
        node = r.releaseNode();
        
        OE_DEBUG << LC << "Loaded " << uri.base() << " (from " << (r.isFromCache()? "cache" : "source") << ")"
            << std::endl;

        osgUtil::Optimizer o;
        o.optimize( node,
            o.REMOVE_REDUNDANT_NODES |
            o.COMBINE_ADJACENT_LODS  |
            o.MERGE_GEOMETRY         |
            o.MAKE_FAST_GEOMETRY     |
            o.CHECK_GEOMETRY         |
            o.SHARE_DUPLICATE_STATE  |
            o.INDEX_MESH             |
            o.VERTEX_PRETRANSFORM    |
            o.VERTEX_POSTTRANSFORM );
        

        // Disable automatic texture unref since resources can be shared/paged.
        SetUnRefPolicyToFalse visitor;
        node->accept( visitor );
    }
    else // failing that, fall back on the old encoding format..
    {
        auto tok = StringTokenizer()
            .delim("(").delim(")")
            .standardQuotes()
            .tokenize(*uri);

        if (tok.size() >= 2)
        {
            node = createNodeFromURI( URI(tok[1]), options.get() );
        }
    }

    if (node == 0L && _status.isOK())
    {
        _status = Status::Error(Status::ServiceUnavailable, "Failed to load resource file");
    }

    return node;
}
