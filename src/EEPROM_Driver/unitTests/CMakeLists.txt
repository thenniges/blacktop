cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g" CACHE STRING "Debug Flags")
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(eepromTests)

enable_language(C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-pthread")

add_subdirectory(googletest)

set(CXX_SOURCES
    eepromTests.cpp
    ../eeprom.c
)

add_executable(eepromTests
    ${CXX_SOURCES})

target_include_directories(eepromTests PRIVATE
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
                          $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>
)

target_link_libraries(eepromTests gmock_main)

target_include_directories(eepromTests
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)