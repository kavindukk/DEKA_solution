
cmake_minimum_required(VERSION 3.10)

project(Deka)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED 1)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${PROJECT_NAME}
    main.cpp
    helper.h
)