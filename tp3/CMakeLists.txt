cmake_minimum_required(VERSION 3.13.5)

project(tp3 LANGUAGES CXX)

add_executable(pandemic)
add_executable(pandemic_test)

set_property(TARGET pandemic PROPERTY CXX_STANDARD 17)
set_property(TARGET pandemic_test PROPERTY CXX_STANDARD 17)

target_link_libraries(pandemic -ltbb)
target_link_libraries(pandemic_test -ltbb)

set(CATCH_HEADER "https://raw.githubusercontent.com/catchorg/Catch2/master/single_include/catch2/catch.hpp")
file(DOWNLOAD ${CATCH_HEADER} ${CMAKE_CURRENT_BINARY_DIR}/include/catch.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)
add_subdirectory(source)
add_subdirectory(tests)
