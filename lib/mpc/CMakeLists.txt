cmake_minimum_required(VERSION 3.10)
project(germanoc C)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

add_library(mpc STATIC mpc.c)

set(INCLUDE ${INCLUDE}
        "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)

set(SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/mpc.c" PARENT_SCOPE)
