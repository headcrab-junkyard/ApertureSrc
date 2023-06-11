/*
Copyright (C) 2019-2021 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"

#include "Input.hpp"
#include "Keys.hpp"

#include "inputsystem/IInputSystem.hpp"

void CInput::Init()
{
	LoadInputSystemModule();
	
	mpKeys = std::make_unique<CKeys>();
	
	mpKeys->Init();
};

void CInput::Shutdown()
{
	UnloadInputSystemModule();
};

void CInput::Commands()
{
	IN_Commands();
};

void CInput::Move(usercmd_t *cmd)
{
	mpClientGame->IN_Move(cmd);
};

#ifdef _WIN32
void CInput::ClearStates()
{
	IN_ClearStates();
};
#endif

void CInput::LoadInputSystemModule()
{
	UnloadInputSystemModule();
	
	mpInputSystemModule = Sys_LoadModule("inputsystem");
	
	if(!mpInputSystemModule)
		mpSystem->Error("Failed to load the input system module!");
	
	auto fnInputSystemFactory{Sys_GetFactory(mpInputSystemModule)};
	
	if(!fnInputSystemFactory)
		mpSystem->Error("Failed to get the input system module factory!");
	
	mpInputSystem = reinterpret_cast<IInputSystem*>(fnInputSystemFactory(OGS_INPUTSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpInputSystem)
		mpSystem->Error("Failed to get the input system interface!");
};

void CInput::UnloadInputSystemModule()
{
	if(mpInputSystem)
	{
		mpInputSystem->Shutdown();
		mpInputSystem = nullptr;
	};
	
	if(mpInputSystemModule)
	{
		Sys_UnloadModule(mpInputSystemModule);
		mpInputSystemModule = nullptr;
	};
};