cmake_minimum_required(VERSION 3.5)
project(EventSimulation)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES src/EventSimulation.c)

add_executable(EventSimulation ${SOURCE_FILES})