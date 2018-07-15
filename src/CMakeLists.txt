cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

set(SOURCES ${SOURCES} src/ast.c src/codegen.c src/main.c src/parser.c PARENT_SCOPE)
