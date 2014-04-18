cmake_minimum_required(VERSION 2.8)
SET (CMAKE_VERBOSE_MAKEFILE ON) 

PROJECT (tiva-usb-test)
include (tm4c123.cmake)

INCLUDE_DIRECTORIES("../src/")
AUX_SOURCE_DIRECTORY ("../src/" APP_SOURCES)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.axf ${APP_SOURCES})
#TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME}.axf "/home/iwasz/local/share/arm-unknown-eabi-cortex-m4/bin/../lib/gcc/arm-unknown-eabi/4.7.3/libgcc.a")
#TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME}.axf "/home/iwasz/local/share/arm-unknown-eabi-cortex-m4/bin/../lib/gcc/arm-unknown-eabi/4.7.3/../../../../arm-unknown-eabi/lib/libc.a")
#TARGET_LINK_LIBRARIES (${CMAKE_PROJECT_NAME}.axf "/home/iwasz/local/share/arm-unknown-eabi-cortex-m4/bin/../lib/gcc/arm-unknown-eabi/4.7.3/../../../../arm-unknown-eabi/lib/libm.a")

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)

# st-flash write stm32-cmake-test.bin 0x8000000
