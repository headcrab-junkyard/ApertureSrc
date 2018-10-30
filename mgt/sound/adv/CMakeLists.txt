project(mgt-sound-adv)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Build the sources list
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "s_adv" # soundsystem?
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface mgt-qlibc mgt-mathlib)