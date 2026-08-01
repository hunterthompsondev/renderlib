project(hello-triangle)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

find_package(glfw3 REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_PROJECT_NAME}
    PUBLIC
        glfw
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GLFW_EXPOSE_NATIVE_WAYLAND
)
