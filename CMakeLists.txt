cmake_minimum_required(VERSION 3.16)
project(Antimatter VERSION 1.0.0 LANGUAGES CXX)

find_package(geode-sdk REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

geode_add_mod(Antimatter
    SOURCES
        src/main.cpp
    INCLUDE_DIRS
        src/
    RESOURCES
        resources/blackhole.png
)

geode_target_output(Antimatter release "${CMAKE_CURRENT_SOURCE_DIR}/mod.json")