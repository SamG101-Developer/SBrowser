#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QmlDebug" for configuration "Release"
set_property(TARGET Qt6::QmlDebug APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::QmlDebug PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/Qt6QmlDebug.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::QmlDebug )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::QmlDebug "${_IMPORT_PREFIX}/lib/Qt6QmlDebug.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
