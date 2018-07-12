cmake_minimum_required(VERSION 3.10)
project(germanoc C)

set(CMAKE_C_STANDARD 99)

if(APPLE)
    message(STATUS "Assuming LLVM is installed via Homebrew...")
    set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm/")
endif(APPLE)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_definitions(-DVERSION="0.1")
add_definitions(-DAUTHORS="Matteo Pacini <m@matteopacini.me>|Andrea Proietto <>")

add_executable(germanoc mpc.h mpc.c main.c parser.c parser.h str_utils.c str_utils.h codegen.c codegen.h)

llvm_map_components_to_libnames(llvm_libs core executionengine mcjit interpreter analysis native bitwriter)

target_link_libraries(germanoc ${llvm_libs} c++)
