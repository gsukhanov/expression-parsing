cmake_minimum_required(VERSION 3.19)
project(ExprParsing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20 -w")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/Expression)
add_subdirectory(Expression)

add_executable(differentiator main.cpp)
target_link_libraries(differentiator Expression)

add_executable(tests tests.cpp)
target_link_libraries(tests Expression)

install(TARGETS differentiator DESTINATION ~/bin)