cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(INCLUDE ${INCLUDE} "${CMAKE_CURRENT_SOURCE_DIR}" PARENT_SCOPE)
set(SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/codegen.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/macros.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/parser.h" PARENT_SCOPE)
