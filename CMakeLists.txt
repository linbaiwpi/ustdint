cmake_minimum_required(VERSION 3.14)
project(libint128)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Add compiler flags for debug and release
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Option to enable string visualization helpers in int128_t
option(VISUALIZE_VALUE "Enable to_dec_string()/to_hex_string() helpers" OFF)
message(STATUS "VISUALIZE_VALUE option: ${VISUALIZE_VALUE}")
message(STATUS "Tip: toggle with -DVISUALIZE_VALUE=ON/OFF, or use ccmake/cmake-gui for interactive selection")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  test_int128
  test_int128.cpp
)

target_link_libraries(
  test_int128
  GTest::gtest_main
)

if (VISUALIZE_VALUE)
  target_compile_definitions(test_int128 PRIVATE VISUALIZE_VALUE)
  # 若需要所有目标启用，可改为 PUBLIC 或使用 add_compile_definitions(VISUALIZE_VALUE)
endif()

include(GoogleTest)
gtest_discover_tests(test_int128)

message(STATUS "====================================== HINT ======================================")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "configure with -DCMAKE_BUILD_TYPE=Release for optimized build")
endif()
if (VISUALIZE_VALUE)
  message(STATUS "configure with -DVISUALIZE_VALUE=OFF for NOT visuliazing values in string")
else()
  message(STATUS "configure with -DVISUALIZE_VALUE=ON for visuliazing values in string")
endif()
message(STATUS "==================================================================================")
