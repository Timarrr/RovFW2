cmake_minimum_required(VERSION 3.10)
project("HighROV-MATE" VERSION 0.7)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-D__SOFTFP__ -mlittle-endian) # needed
add_compile_options(-Ofast -pipe -mtune=cortex-m0plus -march=armv6-m) # optimisation
add_compile_options(-felide-constructors -fomit-frame-pointer) # more optimisation
add_compile_options(-Wl,--as-needed) # linker optimisation
add_compile_options(-DUSB_MANUFACTURER=\"CfRD\" -DUSB_PRODUCT=\"TideHunter\" ) # cosmetic things
add_compile_options(-I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/arm-none-eabi/include/c++/7.2.1 -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/arm-none-eabi/include/c++/7.2.1/arm-none-eabi/thumb/v6-m -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/arm-none-eabi/include/c++/7.2.1/backward -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/lib/gcc/arm-none-eabi/7.2.1/include -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/lib/gcc/arm-none-eabi/7.2.1/include-fixed -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/arm-none-eabi/include -I$HOME/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/packaged-toolchain/arm-none-eabi/include)
add_compile_options(-I$HOME/.arduino15/packages/arduino/tools/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL)

add_subdirectory(SAMD_ISR_Servo/)
add_executable(main main.cpp rov.cpp networking.cpp sensors.cpp thrusters.cpp cameras.cpp isrcontroller.cpp manipulator.cpp helpers.cpp logger.cpp imu.cpp regulators.cpp syscalls.cpp syscalls.h)
target_link_libraries(main PUBLIC samdisr)
target_link_arduino_libraries(main PRIVATE Wire core SparkFun_PCA9536_Arduino_Library Servo SPI Ethernet MS5837)
target_enable_arduino_upload(main)