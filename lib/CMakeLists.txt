cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

add_subdirectory(argtable3)
add_subdirectory(mpc)

set(INCLUDE ${INCLUDE} PARENT_SCOPE)
set(SOURCES ${SOURCES} PARENT_SCOPE)
