/* osgEarth
* Copyright 2008-2014 Pelican Mapping
* MIT License
*/
#include <osgViewer/Viewer>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgEarth/NodeUtils>
#include <osgEarth/LineDrawable>
#include <osgEarth/Registry>
#include <osgEarth/EarthManipulator>
#include <osgEarth/ExampleResources>
#include <osgEarth/Controls>
#include <osgEarthTriton/TritonAPIWrapper>
#include <osgEarthTriton/TritonCallback>
#include <osgEarthTriton/TritonLayer>
#include <osgEarth/AnnotationLayer>
#include <osgEarth/PlaceNode>
#include <osgEarth/GeoPositionNode>

#define LC "[osgearth_triton] "

using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Triton;
namespace ui = osgEarth::Util::Controls;

struct Settings
{
    optional<double> chop;
    optional<double> seaState;
    optional<float> alpha;
    osg::observer_ptr<TritonLayer> tritonLayer;
    
    void apply(Environment& env, Ocean& ocean)
    {
        if (chop.isSet())
        {
            ocean.SetChoppiness(chop.get());
            chop.clear();
        }

        if (seaState.isSet())
        {
            env.SimulateSeaState(seaState.get(), 0.0);
            seaState.clear();
        }

        osg::ref_ptr<TritonLayer> layer;
        if (alpha.isSet() && tritonLayer.lock(layer))
        {
            layer->setOpacity(alpha.value());
            alpha.clear();
        }
    }
};

class TritonCallback : public osgEarth::Triton::Callback
{
public:
    TritonCallback(Settings& settings) : _settings(settings) { }

    void onInitialize(Environment& env, Ocean& ocean)
    {
        //nop
    }

    void onDrawOcean(Environment& env, Ocean& ocean)
    {
        _settings.apply(env, ocean);
    }

    Settings& _settings;
};


struct App
{
    App()
    {
        tritonLayer = NULL;
        map = NULL;
        const double lon = -118.5406, lat = 32.7838;

        anchor.set(
            SpatialReference::get("wgs84"), lon, lat, 0.0,
            ALTMODE_ABSOLUTE);

        isect = new Triton::TritonIntersections();
    }

    Map*         map;
    TritonLayer* tritonLayer;
    Settings     settings;
    osg::ref_ptr<Triton::TritonIntersections> isect;
    AnnotationLayer* labels;
    AnnotationLayer* normals;
    LineDrawable* normalDrawable;
    GeoPoint anchor;

    void addTriton()
    {
        tritonLayer = new TritonLayer();
        tritonLayer->setUserName("my_user_name");
        tritonLayer->setLicenseCode("my_license_code");
        tritonLayer->setMaxAltitude(100000);
        tritonLayer->setUseHeightMap(true);

        const char* ev_t = ::getenv("TRITON_PATH");
        if ( ev_t )
        {
            tritonLayer->setResourcePath(osgDB::concatPaths(std::string(ev_t), "Resources"));

            OE_INFO << LC 
                << "Setting resource path to << " << tritonLayer->getResourcePath()
                << std::endl;
        }
        else
        {
            OE_WARN << LC
                << "No resource path! Triton might not initialize properly. "
                << "Consider setting the TRITON_PATH environment variable."
                << std::endl;
        }

        tritonLayer->setUserCallback(new TritonCallback(settings));
        map->addLayer(tritonLayer);
        settings.tritonLayer = tritonLayer;

        tritonLayer->addIntersections(isect.get());
    }

    void removeTriton()
    {
        if (tritonLayer)
            map->removeLayer(tritonLayer);
        tritonLayer = 0L;
    }

    void addBuoyancyTest(osg::Node* model)
    {
        //labels = new AnnotationLayer();
        //map->addLayer(labels);

        normals = new AnnotationLayer();
        map->addLayer(normals);

        // geometry for a unit normal vector
        normalDrawable = new LineDrawable(GL_LINES);
        normalDrawable->setColor(osg::Vec4(1,1,0,1));
        normalDrawable->pushVertex(osg::Vec3(0,0,0));
        normalDrawable->pushVertex(osg::Vec3(0,0,10));
        normalDrawable->pushVertex(osg::Vec3(-2,0,0.5));
        normalDrawable->pushVertex(osg::Vec3(2,0,0.5));
        normalDrawable->pushVertex(osg::Vec3(0,-2,0.5));
        normalDrawable->pushVertex(osg::Vec3(0,2,0.5));
        normalDrawable->finish();

        // a single shared anchor point for the intersection set:
        isect->setAnchor(anchor);

        // generate a bunch of local points around the anchor:
        for(int x=-50; x<=50; x+=25)
        {
            for(int y=-50; y<=50; y+=25)
            {
                isect->addLocalPoint(osg::Vec3d(x, y, 0));

                // a label communicating the wave height:
                //PlaceNode* label = new PlaceNode();
                //label->setDynamic(true);
                //label->setPosition(anchor);
                //label->setIconImage(image);
                //label->setText("-");
                //labels->getGroup()->addChild(label);

                // a normal vector and optional model:
                GeoPositionNode* normal = new GeoPositionNode();
                normal->setDynamic(true);
                normal->getPositionAttitudeTransform()->addChild(normalDrawable);
                if (model)
                    normal->getPositionAttitudeTransform()->addChild(model);
                normal->setPosition(anchor);
                normals->getGroup()->addChild(normal);
            }
        }

        //ScreenSpaceLayout::setDeclutteringEnabled(false);
    }

    void updateBuoyancyTest()
    {
        for(unsigned i=0; i<isect->getHeights().size(); ++i)
        {
            osg::Vec3d local = isect->getInput()[i];
            local.z() = isect->getHeights()[i];

            //PlaceNode* label = dynamic_cast<PlaceNode*>(labels->getGroup()->getChild(i));
            //label->getPositionAttitudeTransform()->setPosition(local);
            //label->setText(Stringify()<<std::setprecision(2)<<local.z());

            GeoPositionNode* normalNode = dynamic_cast<GeoPositionNode*>(normals->getGroup()->getChild(i));
            normalNode->getPositionAttitudeTransform()->setPosition(local);
            osg::Quat q;
            q.makeRotate(osg::Vec3d(0,0,1), isect->getNormals()[i]);
            normalNode->getPositionAttitudeTransform()->setAttitude(q);
        }
    }
};

App s_app;



template<typename T> struct Set : public ui::ControlEventHandler
{
    optional<T>& _var;
    Set(optional<T>& var) : _var(var) { }
    void onValueChanged(ui::Control*, double value) { _var = value; }
};

struct Toggle : public ui::ControlEventHandler
{
    void onValueChanged(ui::Control*, bool value) {
        if (s_app.tritonLayer)
            s_app.removeTriton();
        else
            s_app.addTriton();
    }
};

ui::Container* createUI()
{
    ui::VBox* box = new ui::VBox();
    box->setBackColor(0,0,0,0.5);
    ui::Grid* grid = box->addControl(new ui::Grid());
    int r=0;
    grid->setControl(0, r, new ui::LabelControl("Chop"));
    grid->setControl(1, r, new ui::HSliderControl(0, 3, 0, new Set<double>(s_app.settings.chop)));
    ++r;
    grid->setControl(0, r, new ui::LabelControl("Sea State"));
    grid->setControl(1, r, new ui::HSliderControl(0, 12, 5, new Set<double>(s_app.settings.seaState)));
    ++r;  
    grid->setControl(0, r, new ui::LabelControl("Alpha"));
    grid->setControl(1, r, new ui::HSliderControl(0, 1.0, 1.0, new Set<float>(s_app.settings.alpha)));
    ++r;
    grid->setControl(0, r, new ui::LabelControl("Toggle"));
    grid->setControl(1, r, new ui::CheckBoxControl(true, new Toggle()));

    grid->getControl(1, r-1)->setHorizFill(true,200);

    return box;
}


int
usage(const char* name)
{
    OE_DEBUG 
        << "\nUsage: " << name << " file.earth" << std::endl
        << osgEarth::Util::MapNodeHelper().usage() << std::endl;

    return 0;
}

int
main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc,argv);

    // help?
    if ( arguments.read("--help") )
        return usage(argv[0]);

    osg::Node* model = 0L;
    std::string filename;
    if (arguments.read("--model", filename))
    {
        model = osgDB::readRefNodeFile(filename).release();
        Registry::shaderGenerator().run(model);
    }

    // create a viewer:
    osgViewer::Viewer viewer(arguments);

    // install our default manipulator (do this before calling load)
    EarthManipulator* manip = new EarthManipulator();
    viewer.setCameraManipulator(manip);

    // load an earth file, and support all or our example command-line options
    // and earth file <external> tags    
    auto node = osgEarth::Util::MapNodeHelper().load(arguments, &viewer);
    if (node.valid() && MapNode::get(node))
    {
        auto group = new osg::Group();
        auto canvas = new ui::ControlCanvas();
        canvas->addChild(createUI());

        group->addChild(node);
        group->addChild(canvas);
        viewer.setSceneData(group);

        s_app.map = MapNode::get( node )->getMap();

        s_app.addTriton();
        s_app.addBuoyancyTest(model);
        
        // Zoom the camera to our area of interest:
        Viewpoint vp;
        vp.heading() = Angle(25, Units::DEGREES);
        vp.pitch() = Angle(-25, Units::DEGREES);
        vp.range() = Distance(400, Units::METERS);
        vp.focalPoint() = s_app.anchor;
        manip->setViewpoint(vp);

        while(!viewer.done())
        {
            viewer.frame();
            s_app.updateBuoyancyTest();
        }
    }
    else
    {
        return usage(argv[0]);
    }
}
