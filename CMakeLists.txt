cmake_minimum_required(VERSION 3.10)

project(Tuple
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads)

# Auto download googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        075196ca060107d44b4e3a1f44b25886ed5bd187 # v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(testTuple
  testTuple.cc
)

target_include_directories(testTuple
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)

target_compile_options(testTuple
  PRIVATE
  "-Wall" "-Wextra" "-g" "-O0" "-fsanitize=address,undefined"
)

set_target_properties(testTuple
  PROPERTIES
    LINK_FLAGS "-fsanitize=address,undefined"
)

target_link_libraries(testTuple
  PRIVATE
    GTest::gtest_main
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(testTuple)
