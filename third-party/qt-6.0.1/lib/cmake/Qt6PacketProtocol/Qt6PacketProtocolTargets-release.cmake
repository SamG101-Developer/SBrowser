#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt6::PacketProtocol" for configuration "Release"
set_property(TARGET Qt6::PacketProtocol APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(Qt6::PacketProtocol PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/Qt6PacketProtocol.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt6::PacketProtocol )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt6::PacketProtocol "${_IMPORT_PREFIX}/lib/Qt6PacketProtocol.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
