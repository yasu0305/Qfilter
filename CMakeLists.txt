cmake_minimum_required(VERSION 3.19)
project(hello LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(hello
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(hello PRIVATE Qt6::Widgets)
