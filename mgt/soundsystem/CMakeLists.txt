project(mgt-soundsystem)

# Build as shared library
add_library(${PROJECT_NAME} MODULE)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

# Build the sources list
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(WIN32)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/win PROJECT_PLATFORM_SOURCES)
else()
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/linux PROJECT_PLATFORM_SOURCES)
endif()

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_PLATFORM_SOURCES}
)

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "soundsystem"
)

# Link with dependencies
target_link_libraries(${PROJECT_NAME} ogs-tier1 mgt-qlibc mgt-mathlib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm)
endif()