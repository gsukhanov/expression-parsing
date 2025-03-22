cmake_minimum_required(VERSION 3.19)
project(ExprParsing)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -w")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SGAExpression)
add_subdirectory(SGAExpression)

add_executable(differentiator main.cpp)
target_link_libraries(differentiator SGAExpression)

add_executable(tests tests.cpp)
target_link_libraries(tests SGAExpression)

install(TARGETS differentiator DESTINATION ~/bin)
