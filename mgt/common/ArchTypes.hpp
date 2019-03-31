/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief architecture types

#pragma once

enum class PlatformType : int
{
	Unknown = 0
	Windows,
	Linux,
	MacOSX,
	Android
};

// TODO: refactor this!

#if (_WIN32 || WIN32)
	#define MGT_PLATFORM MGT_PLATFORM_WINDOWS
	constexpr PlatformType MGT_PLATFORM = PlatformType::Windows;
	
	//#if (_WIN64 || WIN64)
		//#define MGT_PLATFORM MGT_PLATFORM_WINDOWS64
	//#endif
#elif __unix__
	#define MGT_PLATFORM MGT_PLATFORM_POSIX // TODO
#elif __linux__
	#define MGT_PLATFORM MGT_PLATFORM_LINUX
	constexpr PlatformType MGT_PLATFORM = PlatformType::Linux;
#elif __APPLE__
	#define MGT_PLATFORM MGT_PLATFORM_MACOSX
	constexpr PlatformType MGT_PLATFORM = PlatformType::MacOSX;
#endif

#ifdef I_X86
	#define MGT_PLATFORM_X32
#elif I_X64
	#define MGT_PLATFORM_X64
#endif