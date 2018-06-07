/*
* This file is part of Magenta Engine
*
* Copyright (C) 2018 BlackPhrase
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
#include "render/RenderInterface.hpp"

void *gpRenderLib{nullptr};
IRender *gpRender{nullptr};

bool LoadRenderModule(const char *name)
{
	//if(gpRenderLib)
		//UnloadRenderModule();
	
	gpRenderLib = Sys_LoadModule(name);
	
	if(!gpRenderLib)
		return false;
	
	auto fnRenderFactory{Sys_GetFactory(gpRenderLib)};
	
	if(!fnRenderFactory)
		return false;
	
	gpRender = (IRender*)fnRenderFactory(MGT_RENDER_INTERFACE_VERSION, nullptr);
	
	if(!gpRender)
		return false;
	
	return true;
};

void UnloadRenderModule()
{
	if(gpRenderLib)
	{
		Sys_UnloadModule(gpRenderLib);
		gpRenderLib = nullptr;
	};
};

void VID_Init()
{
	if(!LoadRenderModule())
		gpRender = nullptr; //new CRenderNull();
	
	if(!gpRender->Init(Sys_GetFactoryThis()))
		return;
};

void VID_Shutdown()
{
	if(gpRender)
	{
		gpRender->Shutdown();
		gpRender = nullptr;
	};
	
	UnloadRenderModule();
};