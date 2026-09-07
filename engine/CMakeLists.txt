cmake_minimum_required(VERSION 3.16)
project(mdz_engine CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
add_compile_options(-Wall -Wextra)

add_library(mdz_core
  src/json.cpp
  src/project.cpp
  src/picking.cpp
  src/runner.cpp
  src/decompile.cpp
  src/runtime.cpp
)

add_executable(mdz_probe src/main.cpp)
target_link_libraries(mdz_probe PRIVATE mdz_core)

find_package(PkgConfig)
pkg_check_modules(SDL2 sdl2)
pkg_check_modules(SDL2IMAGE SDL2_image)
find_package(OpenGL)

if(SDL2_FOUND AND SDL2IMAGE_FOUND AND OpenGL_FOUND)
  add_executable(mdz_view src/render_main.cpp src/renderer.cpp)
  target_include_directories(mdz_view PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
  target_link_libraries(mdz_view PRIVATE mdz_core ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} OpenGL::GL)
else()
  message(STATUS "SDL2/SDL2_image/OpenGL not found -- skipping mdz_view")
endif()

add_executable(mdz_dump src/dump_main.cpp)
target_link_libraries(mdz_dump PRIVATE mdz_core)

add_executable(mdz_run src/run_main.cpp)
target_link_libraries(mdz_run PRIVATE mdz_core)

add_executable(picking_tests tests/picking_tests.cpp)
target_link_libraries(picking_tests PRIVATE mdz_core)

enable_testing()
add_test(NAME picking COMMAND picking_tests)
