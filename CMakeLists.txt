cmake_minimum_required(VERSION 3.19)
project(qfilter LANGUAGES C CXX)

# Ensure GNUInstallDirs is included after a language is enabled
include(GNUInstallDirs)

# Export compile_commands.json to help IntelliSense include resolution
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure source and execution charsets use UTF-8 where supported
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
elseif(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)

# Build a small static library for the completer utilities
add_library(filteredcompleter STATIC
    filteredcompleter.cpp
    filteredcompleter.h
)
target_link_libraries(filteredcompleter PUBLIC Qt6::Widgets Qt6::Core Qt6::Gui)
target_include_directories(filteredcompleter PUBLIC ${CMAKE_SOURCE_DIR})
set_target_properties(filteredcompleter PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)


add_executable(qfilter
    main.cpp
    mainwindow.cpp
    mainwindow.h
)
target_link_libraries(qfilter PRIVATE Qt6::Widgets filteredcompleter)
set_target_properties(qfilter PROPERTIES AUTOUIC OFF)
