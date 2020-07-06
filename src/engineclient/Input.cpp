/*
Copyright (C) 2019-2020 BlackPhrase

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

#include "quakedef.h"
#include "Input.hpp"

void CInput::Init()
{
	LoadInputSystemModule();
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
	IN_Move(cmd);
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
	
	mpInputSystem = reinterpret_cast<IInputSystem*>(fnInputSystemFactory(OGS_INPUTSYSTEM_INTERFACE_VERSION, nullptr));
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