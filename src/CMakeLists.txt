# Add include directories
include_directories(
	${CMAKE_SOURCE_DIR}/mgt/common
	${CMAKE_SOURCE_DIR}/mgt/engine
	${OGS_SDK_PATH}
	${OGS_SDK_PATH}/common
	${OGS_SDK_PATH}/public
	${OGS_SDK_PATH}/next
)

# We want shared libraries (and modules) without any prefix in their names
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Add subdirectories

add_subdirectory(engine)

add_subdirectory(filesystem)

add_subdirectory(networksystem)
add_subdirectory(physics)
add_subdirectory(scriptsystem)

add_subdirectory(optional)

# If not building as dedicated server then build client-side modules too
if(NOT OGS_DEDICATED)
	add_subdirectory(renderer)
	add_subdirectory(inputsystem)
	add_subdirectory(soundsystem)
	add_subdirectory(voice)
	#TODO: add_subdirectory(vgui3)
	
	add_subdirectory(engineclient)
endif()