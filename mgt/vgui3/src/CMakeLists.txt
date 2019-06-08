project(mgt-vgui3)

# Build as shared library
add_library(${PROJECT_NAME} MODULE)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gwork/include
)

target_link_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/gwork/lib
)

# Build a list of sources
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "vgui3"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface)
target_link_libraries(${PROJECT_NAME} GworkOpenGL GworkTest GworkControls Gwork)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} -liconv)