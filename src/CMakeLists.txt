# Add the maelstrom-node library
add_library(maelstrom_node STATIC
    message.cpp
    node.cpp
)

# Specify the include directories for the library
target_include_directories(maelstrom_node PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Specify compile options
target_compile_options(maelstrom_node PRIVATE
    -Wall -Wextra -pedantic
)

# Export the library for installation
install(TARGETS maelstrom_node
    EXPORT maelstrom_nodeConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install header files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/maelstrom-node
    DESTINATION include/maelstrom-node
)
