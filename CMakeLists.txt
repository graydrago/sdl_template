cmake_minimum_required(VERSION 3.5)

project(template)

function(add_flag flag)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
endfunction()

include_directories("third_parties/json/src")
set(SOURCE_EXE
  ./src/utils.cpp ./src/ShaderProgram.cpp ./src/Geometry.cpp
  ./src/Model.cpp ./src/Camera.cpp ./src/Mesh.cpp
  ./src/Object.cpp ./src/Game.cpp ./src/PlayMusic.cpp
  ./src/Collider.cpp ./src/Line.cpp main.cpp)

add_executable( ${PROJECT_NAME} ${SOURCE_EXE} )

# common flags
add_flag("-std=c++14")
add_flag("-Wall")
#add_flag("-v")

# Sorry, but I support linux and emscripten builds only, because I have a
# lack of time for my home projects.
if(EMSCRIPTEN)
  include_directories("third_parties/glm")

  message("Emscripten Build")
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
  add_flag("-s USE_ZLIB=1")
  add_flag("-s USE_LIBPNG=1")
  add_flag("-s USE_SDL=2")
  add_flag("-s USE_SDL_IMAGE=2")
  add_flag("-s SDL2_IMAGE_FORMATS=\"['png']\"")
  add_flag("-s USE_WEBGL2=1")
  add_flag("-s FULL_ES2=1")
  #add_flag("-s SIMD=1")
  add_flag("-s WASM=1")
  add_flag("-s ALLOW_MEMORY_GROWTH=1")
  add_flag("-s USE_OGG=1")
  add_flag("-s USE_VORBIS=1")
  add_flag("-s ASSERTIONS=1")
  add_flag("-s ERROR_ON_UNDEFINED_SYMBOLS=1")
  add_flag("-s DISABLE_EXCEPTION_CATCHING=2")
  add_flag("--shell-file ../raw_assets/html/shell.html")
  add_flag("--no-heap-copy")
  add_flag("--emrun")
  add_flag("--preload-file ../assets@/assets/")
  add_flag("-O2")
  target_link_libraries( ${PROJECT_NAME} GL GLEW)
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


  target_link_libraries( ${PROJECT_NAME} SDL2 SDL2_mixer SDL2_image GL GLEW)
endif()

