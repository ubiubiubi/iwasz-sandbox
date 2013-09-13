cmake_minimum_required(VERSION 2.6)

SET (CMAKE_VERBOSE_MAKEFILE 1) 
SET (UPLOAD_AFTER_BUILD 0) 

# Name of thos project and excecutable file as well.
PROJECT (spitest)

# Definicje per płytka/procesor etc.
include (raspberrypi.cmake)

AUX_SOURCE_DIRECTORY (../src/ APP_SOURCES)
add_executable (${PROJECT_NAME} ${APP_SOURCES})
