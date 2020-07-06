#project(mgt-voice)

# Set default impl type
#set(MGT_VOICE_DEFAULT_IMPL Null)

# Detect the impl type
#if(NOT MGT_VOICE_IMPL_TYPE)
#    set(MGT_VOICE_IMPL_TYPE Default CACHE STRING
#        "Choose the type of voice module implementation, options are: Null Opus."
#        FORCE
#	)
#    set_property(CACHE MGT_VOICE_IMPL_TYPE PROPERTY STRINGS
#		Default
#		Opus
#	)
#endif()

#if(${MGT_VOICE_IMPL_TYPE} STREQUAL Default)
#	set(MGT_VOICE_IMPL_TYPE ${MGT_VOICE_DEFAULT_IMPL})
#endif()

# Add impl-specific sources
#string(TOLOWER ${MGT_VOICE_IMPL_TYPE} MGT_VOICE_IMPL_TYPE)
#add_subdirectory(${MGT_VOICE_IMPL_TYPE})

add_subdirectory(opus)