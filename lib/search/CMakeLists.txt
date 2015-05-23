# CmakeLists.txt in src dir

# Adds a library called common from the source files
file(GLOB search_SRC
    "*.h"
    "*.cc"
)


ADD_LIBRARY( search ${search_SRC})
target_link_libraries(search common index)



