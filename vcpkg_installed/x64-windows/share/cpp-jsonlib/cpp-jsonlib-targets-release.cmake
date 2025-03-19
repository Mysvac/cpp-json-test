#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "jsonlib::jsonlib" for configuration "Release"
set_property(TARGET jsonlib::jsonlib APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(jsonlib::jsonlib PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/jsonlib.lib"
  )

list(APPEND _cmake_import_check_targets jsonlib::jsonlib )
list(APPEND _cmake_import_check_files_for_jsonlib::jsonlib "${_IMPORT_PREFIX}/lib/jsonlib.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
