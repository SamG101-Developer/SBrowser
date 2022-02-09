cmake_minimum_required(VERSION 3.21)
project(SBrowser)

# TODO -> change Debug / Release lib directories depending on the build type
# TODO -> ie (v8) cmake-build-debug only contains Debug .lib & .dll files
# TODO -> ie (qt) cmake_build-debug only contains <...d> .lib & .dll files

# Set the c++ version, and the binary output directories
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BUILD")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/BUILD")

# Discover all of the first party .hpp and .cpp files
file(GLOB_RECURSE first_party_SRC CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE first_party_INC CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR} src/*.hpp)

# Include the directories for the 1st party root directory
include_directories(src)
include_directories(src/javascript/web_apis)

# Include the directories for the 3rd party header only libraries
include_directories(third-party)

# Link V8
include_directories(third-party/v8-v142-x64.9.3.345.3/include)
file(GLOB v8_LIBs ${CMAKE_SOURCE_DIR}/third-party/v8-v142-x64.9.3.345.3/lib/Debug/*.lib)
file(GLOB v8_DLLs ${CMAKE_SOURCE_DIR}/third-party/v8.redist-v142-x64.9.3.345.3/lib/Debug/*.dll)
link_libraries(${v8_LIBs})

# Link Qt6
include_directories(third-party/qt-6.0.1/include)
file(GLOB qt_LIBs ${CMAKE_SOURCE_DIR}/third-party/qt-6.0.1/lib/*.lib)
file(GLOB qt_DLLs ${CMAKE_SOURCE_DIR}/third-party/qt-6.0.1/bin/*.dll)
link_libraries(${qt_LIBs})

# Create exe
add_executable(SBrowser main.cpp ${first_party_SRC} ${first_party_INC})

# Move all dlls into the binary output directory TODO -> change to file(COPY ... DESTINATION ...)
foreach(v8_DLL ${v8_DLLs})
    add_custom_command(TARGET SBrowser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${v8_DLL} $<TARGET_FILE_DIR:SBrowser>)
endforeach()

foreach(qt_DLL ${qt_DLLs})
    add_custom_command(TARGET SBrowser POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${qt_DLL} $<TARGET_FILE_DIR:SBrowser>)
endforeach()


file(COPY third-party/qt-6.0.1/plugins/platforms DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
