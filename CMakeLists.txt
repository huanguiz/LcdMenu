cmake_minimum_required(VERSION 3.22)
project(LcdMenu)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LcdMenu
        func.cpp
        menu.cpp
        menu.h
        tinystr.cpp
        tinystr.h
        tinyxml.cpp
        tinyxml.h
        tinyxmlerror.cpp
        tinyxmlparser.cpp)
