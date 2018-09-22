cmake_minimum_required(VERSION 3.12)
project(Vision_mini_project)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
set(DEPENDENCIES ${OpenCV_LIBS})

add_executable(Vision_mini_project main.cpp)

target_link_libraries (Vision_mini_project  ${DEPENDENCIES})