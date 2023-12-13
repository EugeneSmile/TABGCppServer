cmake_minimum_required(VERSION 3.5.0)
project(TABGCppServer)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/tools/cmake)
include(SubDirList)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/bin)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -rdynamic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

option(ENABLE_GUI "Enable GUI" ON)

add_subdirectory(libs)
add_subdirectory(src)
