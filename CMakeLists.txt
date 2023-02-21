cmake_minimum_required(VERSION 3.10)
project("HighROV-MATE" VERSION 0.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(SAMD_ISR_Servo/)

add_executable(main main.cpp rov.cpp networking.cpp sensors.cpp thrusters.cpp cameras.cpp isrcontroller.cpp manipulator.cpp helpers.cpp)
target_link_libraries(main PUBLIC samdisr)
target_link_arduino_libraries(main PRIVATE Wire core SparkFun_PCA9536_Arduino_Library Servo SPI Ethernet MS5837)
target_enable_arduino_upload(main)