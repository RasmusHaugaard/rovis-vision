cmake_minimum_required(VERSION 3.12)
project(RoVi_final)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)
set(DEPENDENCIES ${OpenCV_LIBS})


add_executable(RoVi_final main.cpp)

target_link_libraries (RoVi_final ${DEPENDENCIES})