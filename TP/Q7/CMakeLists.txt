cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Q7)

# Source files

set(SRC
    Q7.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(Q7 ${SRC})
