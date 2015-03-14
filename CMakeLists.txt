cmake_minimum_required(VERSION 2.8.4)
project(search_engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(lib)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(test)

