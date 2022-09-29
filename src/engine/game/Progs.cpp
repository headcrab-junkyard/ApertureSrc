/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020, 2022 BlackPhrase
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

#include "quakedef.h"
#include "Progs.hpp"

void PR_Profile_f()
{
};

void CProgs::Init()
{
	PR_Init();
};

int CProgs::SetString(const char *s)
{
	return 0; //return PR_SetString(s);
};

char *CProgs::GetString(int num)
{
	return PR_GetString(num);
};