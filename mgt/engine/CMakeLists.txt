project(mgt-engine)

add_definitions(-DGLQUAKE) # TODO

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/null/*.cpp
)

add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} ogs-interface)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engine"
)