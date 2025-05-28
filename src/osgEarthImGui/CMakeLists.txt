#
# osgEarth ImGui NodeKit
#

set(OSGEARTH_HAVE_IMGUI_NODEKIT ON PARENT_SCOPE) # for BuildConfig

set(LIB_NAME osgEarthImGui)

# This forces the export of all ImGui symbols on Windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# embedded ImGui source code:
set(IMGUI_HEADERS imgui.h imgui_internal.h imgui_impl_opengl3.h imconfig.h imnodes.h implot.h implot_internal.h implot3d.h implot3d_internal.h imstb_rectpack.h imstb_textedit.h imstb_truetype.h)
set(IMGUI_SOURCES imgui.cpp imgui_demo.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp imgui_impl_opengl3.cpp imnodes.cpp implot.cpp implot_demo.cpp implot_items.cpp implot3d.cpp implot3d_demo.cpp implot3d_items.cpp implot3d_meshes.cpp)

set(STOCK_PANELS
    AnnotationsGUI
    CameraGUI
    ContentBrowserGUI
    #DateTimeGUI
    EnvironmentGUI    
    LayersGUI
    NetworkMonitorGUI
    NotifyGUI
    OpenEarthFileGUI
    LiveCamerasGUI
    NodeGraphGUI
    PickerGUI
    RenderingGUI
    SceneGraphGUI
    SearchGUI
    ShaderGUI
    SystemGUI
    TerrainGUI
    TextureInspectorGUI
    ViewpointsGUI
    FeatureEditGUI
    
    # osgEarthProcedural
    LifeMapLayerGUI
    TerrainEditGUI
    TextureSplattingLayerGUI
    VegetationLayerGUI
    
    # osgEarthCesium
    CesiumIonGUI
)

# osgEarth source code:
set(LIB_HEADERS
    Common
    ImGuiEventHandler
    ImGuiApp
    ImGuiPanel )
    
set(LIB_SOURCES 
    ImGuiEventHandler.cpp )

add_osgearth_library(
    TARGET ${LIB_NAME}
    SOURCES ${LIB_SOURCES} ${IMGUI_SOURCES}
    PUBLIC_HEADERS ${LIB_HEADERS} ${IMGUI_HEADERS} ${STOCK_PANELS}
    LIBRARIES PUBLIC OpenGL::GL
    FOLDER "NodeKits")
