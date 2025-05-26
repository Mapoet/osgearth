#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "osgEarth::osgEarth" for configuration "Release"
set_property(TARGET osgEarth::osgEarth APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(osgEarth::osgEarth PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE "GEOS::geos_c"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.3.7.3"
  IMPORTED_SONAME_RELEASE "libosgEarth.so.174"
  )

list(APPEND _cmake_import_check_targets osgEarth::osgEarth )
list(APPEND _cmake_import_check_files_for_osgEarth::osgEarth "${_IMPORT_PREFIX}/lib/x86_64-linux-gnu/libosgEarth.so.3.7.3" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
