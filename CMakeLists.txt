] cmake_minimum_required(VERSION 3.8)
project(MagentaEngine)

# Version
set(MGT_VERSION_MAJOR 0)
set(MGT_VERSION_MINOR 7)
set(MGT_VERSION_PATCH 0)

# Build number
set(MGT_VERSION_BUILD 90)

#configure_file(
#	${CMAKE_CURRENT_SOURCE_DIR}/VersionInfo.hpp.in
#	${CMAKE_CURRENT_SOURCE_DIR}/mgt/core/VersionInfo.hpp
#)

# Set CMake binaries output path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../bin)


# Define available options
option(MGT_DEDICATED "Build only dedicated server" OFF)
option(MGT_NDK_BUILD_EXAMPLES "Build engine examples" OFF)

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD_EXTENSIONS ON)

# Compilation flags
#-Wall -Wextra -Wpedantic
#-Werror -Wshadow
#-Wstrict-overflow -fno-strict-aliasing
#-Wno-missing-field-initializers

#-march=native
#-msse2 -msse4.2
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2") # -O3 -Os
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
#set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL}")

# Add include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/mgt/common
)

# Detect the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
        FORCE
	)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		None
		Debug
		Release
		RelWithDebInfo
		MinSizeRel
	)
endif()

# Add subdirectories

add_subdirectory(mgt)
add_subdirectory(tests)

if(MGT_NDK_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()