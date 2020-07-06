/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020 BlackPhrase
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

#include "quakedef.h"
#include "Video.hpp"

void CVideo::Init(unsigned char *palette)
{
	VID_Init(palette);
	LoadRendererModule("r_gl");
	
	if(!mpRenderer->Init(Sys_GetFactoryThis()))
		mpSystem->Error("Couldn't initialize the renderer module!");
};

void CVideo::Shutdown()
{
	UnloadRendererModule();
};

void CVideo::CheckChanges()
{
	VID_CheckChanges();
};

void CVideo::LoadRendererModule(const char *asName)
{
	UnloadRendererModule();
	
	mpRendererModule = Sys_LoadModule(asName);
	
	if(!mpRendererModule)
		mpSystem->Error("Couldn't load the renderer module (%s)!", asName);
	
	auto fnRendererFactory{Sys_GetFactory(mpRendererModule)};
	
	if(!fnRendererFactory)
		mpSystem->Error("The loaded renderer module (%s) doesn't seem to be a valid module!", asName);
	
	mpRenderer = reinterpret_cast<IRenderer*>(fnRendererFactory(OGS_RENDERER_INTERFACE_VERSION, nullptr));
	
	if(!mpRenderer)
		mpSystem->Error("The loaded renderer module (%s) doesn't seem to have the required interface implemented!", asName);
};

void CVideo::UnloadRendererModule()
{
	if(mpRenderer)
	{
		mpRenderer->Shutdown();
		mpRenderer = nullptr;
	};
	
	if(mpRendererModule)
	{
		Sys_UnloadModule(mpRendererModule);
		mpRendererModule = nullptr;
	};
};