project(ogs-engine)

add_library(${PROJECT_NAME} MODULE)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

add_subdirectory(system)
add_subdirectory(network)
add_subdirectory(physics)
add_subdirectory(script)
add_subdirectory(game)

target_link_libraries(${PROJECT_NAME} ogs-tier1)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engine"
)