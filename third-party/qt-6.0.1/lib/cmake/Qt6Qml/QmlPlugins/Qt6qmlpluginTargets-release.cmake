#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::qmlplugin" for configuration "Release"
set_property(TARGET Qt6::qmlplugin APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::qmlplugin PROPERTIES
  IMPORTED_COMMON_LANGUAGE_RUNTIME_RELEASE ""
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/./qml/QtQml/qmlplugin.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::qmlplugin )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::qmlplugin "${_IMPORT_PREFIX}/./qml/QtQml/qmlplugin.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
