cmake_minimum_required(VERSION 3.19)
project(hello LANGUAGES C CXX)

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

find_package(Qt6 REQUIRED COMPONENTS Widgets Core)

# Build a small static library for the completer utilities
add_library(filteredcompleter STATIC
        filteredcompleter.cpp
        filterutils.cpp
        filteredcompleter.h
        filterutils.h
)
target_link_libraries(filteredcompleter PUBLIC Qt6::Widgets Qt6::Core)
target_include_directories(filteredcompleter PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(hello
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

target_link_libraries(hello PRIVATE Qt6::Widgets filteredcompleter)

# This project does not use Qt Designer .ui files anymore; disable AutoUic for the
# main executable to avoid requiring nonexistent ui_*.h files.
set_target_properties(hello PROPERTIES AUTOUIC OFF)

# Optional small test
if(EXISTS "${CMAKE_SOURCE_DIR}/tests/filterutils_test.cpp")
    add_executable(filterutils_test tests/filterutils_test.cpp)
    target_include_directories(filterutils_test PRIVATE ${CMAKE_SOURCE_DIR})
    target_link_libraries(filterutils_test PRIVATE Qt6::Core filteredcompleter)
    add_custom_target(check-filterutils
            COMMAND filterutils_test
            DEPENDS filterutils_test
    )
endif()
