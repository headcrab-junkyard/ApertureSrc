project(mgt-engine)

#enable_language(ASM)

add_definitions(-DGLQUAKE) # TODO

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PROJECT_SOURCES
	#${CMAKE_CURRENT_SOURCE_DIR}/*.S
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/null/*.cpp
)

add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} ogs-interface mgt-qlibc mgt-mathlib)

if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm) # timeGetTime
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engine"
)