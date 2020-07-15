cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_COMPILER   "gcc-8")
    set(CMAKE_CXX_COMPILER "g++-8")
endif()

project(hobf)

set(PROJECT_NAME "hobf")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(HOBF_LIBRARIES boost_program_options)

include(CheckEnvironment)

set(OPT_LEVEL "-O3")

if(NOOPT)
    set(OPT_LEVEL "-O0")
endif()

add_compile_options(
        -std=c++17
        ${OPT_LEVEL}
        -g3
        -Wall
        -Werror
        -Wextra
        -Wconversion
        -Woverloaded-virtual
)

# ---- Libraries global settings------------------------------------------------
include_directories(
        ${Boost_INCLUDE_DIRS}
)

add_subdirectory(src)
