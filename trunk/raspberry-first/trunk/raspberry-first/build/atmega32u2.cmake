SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes -Wl,--gc-sections")
SET(CXXWARN "-Wall -Wl,--gc-sections")
SET(CTUNING "-fno-exceptions -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
SET(COPT "-Os")
SET(CMCU "-mmcu=atmega32u2")
SET (DUDE_CHIP_NAME "m32u2") 
SET(CDEFS "-DF_CPU=16000000")

SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CXXWARN} ${CEXTRA}")

SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})
SET (CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET (CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
