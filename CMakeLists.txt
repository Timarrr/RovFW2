cmake_minimum_required(VERSION 3.10)
project("HighROV-MATE" VERSION 0.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-D__SOFTFP__ -mlittle-endian) # needed
add_compile_options(-Ofast -pipe -mtune=cortex-m0plus -march=armv6-m) # optimisation
add_compile_options(-felide-constructors) # more optimisation
add_compile_options(-DUSB_MANUFACTURER=\"CfRD\" -DUSB_PRODUCT=\"TideHunter\" ) # cosmetic things

add_subdirectory(SAMD_ISR_Servo/)
add_executable(main main.cpp rov.cpp networking.cpp sensors.cpp thrusters.cpp cameras.cpp isrcontroller.cpp manipulator.cpp helpers.cpp logger.cpp imu.cpp regulators.cpp syscalls.cpp debug.cpp ms5837.cpp)
target_link_libraries(main PUBLIC samdisr)
target_link_arduino_libraries(main PRIVATE Wire core SparkFun_PCA9536_Arduino_Library Servo SPI Ethernet)
target_enable_arduino_upload(main)