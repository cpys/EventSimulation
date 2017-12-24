cmake_minimum_required(VERSION 3.5)
project(EventSimulation)

set(CMAKE_CXX_STANDARD 11)

set(NET src/NetEventSimulation.cpp)
set(SERIAL src/SerialPortEventSimulation.cpp src/SerialPortClient/SerialPortClient.h src/SerialPortClient/SerialPortClient.cpp)
set(SERIAL_RECV src/SerialPortClient/SerialPortClient.h src/SerialPortClient/SerialPortClient.cpp src/SerialPortEventReceive.cpp)

add_executable(EventSimulation ${NET})
add_executable(SerialPortEventSimulation ${SERIAL})
add_executable(SerialPortEventReceive ${SERIAL_RECV})