cmake_minimum_required(VERSION 3.12)
project(rovis-vision)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

file( GLOB VIS_SOURCES vis/*.cpp )
add_library( VIS vis/vis.h ${VIS_SOURCES})
include_directories(vis)

set(DEPENDENCIES ${OpenCV_LIBS} VIS)

file( GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/app app/*.cpp )

foreach( app_source ${APP_SOURCES} )
    string( REPLACE ".cpp" "" app_name ${app_source} )
    add_executable( ${app_name} app/${app_source} )
    target_link_libraries( ${app_name} ${DEPENDENCIES} )
endforeach( app_source ${APP_SOURCES} )