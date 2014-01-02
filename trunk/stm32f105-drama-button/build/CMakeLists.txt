cmake_minimum_required(VERSION 2.8)
SET (CMAKE_VERBOSE_MAKEFILE OFF) 

PROJECT (stm32-drama-button)
include (stm32f105-crosstool.cmake)
ADD_DEFINITIONS(-DUSE_STDPERIPH_DRIVER)
ADD_DEFINITIONS(-DSTM32F10X_CL)

INCLUDE_DIRECTORIES("../src/")
AUX_SOURCE_DIRECTORY ("../src/" APP_SOURCES)

# According to : http://www.atollic.com/index.php/truestudio/building/tsbuildercompiler, TrueSTUDIO uses GCC, so I assume this startup code is OK for any GCC.
LIST (APPEND APP_SOURCES "../3rdparty/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_cl.s")
LIST (APPEND APP_SOURCES "../3rdparty/CMSIS/CM3/CoreSupport/core_cm3.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32F10x_StdPeriph_Driver/src/misc.c")

LIST (APPEND APP_SOURCES "../3rdparty/STM32_USB_OTG_Driver/src/usb_core.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32_USB_OTG_Driver/src/usb_dcd_int.c")
LIST (APPEND APP_SOURCES "../3rdparty/STM32_USB_OTG_Driver/src/usb_dcd.c")

AUX_SOURCE_DIRECTORY ("../3rdparty/STM32_USB_Device_Library/Core/src/" APP_SOURCES)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}.elf ${APP_SOURCES})

ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.hex ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Oihex ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.hex)
ADD_CUSTOM_TARGET(${CMAKE_PROJECT_NAME}.bin ALL DEPENDS ${CMAKE_PROJECT_NAME}.elf COMMAND ${CMAKE_OBJCOPY} -Obinary ${CMAKE_PROJECT_NAME}.elf ${CMAKE_PROJECT_NAME}.bin)
