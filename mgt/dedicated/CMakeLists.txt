project(mgt-dedicated)

add_executable(${PROJECT_NAME})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "mgtds"
)

target_link_libraries(${PROJECT_NAME} ogs-interface)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} -ldl)
endif()