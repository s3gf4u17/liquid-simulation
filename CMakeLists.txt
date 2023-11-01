cmake_minimum_required(VERSION 3.10.2)

project(liquid-simulation)

set(CMAKE_CXX_FLAGS "-O3")

set(IMGUI library/imgui/imgui_demo.cpp library/imgui/imgui_draw.cpp library/imgui/imgui_tables.cpp library/imgui/imgui_widgets.cpp library/imgui/imgui.cpp library/imgui/backends/imgui_impl_glfw.cpp library/imgui/backends/imgui_impl_opengl3.cpp)
add_executable(${PROJECT_NAME} source/main.cpp ${IMGUI})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(library/glfw)
add_subdirectory(library/glad)
add_subdirectory(library/glm)

target_include_directories(${PROJECT_NAME} PUBLIC library/imgui)
target_include_directories(${PROJECT_NAME} PUBLIC library/imgui/backends)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad glm)