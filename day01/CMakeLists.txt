cmake_minimum_required(VERSION 3.0)

project(AoC_Day01_Part01)

# 设置C++标准为C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置可执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(source/part01)
add_subdirectory(source/part02)