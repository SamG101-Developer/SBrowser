#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QuickControls2" for configuration "Release"
set_property(TARGET Qt6::QuickControls2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::QuickControls2 PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/Qt6QuickControls2.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/Qt6QuickControls2.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::QuickControls2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::QuickControls2 "${_IMPORT_PREFIX}/lib/Qt6QuickControls2.lib" "${_IMPORT_PREFIX}/bin/Qt6QuickControls2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
