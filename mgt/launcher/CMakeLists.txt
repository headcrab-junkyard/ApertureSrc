project(ogs-launcher)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(WIN32)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/win PROJECT_PLATFORM_SOURCES)
elseif(UNIX)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/linux PROJECT_PLATFORM_SOURCES)
else()
	message(FATAL_ERROR "Unsupported platform!")
endif()

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_PLATFORM_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "launcher"
)

target_link_libraries(${PROJECT_NAME} ogs-tier1)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} -ldl)
endif()