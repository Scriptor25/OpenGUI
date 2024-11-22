cmake_minimum_required(VERSION 3.30)

project(OpenGUI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sources "src/*.cpp" "include/*.hpp")
add_library(opengui STATIC ${sources})
target_include_directories(opengui PUBLIC "include")

file(GLOB_RECURSE example_sources "example/*.cpp" "example/*.hpp")
add_executable(example ${example_sources})
target_include_directories(example PRIVATE "example")
target_link_libraries(example PRIVATE opengui)
