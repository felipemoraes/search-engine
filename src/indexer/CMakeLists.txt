

include_directories("../../lib")
add_executable(indexer main.cc)

target_link_libraries(indexer RICPlib z htmlcxx common index)

