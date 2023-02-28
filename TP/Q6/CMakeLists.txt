cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Q6)

# Source files

set(SRC
    Q6.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(Q6 ${SRC})
