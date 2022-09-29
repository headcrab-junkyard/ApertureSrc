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
#include "Physics.hpp"
#include "System.hpp"

#include <physics/IPhysics.hpp>

CPhysics::CPhysics(CSystem *apSystem) : mpSystem(apSystem){}

void CPhysics::Init()
{
	LoadModule();
	
	//mpPhysicsSystem->Init(Sys_GetFactoryThis());
};

void CPhysics::Shutdown()
{
	if(mpPhysicsSystem)
	{
		//mpPhysicsSystem->Shutdown();
		mpPhysicsSystem = nullptr;
	};
	
	UnloadModule();
};

void CPhysics::LoadModule()
{
	UnloadModule();
	
	mpPhysicsSystemModule = Sys_LoadModule("physics");
	
	if(!mpPhysicsSystemModule)
		mpSystem->Error("");
	
	auto fnPhysicsSystemFactory{Sys_GetFactory(mpPhysicsSystemModule)};
	
	if(!fnPhysicsSystemFactory)
		mpSystem->Error("");
	
	mpPhysicsSystem = reinterpret_cast<IPhysics*>(fnPhysicsSystemFactory(OGS_PHYSICS_INTERFACE_VERSION, nullptr));
	
	if(!mpPhysicsSystem)
		mpSystem->Error("");
};

void CPhysics::UnloadModule()
{
	if(mpPhysicsSystemModule)
	{
		Sys_UnloadModule(mpPhysicsSystemModule);
		mpPhysicsSystemModule = nullptr;
	};
};