cmake_minimum_required(VERSION 4.1)
project(Project_3)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Project_3
        Board.cpp
        Board.h
        Digits.cpp
        Digits.h
        main.cpp
        RandomGenerator.cpp
        RandomGenerator.h
        ResourcePath.hpp
        ResourcePath.mm
        TextureManager.cpp
        TextureManager.h
        Tile.cpp
        Tile.h)
