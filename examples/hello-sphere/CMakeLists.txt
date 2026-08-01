project(hello-sphere)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        stb-image
)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_PROJECT_NAME}
    PUBLIC
        glfw
        glm::glm
)


target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        GLFW_EXPOSE_NATIVE_WAYLAND
        STB_IMAGE_IMPLEMENTATION
)
