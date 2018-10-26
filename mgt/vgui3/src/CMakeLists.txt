project(mgt-vgui3)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/gwork/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/gwork/lib)

# Build a list of sources
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "vgui3"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface)
target_link_libraries(${PROJECT_NAME} GworkOpenGL GworkTest GworkControls Gwork)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} opengl32)
else()
	target_link_libraries(${PROJECT_NAME} -lGL)
endif()