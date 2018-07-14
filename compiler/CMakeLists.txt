cmake_minimum_required(VERSION 3.10)
project(germanoc C)

set(CMAKE_C_STANDARD 99)

##########
# TARGET #
##########

add_library(compiler STATIC compiler.c)
