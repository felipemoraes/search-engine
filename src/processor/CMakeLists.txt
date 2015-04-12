include_directories("../../lib")
add_executable(query_processor main.cc)

target_link_libraries(query_processor common index)

