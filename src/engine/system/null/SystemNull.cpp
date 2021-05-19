/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2021 BlackPhrase
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
/// @brief null platform system code

#include "quakedef.h"
#include "SystemNull.hpp"

void CSystemNull::Printf(const char *fmt, ...)
{
	va_list argptr;
	
	va_start(argptr, fmt);
	vprintf(fmt, argptr);
	va_end(argptr);
};

void CSystemNull::Error(const char *error, ...)
{
	va_list argptr;

	printf("Sys_Error: ");
	va_start(argptr, error);
	vprintf(error, argptr);
	va_end(argptr);
	printf("\n");

	exit(1);
};

/*
================
Sys_FloatTime
================
*/
double CSystemNull::GetFloatTime()
{
	static double t;

	t += 0.1;

	return t;
};

void CSystem::Sleep(int anMilliSecs)
{
	// NOTHING
};