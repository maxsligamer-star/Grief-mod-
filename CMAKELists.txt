cmake_minimum_required(VERSION 3.23)

project(grief-gd VERSION 1.0.0)

find_package(Geode REQUIRED)

add_geode_mod(${PROJECT_NAME} SHARED
    src/main.cpp
)
