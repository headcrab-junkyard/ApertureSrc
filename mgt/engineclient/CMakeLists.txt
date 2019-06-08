project(mgt-engineclient)

# Build as dynamic library
add_library(${PROJECT_NAME} MODULE)

target_include_directories(${PROJECT_SOURCES}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME}
	PRIVATE GLQUAKE # TODO: remove
)

# Sources list
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/null PROJECT_SOURCES_PLATFORM)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_SOURCES_PLATFORM}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engineclient"
)

# Link with external libraries
target_link_libraries(${PROJECT_NAME} ogs-interface mgt-qlibc mgt-mathlib)