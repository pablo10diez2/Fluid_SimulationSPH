cmake_minimum_required(VERSION 3.0.0)
project(engine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(engine
    src/config.h 
    src/main.cpp 
    src/glad.c
    src/gravity.cpp
    src/gravity.h)

target_include_directories(engine PRIVATE dependencies)
target_link_libraries(engine glfw OpenGL::GL)

