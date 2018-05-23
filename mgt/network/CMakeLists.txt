project(mgt-network)

# Sources list
file(GLOB MGT_NETWORK_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${MGT_NETWORK_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "network"
)