cmake_minimum_required(VERSION 3.0)

project(Consol3 VERSION 1.0.0)

set_property(TARGET Consol3 PROPERTY CXX_STANDARD 17)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

file(GLOB_RECURSE HEADERS
    "src/*.hpp"
)

add_executable(Consol3 ${SOURCES} ${HEADERS})

target_include_directories(Consol3
PRIVATE ${CMAKE_SOURCE_DIR}/src
PRIVATE ${CMAKE_SOURCE_DIR}/src/Display
PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Linux
PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Multiplatform
PRIVATE ${CMAKE_SOURCE_DIR}/src/Display/Windows
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Rendering
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Lighting
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Shaders
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Resources
PRIVATE ${CMAKE_SOURCE_DIR}/src/Engine/Resources/Util
PRIVATE ${CMAKE_SOURCE_DIR}/src/Game
PRIVATE ${CMAKE_SOURCE_DIR}/src/Math
PRIVATE ${CMAKE_SOURCE_DIR}/src/Math/Util
)
