include_directories(
	${CMAKE_SOURCE_DIR}/mgt/common
	${OGS_SDK_PATH}/common
	${OGS_SDK_PATH}/public
	${OGS_SDK_PATH}/next
)

add_subdirectory(engine)

add_subdirectory(filesystem)

add_subdirectory(networksystem)
add_subdirectory(physics)
add_subdirectory(scriptsystem)

if(NOT OGS_DEDICATED)
	add_subdirectory(renderer)
	add_subdirectory(inputsystem)
	add_subdirectory(soundsystem)
	add_subdirectory(voice)
	
	add_subdirectory(engineclient)
endif()