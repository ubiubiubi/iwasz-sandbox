# GCC toolchain prefix
SET(TOOLCHAIN_PREFIX "/home/iwasz/local/share/gcc-arm-none-eabi-4_7-2013q1")
SET(TARGET_TRIPLET "arm-none-eabi")
SET(STM_DIR "/home/iwasz/local/share/STM32F4-Discovery_FW_V1.1.0")

SET(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
SET(TOOLCHAIN_INC_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/include)
SET(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/lib)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-g++)
SET(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-as)
SET(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objcopy)
SET(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_DIR}/${TARGET_TRIPLET}-objdump)

SET(CMAKE_C_FLAGS "-mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -std=gnu99" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-std=c++11 -isystem ${TOOLCHAIN_INC_DIR} -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fno-builtin -Wall  -fdata-sections -ffunction-sections" CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_ASM_FLAGS "-mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16" CACHE INTERNAL "asm compiler flags")

SET(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb -gstabs+" CACHE INTERNAL "c debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g -gstabs+" CACHE INTERNAL "cxx debug compiler flags")
SET(CMAKE_ASM_FLAGS_DEBUG "-g -gstabs+" CACHE INTERNAL "asm debug compiler flags")

SET(CMAKE_C_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "c release compiler flags")
SET(CMAKE_CXX_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "cxx release compiler flags")
SET(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "asm release compiler flags")

SET(CMAKE_EXE_LINKER_FLAGS "-mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T${CMAKE_CURRENT_BINARY_DIR}/stm32f4.ld " CACHE INTERNAL "exe link flags")
SET(CMAKE_MODULE_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR} -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -flto" CACHE INTERNAL "module link flags")
SET(CMAKE_SHARED_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR} -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 -flto" CACHE INTERNAL "shared link flags")

SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET} CACHE INTERNAL "cross root directory")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH CACHE INTERNAL "")
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE INTERNAL "")
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE INTERNAL "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

INCLUDE_DIRECTORIES(
    ${STM_DIR}/Utilities/STM32F4-Discovery
    ${STM_DIR}/Libraries/CMSIS/Include
    ${STM_DIR}/Libraries/CMSIS/ST/STM32F4xx/Include
    ${STM_DIR}/Libraries/STM32F4xx_StdPeriph_Driver/inc    
)

enable_language(ASM-ATT)

