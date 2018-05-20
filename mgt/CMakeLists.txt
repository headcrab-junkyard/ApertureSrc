# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Set this dir as a root
set(MGT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # TODO: MGT_ROOT?

# Add include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/common
	#${CMAKE_CURRENT_SOURCE_DIR}/public
	${CMAKE_SOURCE_DIR}/../ogs-sdk/ogs
	${CMAKE_SOURCE_DIR}/../ogs-sdk/ogs/common
	${CMAKE_SOURCE_DIR}/../ogs-sdk/ogs/engine
	${CMAKE_SOURCE_DIR}/../ogs-sdk/ogs/public
)

add_subdirectory(${CMAKE_SOURCE_DIR}/../ogs-sdk ${CMAKE_BINARY_DIR}/ogs-sdk)

# Add subdirectories
#add_subdirectory(optional)
#add_subdirectory(core)
#add_subdirectory(filesystem)
#add_subdirectory(network)
#add_subdirectory(physics)
add_subdirectory(engine)
#add_subdirectory(game)

#add_subdirectory(application)
add_subdirectory(dedicated)

# If not building as dedicated server then build these modules too
if(NOT MGT_DEDICATED)
	#add_subdirectory(engineclient)
	add_subdirectory(gameui)
	#add_subdirectory(graphics)
	#add_subdirectory(input)
	#add_subdirectory(render)
	#add_subdirectory(sound)
	#add_subdirectory(voice)
	add_subdirectory(launcher)
endif()