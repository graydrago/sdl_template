cmake_minimum_required(VERSION 2.8)

project(template)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
else()
    message(FATAL_ERROR "You should use the GLEW library. CMake will exit.")
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -lGLEW" )

set( SOURCE_EXE main.cpp )

add_custom_target(link_target ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_SOURCE_DIR}/assets" "${CMAKE_BINARY_DIR}/assets")

add_executable( ${PROJECT_NAME} ${SOURCE_EXE} )

target_link_libraries( ${PROJECT_NAME} SDL2 GL)

