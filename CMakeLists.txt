cmake_minimum_required(VERSION 3.16.3)
project(line-reader)

set(CMAKE_CXX_STANDARD 17)

add_library(line-reader line-reader.cpp)
target_include_directories(line-reader PUBLIC include)
