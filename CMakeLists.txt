#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(SLAM LANGUAGES CXX)

#Require C++20 to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#Find Catch2 package
find_package(Catch2 REQUIRED)

#Include Catch header
include(Catch)

#Globally include 'include' directory
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

#Add executable target
add_executable(token_test ${CMAKE_CURRENT_SOURCE_DIR}/test/token_test.cpp ${CMAKE_CURRENT_SOURCE_DIR}/inc/token.hpp)

