#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::PrintSupport" for configuration "Release"
set_property(TARGET Qt6::PrintSupport APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::PrintSupport PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Qt6PrintSupport.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Qt6PrintSupport.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::PrintSupport )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::PrintSupport "${_IMPORT_PREFIX}/lib/Qt6PrintSupport.lib" "${_IMPORT_PREFIX}/bin/Qt6PrintSupport.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
