cmake_minimum_required(VERSION 3.30)

project(OpenGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_STATIC ON)
set(ONLY_LIBS ON)

add_subdirectory("deps/glfw")
add_subdirectory("deps/glew")
add_subdirectory("deps/yaml")

add_library(imgui STATIC
        "deps/imgui/imgui.cpp"
        "deps/imgui/imgui_demo.cpp"
        "deps/imgui/imgui_draw.cpp"
        "deps/imgui/imgui_tables.cpp"
        "deps/imgui/imgui_widgets.cpp"
        "deps/imgui/misc/cpp/imgui_stdlib.cpp"
        "deps/imgui/backends/imgui_impl_glfw.cpp"
        "deps/imgui/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PUBLIC "deps/imgui")
target_link_libraries(imgui PUBLIC glfw)

file(GLOB_RECURSE sources "src/*.cpp" "include/*.hpp")
add_library(opengui STATIC ${sources})
target_include_directories(opengui PUBLIC "include")
target_link_libraries(opengui PUBLIC glfw libglew_static imgui yaml-cpp::yaml-cpp)

file(GLOB_RECURSE example_sources "example/*.cpp" "example/*.hpp")
add_executable(example ${example_sources})
target_include_directories(example PRIVATE "example")
target_link_libraries(example PRIVATE opengui)
