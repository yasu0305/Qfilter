cmake_minimum_required(VERSION 3.19)
project(hello LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# デバッグ情報を有効化
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0" CACHE STRING "Debug flags")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(hello
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    analogclock.cpp
    analogclock.h
    rolexclock.cpp
    rolexclock.h
    grandfatherclock.cpp
    grandfatherclock.h
    gshockdigital.cpp
    gshockdigital.h
    othello.cpp
    othello.h
)

target_link_libraries(hello PRIVATE Qt6::Widgets)
