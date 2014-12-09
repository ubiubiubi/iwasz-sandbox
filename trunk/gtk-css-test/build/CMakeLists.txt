CMAKE_MINIMUM_REQUIRED (VERSION 2.4.7 FATAL_ERROR)
project (gtk-css-test)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

add_definitions (-Wl,-z,defs)

SET (CMAKE_C_FLAGS "-g -fPIC -Wall -O0 -Winvalid-pch -ffunction-sections -fdata-sections -pthread -std=c++11 -static")
SET (CMAKE_CXX_FLAGS ${CMAKE_C_FLAGS})
SET (CMAKE_C_FLAGS_RELEASE "-g0 -fPIC -Wall -O3 -Winvalid-pch -ffunction-sections -fdata-sections -ffast-math -DNDEBUG -pthread")
SET (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
 
SET (CMAKE_VERBOSE_MAKEFILE 0)

#Boost
find_package (Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include (FindPkgConfig)
pkg_check_modules (GTK "gtk+-3.0")
INCLUDE_DIRECTORIES (${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
     
AUX_SOURCE_DIRECTORY (../src SOURCES)

# Local includes (inside project).
INCLUDE_DIRECTORIES (../src)
add_executable (${PROJECT_NAME} ${SOURCES})
TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${GTK_LIBRARIES})
TARGET_LINK_LIBRARIES (${PROJECT_NAME} boost_program_options)
