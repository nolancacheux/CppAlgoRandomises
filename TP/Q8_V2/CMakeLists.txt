cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

project(Q8_V2)

# Source files

set(SRC
    Q8_V2.cpp
    
)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)


add_executable(Q8_V2 ${SRC})
