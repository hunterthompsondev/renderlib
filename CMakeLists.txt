cmake_minimum_required(VERSION 3.16)
project(renderlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME} STATIC
    src/rhi/RHI.cpp
)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

add_subdirectory(src/rhi)

# Examples
add_subdirectory(examples/rhi-hello-triangle)
