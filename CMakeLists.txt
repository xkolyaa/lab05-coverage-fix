cmake_minimum_required(VERSION 3.4)

project(lab05)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=maybe-uninitialized")

add_subdirectory(formatter_lib)
add_subdirectory(formatter_ex_lib)
add_subdirectory(solver_lib)
add_subdirectory(hello_world_application)
add_subdirectory(solver_application)

option(BUILD_TESTS "Создать тесты" OFF)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(third-party/gtest)
  
  add_library(banking STATIC banking/Account.cpp banking/Transaction.cpp)
  target_include_directories(banking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/banking)
  target_compile_options(banking PRIVATE --coverage)
  target_link_libraries(banking gcov)

  file(GLOB SOLVER_TEST_SOURCES tests/*.cpp)
  add_executable(check ${SOLVER_TEST_SOURCES})
  target_link_libraries(check banking solver_lib gtest_main gmock_main gcov)
  add_test(NAME check COMMAND check)
endif()
