cmake_minimum_required(VERSION 3.14)
project(ffmpeg-cpp C CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib/glfw)

add_executable(ffmpeg-cpp src/main.cpp)

target_link_libraries(ffmpeg-cpp glfw)
