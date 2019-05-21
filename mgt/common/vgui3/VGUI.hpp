/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
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

#pragma once

#define null 0L

#pragma warning(disable:4800)
#pragma warning(disable:4786)
#pragma warning(disable:4355)
#pragma warning(disable:4097)
#pragma warning(disable:4514)
#pragma warning(disable:4100)
#pragma warning(disable:4127)

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#ifndef _WCHAR_T_DEFINED
#if !defined(__x86_64__) && !defined(__WCHAR_TYPE__ )
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif
#endif

namespace vgui3
{

// handle to an internal vgui panel
// this is the only handle to a panel that is valid across dll boundaries
typedef unsigned int VPANEL;

// handles to vgui objects
// NULL values signify an invalid value
typedef unsigned long HScheme;
typedef unsigned long HTexture;
typedef unsigned long HCursor;
typedef unsigned long HPanel;
const HPanel INVALID_PANEL = 0xffffffff;
typedef unsigned long HFont;
const HFont INVALID_FONT = 0;

} // namespace vgui3

/*


namespace vgui2
{




typedef int HContext;



// the value of an invalid font handle
const VPANEL NULL_PANEL = 0;
const HFont INVALID_FONT = 0;
const HPanel INVALID_PANEL = 0xffffffff;

} // namespace vgui

*/