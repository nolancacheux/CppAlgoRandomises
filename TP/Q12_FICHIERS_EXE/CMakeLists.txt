cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Q12_FICHIERS_EXE)

# Source files

set(SRC
    Q12_file.cpp
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

add_executable(Q12_FICHIERS_EXE ${SRC})
