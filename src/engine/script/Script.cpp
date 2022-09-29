/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2021-2022 BlackPhrase
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
#include "Script.hpp" //"ScriptSystem.hpp"
#include "System.hpp"

#include <script/IScript.hpp>
CScript::CScript(CSystem *apSystem) : mpSystem(apSystem){}

void CScript::Init()
{
	LoadModule();
	
	mpScriptSystem->Init(Sys_GetFactoryThis());
};

void CScript::Shutdown()
{
	if(mpScriptSystem)
	{
		mpScriptSystem->Shutdown();
		mpScriptSystem = nullptr;
	};
	
	UnloadModule();
};

void CScript::LoadModule()
{
	UnloadModule();
	
	mpScriptSystemModule = Sys_LoadModule("scriptsystem");
	
	if(!mpScriptSystemModule)
		mpSystem->Error("");
	
	auto fnScriptSystemFactory{Sys_GetFactory(mpScriptSystemModule)};
	
	if(!fnScriptSystemFactory)
		mpSystem->Error("");
	
	mpScriptSystem = reinterpret_cast<IScriptSystem*>(fnScriptSystemFactory(OGS_SCRIPTSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpScriptSystem)
		mpSystem->Error("");
};

void CScript::UnloadModule()
{
	if(mpScriptSystemModule)
	{
		Sys_UnloadModule(mpScriptSystemModule);
		mpScriptSystemModule = nullptr;
	};
};