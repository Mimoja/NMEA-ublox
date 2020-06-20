cmake_minimum_required(VERSION 3.10)
project(NMEA C)

set(CMAKE_C_STANDARD 11)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(NMEA nmea.c tests.h helper.h nmea_handler.c nmea_handler.h nmea.h helper.c)