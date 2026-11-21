project(rhi-hello-triangle)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        ${CMAKE_PROJECT_NAME}
)
