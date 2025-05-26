# Install script for directory: /mnt/d/works/osgearth/src/osgEarthImGui

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarthImGui.so.3.7.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarthImGui.so.174"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu" TYPE SHARED_LIBRARY FILES
    "/mnt/d/works/osgearth/build/src/osgEarthImGui/libosgEarthImGui.so.3.7.3"
    "/mnt/d/works/osgearth/build/src/osgEarthImGui/libosgEarthImGui.so.174"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarthImGui.so.3.7.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/libosgEarthImGui.so.174"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/mnt/d/works/osgearth/build/src/osgEarth:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu" TYPE SHARED_LIBRARY FILES "/mnt/d/works/osgearth/build/src/osgEarthImGui/libosgEarthImGui.so")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/osgEarthImGui" TYPE FILE FILES
    "/mnt/d/works/osgearth/src/osgEarthImGui/Common"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ImGuiEventHandler"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ImGuiApp"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ImGuiPanel"
    "/mnt/d/works/osgearth/src/osgEarthImGui/imgui.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/imgui_internal.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/imgui_impl_opengl3.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/imconfig.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/imnodes.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/implot.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/implot_internal.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/implot3d.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/implot3d_internal.h"
    "/mnt/d/works/osgearth/src/osgEarthImGui/AnnotationsGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/CameraGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ContentBrowserGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/EnvironmentGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/LayersGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/NetworkMonitorGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/NotifyGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/OpenEarthFileGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/LiveCamerasGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/NodeGraphGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/PickerGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/RenderingGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/SceneGraphGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/SearchGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ShaderGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/SystemGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/TerrainGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/TextureInspectorGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/ViewpointsGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/FeatureEditGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/LifeMapLayerGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/TerrainEditGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/TextureSplattingLayerGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/VegetationLayerGUI"
    "/mnt/d/works/osgearth/src/osgEarthImGui/CesiumIonGUI"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarthImGui-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarthImGui-targets.cmake"
         "/mnt/d/works/osgearth/build/src/osgEarthImGui/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarthImGui-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarthImGui-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth/osgEarthImGui-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth" TYPE FILE FILES "/mnt/d/works/osgearth/build/src/osgEarthImGui/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarthImGui-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/x86_64-linux-gnu/cmake/osgearth" TYPE FILE FILES "/mnt/d/works/osgearth/build/src/osgEarthImGui/CMakeFiles/Export/26a71f9b096e1e5b96221e58b65d15bc/osgEarthImGui-targets-release.cmake")
  endif()
endif()

