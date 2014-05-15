CMAKE_MINIMUM_REQUIRED (VERSION 2.4.7 FATAL_ERROR)
project (anykey)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

SET(CMAKE_C_FLAGS_DEBUG "-g -O0" CACHE INTERNAL "c debug compiler flags")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++11 " CACHE INTERNAL "cxx debug compiler flags")

SET (CMAKE_C_FLAGS_RELEASE "-O3 -ffast-math -DNDEBUG" CACHE STRING "")
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -std=c++11" CACHE STRING "")
 
ADD_DEFINITIONS (-Wl,-z,defs -fPIC -Wall -Winvalid-pch -ffunction-sections -fdata-sections -pthread)
SET (CMAKE_VERBOSE_MAKEFILE 0)

#Boost
find_package (Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include (FindPkgConfig)
pkg_check_modules (LIBUSBX REQUIRED "libusb-1.0")
INCLUDE_DIRECTORIES (${LIBUSBX_INCLUDE_DIRS})
link_directories(${LIBUSBX_LIBRARY_DIRS})

pkg_check_modules (GTK "gtk+-3.0")
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

AUX_SOURCE_DIRECTORY (../src SOURCES)

# Local includes (inside project).
INCLUDE_DIRECTORIES (../src)
add_executable (${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${LIBUSBX_LIBRARIES})
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${GTK_LIBRARIES})
