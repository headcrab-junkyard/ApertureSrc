project(mgt-networksystem)

# Sources list
file(GLOB MGT_NETWORK_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${MGT_NETWORK_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "networksystem"
)

target_link_libraries(${PROJECT_NAME} ogs-interface)

if(WIN32)
	# NOTE: looks like wsock32 is loaded dynamically
	# ws2_32
	target_link_libraries(${PROJECT_NAME} wsock32 winmm)
endif()