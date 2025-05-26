# Install script for directory: /mnt/d/works/osgearth/src/osgEarth

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.3.7.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.174"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu" TYPE SHARED_LIBRARY FILES
    "/mnt/d/works/osgearth/build/src/osgEarth/libosgEarth.so.3.7.3"
    "/mnt/d/works/osgearth/build/src/osgEarth/libosgEarth.so.174"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.3.7.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.174"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu" TYPE SHARED_LIBRARY FILES "/mnt/d/works/osgearth/build/src/osgEarth/libosgEarth.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgEarth" TYPE FILE FILES
    "/mnt/d/works/osgearth/src/osgEarth/AGG.h"
    "/mnt/d/works/osgearth/src/osgEarth/AltitudeFilter"
    "/mnt/d/works/osgearth/src/osgEarth/AltitudeSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationData"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationLayer"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationNode"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationRegistry"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationSettings"
    "/mnt/d/works/osgearth/src/osgEarth/AnnotationUtils"
    "/mnt/d/works/osgearth/src/osgEarth/ArcGISServer"
    "/mnt/d/works/osgearth/src/osgEarth/ArcGISTilePackage"
    "/mnt/d/works/osgearth/src/osgEarth/AtlasBuilder"
    "/mnt/d/works/osgearth/src/osgEarth/AttributesFilter"
    "/mnt/d/works/osgearth/src/osgEarth/AutoClipPlaneHandler"
    "/mnt/d/works/osgearth/src/osgEarth/AutoScaleCallback"
    "/mnt/d/works/osgearth/src/osgEarth/AzureMaps"
    "/mnt/d/works/osgearth/src/osgEarth/BboxDrawable"
    "/mnt/d/works/osgearth/src/osgEarth/BBoxSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/BillboardResource"
    "/mnt/d/works/osgearth/src/osgEarth/BillboardSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/Bing"
    "/mnt/d/works/osgearth/src/osgEarth/Bounds"
    "/mnt/d/works/osgearth/src/osgEarth/BufferFilter"
    "/mnt/d/works/osgearth/src/osgEarth/BuildGeometryFilter"
    "/mnt/d/works/osgearth/src/osgEarth/BuildTextFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Cache"
    "/mnt/d/works/osgearth/src/osgEarth/CacheBin"
    "/mnt/d/works/osgearth/src/osgEarth/CachePolicy"
    "/mnt/d/works/osgearth/src/osgEarth/CacheSeed"
    "/mnt/d/works/osgearth/src/osgEarth/Callbacks"
    "/mnt/d/works/osgearth/src/osgEarth/Callouts"
    "/mnt/d/works/osgearth/src/osgEarth/CameraUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Capabilities"
    "/mnt/d/works/osgearth/src/osgEarth/CascadeDrapingDecorator"
    "/mnt/d/works/osgearth/src/osgEarth/CentroidFilter"
    "/mnt/d/works/osgearth/src/osgEarth/CesiumIon"
    "/mnt/d/works/osgearth/src/osgEarth/Chonk"
    "/mnt/d/works/osgearth/src/osgEarth/CircleNode"
    "/mnt/d/works/osgearth/src/osgEarth/ClampableNode"
    "/mnt/d/works/osgearth/src/osgEarth/ClampCallback"
    "/mnt/d/works/osgearth/src/osgEarth/Clamping"
    "/mnt/d/works/osgearth/src/osgEarth/ClampingTechnique"
    "/mnt/d/works/osgearth/src/osgEarth/ClipSpace"
    "/mnt/d/works/osgearth/src/osgEarth/ClusterNode"
    "/mnt/d/works/osgearth/src/osgEarth/Color"
    "/mnt/d/works/osgearth/src/osgEarth/ColorFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Common"
    "/mnt/d/works/osgearth/src/osgEarth/Composite"
    "/mnt/d/works/osgearth/src/osgEarth/CompositeFeatureSource"
    "/mnt/d/works/osgearth/src/osgEarth/CompressedArray"
    "/mnt/d/works/osgearth/src/osgEarth/CompositeTiledModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/Config"
    "/mnt/d/works/osgearth/src/osgEarth/Containers"
    "/mnt/d/works/osgearth/src/osgEarth/ContourMap"
    "/mnt/d/works/osgearth/src/osgEarth/ConvertTypeFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Coverage"
    "/mnt/d/works/osgearth/src/osgEarth/CoverageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/CoverageSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/CropFilter"
    "/mnt/d/works/osgearth/src/osgEarth/CssUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Cube"
    "/mnt/d/works/osgearth/src/osgEarth/CullingUtils"
    "/mnt/d/works/osgearth/src/osgEarth/DateTime"
    "/mnt/d/works/osgearth/src/osgEarth/DateTimeRange"
    "/mnt/d/works/osgearth/src/osgEarth/DebugImageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/DecalLayer"
    "/mnt/d/works/osgearth/src/osgEarth/DepthOffset"
    "/mnt/d/works/osgearth/src/osgEarth/Draggers"
    "/mnt/d/works/osgearth/src/osgEarth/DrapeableNode"
    "/mnt/d/works/osgearth/src/osgEarth/DrapingCullSet"
    "/mnt/d/works/osgearth/src/osgEarth/DrapingTechnique"
    "/mnt/d/works/osgearth/src/osgEarth/DrawInstanced"
    "/mnt/d/works/osgearth/src/osgEarth/EarthManipulator"
    "/mnt/d/works/osgearth/src/osgEarth/ECEF"
    "/mnt/d/works/osgearth/src/osgEarth/Elevation"
    "/mnt/d/works/osgearth/src/osgEarth/ElevationLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ElevationLOD"
    "/mnt/d/works/osgearth/src/osgEarth/ElevationPool"
    "/mnt/d/works/osgearth/src/osgEarth/ElevationQuery"
    "/mnt/d/works/osgearth/src/osgEarth/ElevationRanges"
    "/mnt/d/works/osgearth/src/osgEarth/EllipseNode"
    "/mnt/d/works/osgearth/src/osgEarth/Ellipsoid"
    "/mnt/d/works/osgearth/src/osgEarth/Endian"
    "/mnt/d/works/osgearth/src/osgEarth/Ephemeris"
    "/mnt/d/works/osgearth/src/osgEarth/ExampleResources"
    "/mnt/d/works/osgearth/src/osgEarth/Export"
    "/mnt/d/works/osgearth/src/osgEarth/Expression"
    "/mnt/d/works/osgearth/src/osgEarth/Extension"
    "/mnt/d/works/osgearth/src/osgEarth/ExtrudeGeometryFilter"
    "/mnt/d/works/osgearth/src/osgEarth/ExtrusionSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/FadeEffect"
    "/mnt/d/works/osgearth/src/osgEarth/Feature"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureCursor"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureDisplayLayout"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureElevationLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureImageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureImageRTTLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureIndex"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureModelGraph"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureModelSource"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureNode"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureRasterizer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureSDFLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureSource"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureSourceIndexNode"
    "/mnt/d/works/osgearth/src/osgEarth/FeatureStyleSorter"
    "/mnt/d/works/osgearth/src/osgEarth/FileUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Fill"
    "/mnt/d/works/osgearth/src/osgEarth/Filter"
    "/mnt/d/works/osgearth/src/osgEarth/FilterContext"
    "/mnt/d/works/osgearth/src/osgEarth/FilteredFeatureSource"
    "/mnt/d/works/osgearth/src/osgEarth/FlatteningLayer"
    "/mnt/d/works/osgearth/src/osgEarth/Formatter"
    "/mnt/d/works/osgearth/src/osgEarth/FractalElevationLayer"
    "/mnt/d/works/osgearth/src/osgEarth/FrameClock"
    "/mnt/d/works/osgearth/src/osgEarth/GARSGraticule"
    "/mnt/d/works/osgearth/src/osgEarth/GDAL"
    "/mnt/d/works/osgearth/src/osgEarth/GDALDEM"
    "/mnt/d/works/osgearth/src/osgEarth/GeoCommon"
    "/mnt/d/works/osgearth/src/osgEarth/GeoData"
    "/mnt/d/works/osgearth/src/osgEarth/GeodeticGraticule"
    "/mnt/d/works/osgearth/src/osgEarth/GeodeticLabelingEngine"
    "/mnt/d/works/osgearth/src/osgEarth/Geoid"
    "/mnt/d/works/osgearth/src/osgEarth/GeoMath"
    "/mnt/d/works/osgearth/src/osgEarth/Geometry"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryClamper"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryCloud"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryCompiler"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryFactory"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryRasterizer"
    "/mnt/d/works/osgearth/src/osgEarth/GeometryUtils"
    "/mnt/d/works/osgearth/src/osgEarth/GeoPositionNode"
    "/mnt/d/works/osgearth/src/osgEarth/GeoPositionNodeAutoScaler"
    "/mnt/d/works/osgearth/src/osgEarth/GEOS"
    "/mnt/d/works/osgearth/src/osgEarth/GeoTransform"
    "/mnt/d/works/osgearth/src/osgEarth/GLSLChunker"
    "/mnt/d/works/osgearth/src/osgEarth/GLUtils"
    "/mnt/d/works/osgearth/src/osgEarth/GraticuleLabelingEngine"
    "/mnt/d/works/osgearth/src/osgEarth/HeightFieldUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Horizon"
    "/mnt/d/works/osgearth/src/osgEarth/HorizonClipPlane"
    "/mnt/d/works/osgearth/src/osgEarth/HTM"
    "/mnt/d/works/osgearth/src/osgEarth/HTTPClient"
    "/mnt/d/works/osgearth/src/osgEarth/IconResource"
    "/mnt/d/works/osgearth/src/osgEarth/IconSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/ImageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ImageMosaic"
    "/mnt/d/works/osgearth/src/osgEarth/ImageOverlay"
    "/mnt/d/works/osgearth/src/osgEarth/ImageOverlayEditor"
    "/mnt/d/works/osgearth/src/osgEarth/ImageToFeatureLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ImageToHeightFieldConverter"
    "/mnt/d/works/osgearth/src/osgEarth/ImageUtils"
    "/mnt/d/works/osgearth/src/osgEarth/InstanceBuilder"
    "/mnt/d/works/osgearth/src/osgEarth/InstanceCloud"
    "/mnt/d/works/osgearth/src/osgEarth/InstanceResource"
    "/mnt/d/works/osgearth/src/osgEarth/InstanceSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/IntersectionPicker"
    "/mnt/d/works/osgearth/src/osgEarth/IOTypes"
    "/mnt/d/works/osgearth/src/osgEarth/JoinLines"
    "/mnt/d/works/osgearth/src/osgEarth/JsonUtils"
    "/mnt/d/works/osgearth/src/osgEarth/LabelNode"
    "/mnt/d/works/osgearth/src/osgEarth/LandCover"
    "/mnt/d/works/osgearth/src/osgEarth/LandCoverLayer"
    "/mnt/d/works/osgearth/src/osgEarth/LatLongFormatter"
    "/mnt/d/works/osgearth/src/osgEarth/Layer"
    "/mnt/d/works/osgearth/src/osgEarth/LayerReference"
    "/mnt/d/works/osgearth/src/osgEarth/LayerShader"
    "/mnt/d/works/osgearth/src/osgEarth/Lighting"
    "/mnt/d/works/osgearth/src/osgEarth/LinearLineOfSight"
    "/mnt/d/works/osgearth/src/osgEarth/LineDrawable"
    "/mnt/d/works/osgearth/src/osgEarth/LineFunctor"
    "/mnt/d/works/osgearth/src/osgEarth/LineOfSight"
    "/mnt/d/works/osgearth/src/osgEarth/LineSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/LoadableNode"
    "/mnt/d/works/osgearth/src/osgEarth/LocalGeometryNode"
    "/mnt/d/works/osgearth/src/osgEarth/LocalTangentPlane"
    "/mnt/d/works/osgearth/src/osgEarth/Locators"
    "/mnt/d/works/osgearth/src/osgEarth/LODGenerator"
    "/mnt/d/works/osgearth/src/osgEarth/LogarithmicDepthBuffer"
    "/mnt/d/works/osgearth/src/osgEarth/Map"
    "/mnt/d/works/osgearth/src/osgEarth/MapboxGLGlyphManager"
    "/mnt/d/works/osgearth/src/osgEarth/MapboxGLImageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/MapCallback"
    "/mnt/d/works/osgearth/src/osgEarth/MapModelChange"
    "/mnt/d/works/osgearth/src/osgEarth/MapNode"
    "/mnt/d/works/osgearth/src/osgEarth/MapNodeObserver"
    "/mnt/d/works/osgearth/src/osgEarth/MaterialLoader"
    "/mnt/d/works/osgearth/src/osgEarth/Math"
    "/mnt/d/works/osgearth/src/osgEarth/MBTiles"
    "/mnt/d/works/osgearth/src/osgEarth/MeasureTool"
    "/mnt/d/works/osgearth/src/osgEarth/MemCache"
    "/mnt/d/works/osgearth/src/osgEarth/MemoryUtils"
    "/mnt/d/works/osgearth/src/osgEarth/MeshConsolidator"
    "/mnt/d/works/osgearth/src/osgEarth/MeshFlattener"
    "/mnt/d/works/osgearth/src/osgEarth/MeshSubdivider"
    "/mnt/d/works/osgearth/src/osgEarth/MetadataNode"
    "/mnt/d/works/osgearth/src/osgEarth/MetaTile"
    "/mnt/d/works/osgearth/src/osgEarth/Metrics"
    "/mnt/d/works/osgearth/src/osgEarth/MGRSFormatter"
    "/mnt/d/works/osgearth/src/osgEarth/MGRSGraticule"
    "/mnt/d/works/osgearth/src/osgEarth/ModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ModelNode"
    "/mnt/d/works/osgearth/src/osgEarth/ModelResource"
    "/mnt/d/works/osgearth/src/osgEarth/ModelSource"
    "/mnt/d/works/osgearth/src/osgEarth/ModelSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/MVT"
    "/mnt/d/works/osgearth/src/osgEarth/NativeProgramAdapter"
    "/mnt/d/works/osgearth/src/osgEarth/NetworkMonitor"
    "/mnt/d/works/osgearth/src/osgEarth/NodeUtils"
    "/mnt/d/works/osgearth/src/osgEarth/NoiseTextureFactory"
    "/mnt/d/works/osgearth/src/osgEarth/Notify"
    "/mnt/d/works/osgearth/src/osgEarth/ObjectIDPicker"
    "/mnt/d/works/osgearth/src/osgEarth/ObjectIndex"
    "/mnt/d/works/osgearth/src/osgEarth/OGRFeatureSource"
    "/mnt/d/works/osgearth/src/osgEarth/OgrUtils"
    "/mnt/d/works/osgearth/src/osgEarth/optional"
    "/mnt/d/works/osgearth/src/osgEarth/OverlayDecorator"
    "/mnt/d/works/osgearth/src/osgEarth/PagedNode"
    "/mnt/d/works/osgearth/src/osgEarth/PatchLayer"
    "/mnt/d/works/osgearth/src/osgEarth/PBRMaterial"
    "/mnt/d/works/osgearth/src/osgEarth/PhongLightingEffect"
    "/mnt/d/works/osgearth/src/osgEarth/Picker"
    "/mnt/d/works/osgearth/src/osgEarth/PlaceNode"
    "/mnt/d/works/osgearth/src/osgEarth/PluginLoader"
    "/mnt/d/works/osgearth/src/osgEarth/PointDrawable"
    "/mnt/d/works/osgearth/src/osgEarth/PointSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/PolygonizeLines"
    "/mnt/d/works/osgearth/src/osgEarth/PolygonSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/PowerlineLayer"
    "/mnt/d/works/osgearth/src/osgEarth/PrimitiveIntersector"
    "/mnt/d/works/osgearth/src/osgEarth/Profile"
    "/mnt/d/works/osgearth/src/osgEarth/Progress"
    "/mnt/d/works/osgearth/src/osgEarth/Query"
    "/mnt/d/works/osgearth/src/osgEarth/RadialLineOfSight"
    "/mnt/d/works/osgearth/src/osgEarth/Random"
    "/mnt/d/works/osgearth/src/osgEarth/RectangleNode"
    "/mnt/d/works/osgearth/src/osgEarth/RefinePolicy"
    "/mnt/d/works/osgearth/src/osgEarth/Registry"
    "/mnt/d/works/osgearth/src/osgEarth/RenderSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/ResampleFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Resource"
    "/mnt/d/works/osgearth/src/osgEarth/ResourceCache"
    "/mnt/d/works/osgearth/src/osgEarth/ResourceLibrary"
    "/mnt/d/works/osgearth/src/osgEarth/Revisioning"
    "/mnt/d/works/osgearth/src/osgEarth/RTTPicker"
    "/mnt/d/works/osgearth/src/osgEarth/ScaleFilter"
    "/mnt/d/works/osgearth/src/osgEarth/ScatterFilter"
    "/mnt/d/works/osgearth/src/osgEarth/SceneGraphCallback"
    "/mnt/d/works/osgearth/src/osgEarth/ScreenSpaceLayout"
    "/mnt/d/works/osgearth/src/osgEarth/ScreenSpaceLayoutCallout"
    "/mnt/d/works/osgearth/src/osgEarth/ScreenSpaceLayoutDeclutter"
    "/mnt/d/works/osgearth/src/osgEarth/ScreenSpaceLayoutImpl"
    "/mnt/d/works/osgearth/src/osgEarth/Script"
    "/mnt/d/works/osgearth/src/osgEarth/ScriptEngine"
    "/mnt/d/works/osgearth/src/osgEarth/ScriptFilter"
    "/mnt/d/works/osgearth/src/osgEarth/SDF"
    "/mnt/d/works/osgearth/src/osgEarth/SDF"
    "/mnt/d/works/osgearth/src/osgEarth/SelectExtentTool"
    "/mnt/d/works/osgearth/src/osgEarth/Session"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderFactory"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderGenerator"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderLoader"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderMerger"
    "/mnt/d/works/osgearth/src/osgEarth/Shaders"
    "/mnt/d/works/osgearth/src/osgEarth/ShaderUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Shadowing"
    "/mnt/d/works/osgearth/src/osgEarth/SimpleOceanLayer"
    "/mnt/d/works/osgearth/src/osgEarth/SimplePager"
    "/mnt/d/works/osgearth/src/osgEarth/SimplexNoise"
    "/mnt/d/works/osgearth/src/osgEarth/SimplifyFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Skins"
    "/mnt/d/works/osgearth/src/osgEarth/Sky"
    "/mnt/d/works/osgearth/src/osgEarth/SkyView"
    "/mnt/d/works/osgearth/src/osgEarth/SpatialReference"
    "/mnt/d/works/osgearth/src/osgEarth/StarData"
    "/mnt/d/works/osgearth/src/osgEarth/StateSetCache"
    "/mnt/d/works/osgearth/src/osgEarth/StateTransition"
    "/mnt/d/works/osgearth/src/osgEarth/Status"
    "/mnt/d/works/osgearth/src/osgEarth/StringUtils"
    "/mnt/d/works/osgearth/src/osgEarth/Stroke"
    "/mnt/d/works/osgearth/src/osgEarth/Style"
    "/mnt/d/works/osgearth/src/osgEarth/StyleSelector"
    "/mnt/d/works/osgearth/src/osgEarth/StyleSheet"
    "/mnt/d/works/osgearth/src/osgEarth/SubstituteModelFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Symbol"
    "/mnt/d/works/osgearth/src/osgEarth/Tags"
    "/mnt/d/works/osgearth/src/osgEarth/TDTiles"
    "/mnt/d/works/osgearth/src/osgEarth/Terrain"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainConstraintLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainEffect"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainEngineNode"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainEngineRequirements"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainMeshLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainOptions"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainProfile"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainResources"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainTileModel"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainTileModelFactory"
    "/mnt/d/works/osgearth/src/osgEarth/TerrainTileNode"
    "/mnt/d/works/osgearth/src/osgEarth/TessellateOperator"
    "/mnt/d/works/osgearth/src/osgEarth/Tessellator"
    "/mnt/d/works/osgearth/src/osgEarth/Text"
    "/mnt/d/works/osgearth/src/osgEarth/TextSymbol"
    "/mnt/d/works/osgearth/src/osgEarth/TextSymbolizer"
    "/mnt/d/works/osgearth/src/osgEarth/TextureArena"
    "/mnt/d/works/osgearth/src/osgEarth/TextureBuffer"
    "/mnt/d/works/osgearth/src/osgEarth/TFS"
    "/mnt/d/works/osgearth/src/osgEarth/TFSPackager"
    "/mnt/d/works/osgearth/src/osgEarth/Threading"
    "/mnt/d/works/osgearth/src/osgEarth/ThreeDTilesLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TileCache"
    "/mnt/d/works/osgearth/src/osgEarth/TiledFeatureModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TiledModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TileEstimator"
    "/mnt/d/works/osgearth/src/osgEarth/TileHandler"
    "/mnt/d/works/osgearth/src/osgEarth/TileIndex"
    "/mnt/d/works/osgearth/src/osgEarth/TileIndexBuilder"
    "/mnt/d/works/osgearth/src/osgEarth/TileKey"
    "/mnt/d/works/osgearth/src/osgEarth/TileLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TileMesher"
    "/mnt/d/works/osgearth/src/osgEarth/TileRasterizer"
    "/mnt/d/works/osgearth/src/osgEarth/TileSource"
    "/mnt/d/works/osgearth/src/osgEarth/TileSourceElevationLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TileSourceImageLayer"
    "/mnt/d/works/osgearth/src/osgEarth/TileVisitor"
    "/mnt/d/works/osgearth/src/osgEarth/TimeControl"
    "/mnt/d/works/osgearth/src/osgEarth/TimeSeriesImage"
    "/mnt/d/works/osgearth/src/osgEarth/TMS"
    "/mnt/d/works/osgearth/src/osgEarth/TMSBackFiller"
    "/mnt/d/works/osgearth/src/osgEarth/TopologyGraph"
    "/mnt/d/works/osgearth/src/osgEarth/TrackNode"
    "/mnt/d/works/osgearth/src/osgEarth/TransformFilter"
    "/mnt/d/works/osgearth/src/osgEarth/Units"
    "/mnt/d/works/osgearth/src/osgEarth/URI"
    "/mnt/d/works/osgearth/src/osgEarth/Utils"
    "/mnt/d/works/osgearth/src/osgEarth/UTMGraticule"
    "/mnt/d/works/osgearth/src/osgEarth/UTMLabelingEngine"
    "/mnt/d/works/osgearth/src/osgEarth/VerticalDatum"
    "/mnt/d/works/osgearth/src/osgEarth/VideoLayer"
    "/mnt/d/works/osgearth/src/osgEarth/ViewFitter"
    "/mnt/d/works/osgearth/src/osgEarth/Viewpoint"
    "/mnt/d/works/osgearth/src/osgEarth/VirtualProgram"
    "/mnt/d/works/osgearth/src/osgEarth/VisibleLayer"
    "/mnt/d/works/osgearth/src/osgEarth/WFS"
    "/mnt/d/works/osgearth/src/osgEarth/WindLayer"
    "/mnt/d/works/osgearth/src/osgEarth/WireLines"
    "/mnt/d/works/osgearth/src/osgEarth/WMS"
    "/mnt/d/works/osgearth/src/osgEarth/XmlUtils"
    "/mnt/d/works/osgearth/src/osgEarth/XYZ"
    "/mnt/d/works/osgearth/src/osgEarth/XYZFeatureSource"
    "/mnt/d/works/osgearth/src/osgEarth/XYZModelLayer"
    "/mnt/d/works/osgearth/src/osgEarth/rtree.h"
    "/mnt/d/works/osgearth/src/osgEarth/weemesh.h"
    "/mnt/d/works/osgearth/src/osgEarth/weejobs.h"
    "/mnt/d/works/osgearth/src/osgEarth/tinyxml/tinyxml.h"
    "/mnt/d/works/osgearth/src/osgEarth/tinyxml/tinystr.h"
    "/mnt/d/works/osgearth/build/build_include/osgEarth/BuildConfig"
    "/mnt/d/works/osgearth/build/build_include/osgEarth/Version"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarth-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarth-targets.cmake"
         "/mnt/d/works/osgearth/build/src/osgEarth/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarth-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarth-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarth-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth" TYPE FILE FILES "/mnt/d/works/osgearth/build/src/osgEarth/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarth-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth" TYPE FILE FILES "/mnt/d/works/osgearth/build/src/osgEarth/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarth-targets-release.cmake")
  endif()
endif()

