cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Q10)

# Source files

set(SRC
    Q10.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(Q10 ${SRC})
