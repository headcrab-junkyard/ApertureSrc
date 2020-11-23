/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020 BlackPhrase
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

#include "Engine.hpp"
#include "Host.hpp"
#include "engineclient/IEngineClient.hpp"

EXPOSE_SINGLE_INTERFACE(CEngine, IEngine, OGS_ENGINE_INTERFACE_VERSION);

bool CEngine::Init(const InitParams &apInitParams)
{
	IEngineClient *pEngineClient{LoadClientModule()};
	
	return mpHost->Init(pEngineClient);
};

void CEngine::Shutdown()
{
	mpHost->Shutdown();
	
	UnloadClientModule();
};

bool CEngine::Frame()
{
	return mpHost->Frame();
};

IEngineClient *CEngine::LoadClientModule()
{
	mpClientModule = Sys_LoadModule("engineclient");
	
	if(!mpClientModule)
		return nullptr;
	
	auto pfnClientModuleFactory{Sys_GetFactory(mpClientModule)};
	
	if(!pfnClientModuleFactory)
		return nullptr;
	
	auto pEngineClient{reinterpret_cast<IEngineClient*>(pfnClientModuleFactory(OGS_ENGINECLIENT_INTERFACE_VERSION, nullptr))};
	return pEngineClient;
};

void CEngine::UnloadClientModule()
{
	Sys_UnloadModule(mpClientModule);
};