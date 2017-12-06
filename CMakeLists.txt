cmake_minimum_required(VERSION 3.5)
project(EventSimulation)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/EventSimulation.cpp)

add_executable(EventSimulation ${SOURCE_FILES})