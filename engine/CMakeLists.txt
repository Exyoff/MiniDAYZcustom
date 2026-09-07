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
)

add_executable(mdz_probe src/main.cpp)
target_link_libraries(mdz_probe PRIVATE mdz_core)

add_executable(picking_tests tests/picking_tests.cpp)
target_link_libraries(picking_tests PRIVATE mdz_core)

enable_testing()
add_test(NAME picking COMMAND picking_tests)
