include_directories("../../lib")
add_executable(searcher main.cc)

target_link_libraries(searcher common index)

