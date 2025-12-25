cmake_minimum_required(VERSION 3.19)
project(hello LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Build a static library for the completer utilities to allow reuse
add_library(filteredcompleter STATIC
    filteredcompleter.cpp
    filterutils.cpp
)
target_link_libraries(filteredcompleter PRIVATE Qt6::Widgets)
target_include_directories(filteredcompleter PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(hello
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(hello PRIVATE Qt6::Widgets filteredcompleter)

# Small unit test for filterutils
add_executable(filterutils_test tests/filterutils_test.cpp)
target_include_directories(filterutils_test PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(filterutils_test PRIVATE Qt6::Core filteredcompleter)
add_custom_target(check-filterutils
    COMMAND filterutils_test
    DEPENDS filterutils_test
)
