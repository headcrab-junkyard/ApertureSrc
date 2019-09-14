/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

#include "quakedef.h"

//qboolean pr_trace; // TODO: unused?

//int pr_argc; // TODO: unused?

//=============================================================================

char *pr_strtbl[MAX_PRSTR]{};
int num_prstr{0};

char *PR_GetString(int num)
{
	if(num < 0)
	{
		//Con_DPrintf("GET:%d == %s\n", num, pr_strtbl[-num]);
		return pr_strtbl[-num];
	};
	return pr_strings + num;
};

int PR_SetString(char *s)
{
	if(s - pr_strings < 0)
	{
		for(int i = 0; i <= num_prstr; i++)
			if(pr_strtbl[i] == s)
				break;
		if(i < num_prstr)
			return -i;
		if(num_prstr == MAX_PRSTR - 1)
			gpSystem->Error("MAX_PRSTR");
		num_prstr++;
		pr_strtbl[num_prstr] = s;
		//Con_DPrintf("SET:%d == %s\n", -num_prstr, s);
		return -num_prstr;
	};
	return (int)(s - pr_strings);
};