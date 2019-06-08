project(mgt-script)

# Build as a shared library
add_library(${PROJECT_NAME} MODULE)

# Sources list
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "script"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface)