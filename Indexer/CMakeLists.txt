cmake_minimum_required(VERSION 2.8.4)
project(Indexer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Indexer ${SOURCE_FILES})


ADD_SUBDIRECTORY( src )
ADD_SUBDIRECTORY( test )
