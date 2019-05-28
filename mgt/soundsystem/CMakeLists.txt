project(mgt-sound)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Build the sources list
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

if(WIN32)
	file(GLOB PROJECT_PLATFORM_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/win/*.cpp
	)
else()
	file(GLOB PROJECT_PLATFORM_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/linux/*.cpp
	)
endif()

# Build as shared library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES} ${PROJECT_PLATFORM_SOURCES})

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "sound" # soundsystem?
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-interface mgt-qlibc mgt-mathlib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm)
endif()