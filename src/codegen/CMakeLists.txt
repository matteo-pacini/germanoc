cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/var_expr.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/var_decl.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/print.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/read.c" PARENT_SCOPE)
