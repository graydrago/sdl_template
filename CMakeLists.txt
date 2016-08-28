cmake_minimum_required(VERSION 2.8)

project(template)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall" )

set( SOURCE_EXE main.cpp )

add_executable( ${PROJECT_NAME} ${SOURCE_EXE} )

target_link_libraries( ${PROJECT_NAME} SDL2 GL)

