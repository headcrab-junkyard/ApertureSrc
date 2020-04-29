/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2017-2018, 2020 BlackPhrase
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
#include "null/InputNull.hpp"

void *gpInputLib{ nullptr };
IInput *gpInput{ nullptr };

bool LoadInputModule()
{
	//if(gpInputLib)
		//return false;

	gpInputLib = Sys_LoadModule("input");

	if(!gpInputLib)
		return false;

	auto fnInputFactory{ Sys_GetFactory(gpInputLib) };

	if(!fnInputFactory)
		return false;

	gpInput = (IInput *)fnInputFactory(MGT_INPUT_INTERFACE_VERSION, nullptr);

	if(!gpInput)
		return false;

	return true;
};

void UnloadInputModule()
{
	if(gpInputLib)
	{
		Sys_UnloadModule(gpInputLib);
		gpInputLib = nullptr;
	};
};

void IN_Init()
{
	if(!LoadInputModule())
		gpInput = new CInputNull();
	
	if(!gpInput->Init())
		return;
};

void IN_Shutdown()
{
	if(gpInput)
	{
		gpInput->Shutdown();
		gpInput = nullptr;
	};
	
	UnloadInputModule();
};

//void CInputNull::Commands()
void IN_Commands()
{
	// TODO
};

/*
//void CInputNull::Move(usercmd_t *cmd)
void IN_Move(usercmd_t *cmd)
{
};
*/

// TODO: IN_ClearStates?

// TODO: unused?
/*
===========
IN_ModeChanged
===========
*/
/*
void IN_ModeChanged()
{
}
*/