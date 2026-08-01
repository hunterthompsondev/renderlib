cmake_minimum_required(VERSION 3.16)
project(renderlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} STATIC
    src/RenderLib.cpp
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

add_subdirectory(examples/hello-triangle)
add_subdirectory(examples/hello-sphere)
add_subdirectory(examples/suzanne-scatter)
add_subdirectory(src/core)
add_subdirectory(src/backends/vulkan)

