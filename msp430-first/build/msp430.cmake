# GCC toolchain prefix
#SET(TOOLCHAIN_PREFIX "/home/iwasz/local/share/arm-unknown-eabi-cortex-m4")
#SET(TARGET_TRIPLET "arm-unknown-eabi")

#ADD_DEFINITIONS (-DPART_TM4C123GH6PM)
#ADD_DEFINITIONS (-Dgcc)

SET(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
SET(TOOLCHAIN_INC_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/include)
SET(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET}/lib)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_COMPILER msp430-gcc)
SET(CMAKE_CXX_COMPILER msp430-g++)
SET(CMAKE_ASM_COMPILER msp430-as)
SET(CMAKE_OBJCOPY msp430-objcopy)
SET(CMAKE_OBJDUMP msp430-objdump)

SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -std=gnu99 -ffunction-sections -fdata-sections -MD -Wall -pedantic" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fdata-sections -ffunction-sections -MD -Wall -pedantic" CACHE INTERNAL "cxx compiler flags")

SET(CMAKE_C_FLAGS_DEBUG "-ggdb -gstabs+ -DDEBUG" CACHE INTERNAL "c debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "-ggdb -gstabs+ -DDEBUG" CACHE INTERNAL "cxx debug compiler flags")
SET(CMAKE_ASM_FLAGS_DEBUG "-gstabs+" CACHE INTERNAL "asm debug compiler flags")

#SET(CMAKE_MODULE_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR}" CACHE INTERNAL "module link flags")
#SET(CMAKE_SHARED_LINKER_FLAGS "-L${TOOLCHAIN_LIB_DIR}" CACHE INTERNAL "shared link flags")

#SET(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PREFIX}/${TARGET_TRIPLET} CACHE INTERNAL "cross root directory")
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH CACHE INTERNAL "")
#SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY CACHE INTERNAL "")
#SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY CACHE INTERNAL "")
#SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
#SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

enable_language(ASM-ATT)
