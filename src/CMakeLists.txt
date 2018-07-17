cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(SOURCES ${SOURCES}
        "${CMAKE_CURRENT_SOURCE_DIR}/ast.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/codegen.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/main.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/parser.c" PARENT_SCOPE)
