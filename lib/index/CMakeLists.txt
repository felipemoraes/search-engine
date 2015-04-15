# CmakeLists.txt in src dir

# Adds a library called common from the source files
file(GLOB index_SRC
    "*.h"
    "*.cc"
)


ADD_LIBRARY( index ${index_SRC})
target_link_libraries(index common gumbo)



