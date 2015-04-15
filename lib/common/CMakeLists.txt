# Adds a library called common from the source files
file(GLOB common_SRC
    "*.h"
    "*.cc"
)


ADD_LIBRARY( common ${common_SRC})
target_link_libraries(common gumbo)
