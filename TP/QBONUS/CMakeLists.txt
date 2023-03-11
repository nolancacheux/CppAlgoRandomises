cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(QBONUS)

# Source files

set(SRC
    QBONUS.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(QBONUS ${SRC})
