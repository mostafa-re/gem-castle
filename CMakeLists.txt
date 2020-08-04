cmake_minimum_required(VERSION 3.10)

project(gem_castle)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED true)

file(GLOB SOURCE "src/*.cpp")
set(COMPILE_FLAGS "-Wall -Wextra -g -pthread")

include_directories(include)
set(CMAKE_CXX_FLAGS ${COMPILE_FLAGS})
add_executable(${PROJECT_NAME}.app ${SOURCE})
target_link_libraries(${PROJECT_NAME}.app m sfml-system sfml-window sfml-graphics)