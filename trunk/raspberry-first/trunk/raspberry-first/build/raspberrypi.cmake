SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER /home/iwasz/local/share/raspi-gcc/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /home/iwasz/local/share/raspi-gcc/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++)

#SET(CFLAGS "-O3 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s")
#SET(CXXFLAGS "-O3 -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s")
SET(CFLAGS "-O3 -march=armv6zk -mtune=arm1176jzf-s")
SET(CXXFLAGS "-O3 -march=armv6zk -mtune=arm1176jzf-s")

SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})
