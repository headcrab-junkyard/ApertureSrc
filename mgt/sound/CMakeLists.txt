project(mgt-sound)

# Build the sources list
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "sound" # soundsystem?
)