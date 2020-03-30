/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2018, 2020 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief general module wide definitions

// Note: this file should be placed in precompiled header

#pragma once

#include <string>
#include <list>
#include <vector>
#include <set>
#include <chrono>

#include "const.h"
//#include "math/MathTypes.hpp"

#ifdef _WIN32
	#ifdef MODULE_EXPORT
		#define MODULE_API [[dllexport]]
	#else
		#define MODULE_API [[dllimport]]
	#endif
#elif __linux__
	#ifdef MODULE_EXPORT
		#define MODULE_API [[visibility("default")]]
	#else
		#define MODULE_API
	#endif
#else
	#error "Unsupported compiler!"
#endif

#define C_EXPORT extern "C" MODULE_API

// This can be used to declare an abstract class (interface only)
// Interfaces should not be instantiated
// If they are, an access violation will occur
#define interface struct

#ifdef UNICODE
	#define _T(x) L##x
#else
	#define _T(x) x  
#endif

using namespace std::chrono_literals;

using uint = unsigned int;
using dword = unsigned long;

using tString = std::string;
using tStringList = std::list<tString>;
using tStringVec = std::vector<tString>;
using tStringSet = std::set<tString>;

using tWString = std::wstring;
using tWStringList = std::list<tWString>;
using tWStringVec = std::vector<tWString>;
using tWStringSet = std::set<tWString>;

using tHighResClock = std::chrono::high_resolution_clock;

#ifndef BIT
constexpr auto BIT(int anBit)
{
	return 1 << anBit;
};
#endif

//constexpr auto MAX_INFO_STRING{256}; ///< Infostrings are transmitted across network
constexpr auto MAX_SYSPATH{1024}; ///< System filepath