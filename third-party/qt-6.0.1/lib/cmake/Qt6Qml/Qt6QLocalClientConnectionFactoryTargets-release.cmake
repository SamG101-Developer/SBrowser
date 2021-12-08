#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::QLocalClientConnectionFactory" for configuration "Release"
set_property(TARGET Qt6::QLocalClientConnectionFactory APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::QLocalClientConnectionFactory PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/./plugins/qmltooling/qmldbg_local.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::QLocalClientConnectionFactory )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::QLocalClientConnectionFactory "${_IMPORT_PREFIX}/./plugins/qmltooling/qmldbg_local.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
