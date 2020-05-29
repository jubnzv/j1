cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(j1 CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "output/bin/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "output/lib/")

# ##############################################################################
# Configure build
# ##############################################################################
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

message("")
message(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
message("")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DEBUG_J1 1)
else()
  set(DEBUG_J1 0)
endif()

# Use colorized terminal output
set(TERM_COLORS 1)

configure_file(src/lib/Config.hpp.in src/lib/Config.hpp @ONLY)

# ##############################################################################
# Build executables
# ##############################################################################
# VM
add_executable(j1vm src/vm/main.cpp src/vm/VM.cpp src/vm/J1.cpp
                    src/vm/Stack.cpp src/vm/IOConsole.cpp)
set_property(TARGET j1vm PROPERTY CXX_STANDARD 20)

# Compiler
add_executable(j1c src/compiler/main.cpp)
set_property(TARGET j1c PROPERTY CXX_STANDARD 20)

# Common headers
include_directories(${PROJECT_SOURCE_DIR}/src/lib/)

# ##############################################################################
# Tests
# ##############################################################################
# GTest: Suppress CMake policy warning (CMP0048)
if(POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif(POLICY CMP0048)

include(cmake/googletest.cmake)
fetch_googletest(${CMAKE_SOURCE_DIR}/cmake
                 ${CMAKE_SOURCE_DIR}/third-party/googletest)
enable_testing()
add_subdirectory(tests)
