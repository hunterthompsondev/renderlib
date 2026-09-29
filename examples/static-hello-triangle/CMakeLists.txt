project(static-hello-triangle)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        stb-image-write
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_PROJECT_NAME}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        STB_IMAGE_WRITE_IMPLEMENTATION
)
