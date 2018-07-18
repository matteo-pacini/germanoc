cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/fold.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/apply.c" PARENT_SCOPE)
