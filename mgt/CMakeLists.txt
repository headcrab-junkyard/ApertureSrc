# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Set this dir as a root
set(MGT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}) # TODO: MGT_ROOT?

# Add include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/common
)

# Add subdirectories

add_subdirectory(qlibc)
#add_subdirectory(applib)
add_subdirectory(mathlib)
add_subdirectory(pm_shared)
add_subdirectory(interface)
#add_subdirectory(utils)

#add_subdirectory(optional)

add_subdirectory(filesystem)
add_subdirectory(network)
#add_subdirectory(physics)
add_subdirectory(script)

#add_subdirectory(core)
##add_subdirectory(engine)

#add_subdirectory(game) #game/server

add_subdirectory(dedicated)

# If not building as dedicated server then build client-side modules too
if(NOT MGT_DEDICATED)
	add_subdirectory(gameui)
	#add_subdirectory(graphics)
	add_subdirectory(input)
	#add_subdirectory(render)
	add_subdirectory(sound)
	##add_subdirectory(vgui3)
	#add_subdirectory(voice)
	
	add_subdirectory(engineclient)
	
	add_subdirectory(launcher)
endif()