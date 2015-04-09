# Adds a library called common from the source files
file(GLOB common_SRC
    "*.h"
    "*.cc"
    "*.hxx"
)


ADD_LIBRARY( common ${common_SRC})
target_link_libraries(common htmlcxx)
