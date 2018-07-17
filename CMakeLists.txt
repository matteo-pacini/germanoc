cmake_minimum_required(VERSION 3.10)
project(germanoc)

set(CMAKE_C_STANDARD 99)

########
# LIBS #
########

add_subdirectory(lib)

########
# LLVM #
########

if(APPLE)
    message(STATUS "Assuming LLVM is installed via Homebrew...")
    set(LLVM_DIR "/usr/local/opt/llvm/lib/cmake/llvm/")
endif(APPLE)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

########
# GLIB #
########

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

##########
# TARGET #
##########

add_subdirectory(include)
add_subdirectory(src)

include_directories(${INCLUDE})

add_definitions(-DVERSION="0.1")
add_definitions(-DAUTHORS="Matteo Pacini <m@matteopacini.me>|Andrea Proietto <>")

add_executable(mosconic ${SOURCES})

llvm_map_components_to_libnames(LLVM_LIBS core native bitwriter)

target_link_libraries(mosconic ${LLVM_LIBS} ${GLIB_LDFLAGS} mpc compiler argtable3)
