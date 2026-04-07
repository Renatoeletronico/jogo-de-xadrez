cmake_minimum_required(VERSION 3.20)
project(xadrez)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(xadrez 
    main.cpp
    board.cpp
)

target_link_libraries(xadrez PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)