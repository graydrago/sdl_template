cmake_minimum_required(VERSION 3.5)

project(template)

function(add_flag flag)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
endfunction()

include_directories("./include/glm")
set(SOURCE_EXE ./src/utils.cpp ./src/ShaderProgram.cpp ./src/Geometry.cpp ./src/Model.cpp main.cpp )

# common flags
add_flag("-std=c++11")
add_flag("-Wall")
#add_flag("-v")

# Sorry, but I support linux and emscripten builds only, because I have a
# lack of time for my home projects.
if(EMSCRIPTEN)
  message("Emscripten Build")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  add_flag("-s USE_SDL=2")
  add_flag("-s SIMD=1")
  add_flag("-s ALLOW_MEMORY_GROWTH=1")
  add_flag("--emrun")
  add_flag("--preload-file ../assets@/assets/")
  add_flag("-O2")
else()
  message("Linux Build")

  find_package(GLEW REQUIRED)
  if (GLEW_FOUND)
      include_directories(${GLEW_INCLUDE_DIRS})
      link_libraries(${GLEW_LIBRARIES})
  endif()

  add_custom_target(link_target ALL
      COMMAND ${CMAKE_COMMAND} -E create_symlink
          "${CMAKE_SOURCE_DIR}/assets"
          "${CMAKE_BINARY_DIR}/assets")

endif()


add_executable( ${PROJECT_NAME} ${SOURCE_EXE} )
target_link_libraries( ${PROJECT_NAME} SDL2 GL GLEW )

