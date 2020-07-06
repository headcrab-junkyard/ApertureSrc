add_subdirectory(engine)
add_subdirectory(engineclient)

add_subdirectory(filesystem)

add_subdirectory(networksystem)
add_subdirectory(physics)
add_subdirectory(scriptsystem)

if(NOT OGS_DEDICATED)
	add_subdirectory(renderer)
	add_subdirectory(inputsystem)
	add_subdirectory(soundsystem)
	add_subdirectory(voice)
endif()