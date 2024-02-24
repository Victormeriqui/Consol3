cmake_minimum_required(VERSION 3.5)

project(Consol3 VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(WIN32)
    add_definitions(-DSYS_WINDOWS)
    message(STATUS "Building for Windows")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")  # Compiler optimization level
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /INCREMENTAL:NO /LTCG")  # Linker optimizations
elseif(APPLE)
    add_definitions(-DSYS_MAC)
    message(STATUS "Building for Mac")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")  # Compiler optimization level
elseif(UNIX)
    add_definitions(-DSYS_LINUX)
    message(STATUS "Building for Linux")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")  # Compiler optimization level
endif()

if(NOT CMAKE_BUILD_TYPE)
    # Set the default build type to Release
    set(CMAKE_BUILD_TYPE Release)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.hpp")
set(source_folders 
    PRIVATE ${CMAKE_SOURCE_DIR}/src
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Display
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Linux
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Mac
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Multiplatform
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Windows
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Input
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Rendering
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Lighting
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Shaders
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Resources
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Resources/Util
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Game
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Math
    PRIVATE ${CMAKE_SOURCE_DIR}/src/Math/Util
)

add_executable(Consol3_raster ${SOURCES} ${HEADERS})
target_compile_definitions(Consol3_raster PUBLIC -DGAME_RASTER)
add_executable(Consol3_voxel ${SOURCES} ${HEADERS})
target_compile_definitions(Consol3_voxel PUBLIC -DGAME_VOXEL)

target_include_directories(Consol3_raster ${source_folders})
target_include_directories(Consol3_voxel ${source_folders})

if(APPLE)
    target_link_libraries(Consol3_raster "-framework CoreGraphics -framework CoreFoundation")
    target_link_libraries(Consol3_voxel "-framework CoreGraphics -framework CoreFoundation")
endif()

