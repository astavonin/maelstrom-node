cmake_minimum_required(VERSION 3.20)
project(maelstrom_node VERSION 1.0.0 LANGUAGES CXX)

# Enable C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set vcpkg toolchain file
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# Add the maelstrom-node library
add_library(maelstrom_node STATIC
    src/message.cpp
    src/node.cpp
)

# Include Boost headers
find_package(Boost REQUIRED NO_MODULE)  # Boost header-only dependencies like algorithm
target_include_directories(maelstrom_node PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# Include nlohmann_json and spdlog
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

# Link libraries (header-only libraries don’t require linking)
target_link_libraries(maelstrom_node PUBLIC
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    Boost::headers  # General Boost headers (for compatibility)
)

# Install targets
install(TARGETS maelstrom_node
    EXPORT maelstrom_nodeConfig
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/maelstrom-node
    DESTINATION include/maelstrom-node
)
