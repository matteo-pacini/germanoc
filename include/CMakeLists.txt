cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(SOURCES ${SOURCES} include/ast.h include/codegen.h include/parser.h PARENT_SCOPE)
