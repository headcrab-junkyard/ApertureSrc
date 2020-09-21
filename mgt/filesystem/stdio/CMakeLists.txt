project(ogs-filesystem-stdio)

# Build as a shared library
add_library(${PROJECT_NAME} MODULE)

# Sources list
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

# Setup properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "filesystem_stdio"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-tier1 ogs-qcommon)