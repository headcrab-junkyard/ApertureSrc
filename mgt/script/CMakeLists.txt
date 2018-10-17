project(mgt-script)

# Sources list
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as a shared library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "script"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface)