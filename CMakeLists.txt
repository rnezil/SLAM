#Specify minimum cmake version requirement
CMAKE_MINIMUM_REQUIRED(VERSION 3.1 FATAL_ERROR)

#Specify project name and languages
project(SLAM LANGUAGES CXX)

#Require C++20 to be used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

#Create some variables for future convenience
set(SLAM_include ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SLAM_source ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(SLAM_test ${CMAKE_CURRENT_SOURCE_DIR}/src/test)

#Include code sanitizers
include(${SLAM_include}/Sanitizers.cmake)

#Globally include 'include' directory
include_directories(${SLAM_include})

#Add executable target
add_executable(SLAM ${SLAM_source}/main.cpp)
add_executable(token_test ${SLAM_test}/token_test.cpp)
add_executable(function_lexing ${SLAM_test}/function_lexing.cpp)
add_executable(parse_test ${SLAM_test}/parse_test.cpp)

#Add AST library target
add_library(AST ${SLAM_source}/ast.cpp)

#Link executables with AST library
target_link_libraries(SLAM AST)
target_link_libraries(function_lexing AST)
target_link_libraries(parse_test AST)

#Install the SLAM program
install(TARGETS SLAM DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

#Install the SLAM library
install(TARGETS AST DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

#Install the demo script
#install(PROGRAMS demo DESTINATION ${CMAKE_INSTALL_PREFIX}bin)

