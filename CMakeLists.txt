cmake_minimum_required(VERSION 2.8.4)
project(Search_Engine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

ADD_SUBDIRECTORY( Indexer )
ADD_SUBDIRECTORY( Util )

