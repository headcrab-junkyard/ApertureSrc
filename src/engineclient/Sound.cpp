/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019-2022 BlackPhrase
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

#include "Sound.hpp"
#include "CDAudio.hpp"

#include "soundsystem/ISoundSystem.hpp"

void CSound::Init()
{
	mpCDAudio = std::make_unique<CCDAudio>();
	
	LoadSoundSystemModule();
	
	mpSoundSystem->Init();
	
	mpCDAudio->Init();
};

void CSound::Shutdown()
{
	mpCDAudio->Shutdown();
	
	UnloadSoundSystemModule();
};

void CSound::Update()
{
	mpSoundSystem->Update();
	
	mpCDAudio->Update();
};

void CSound::SetListenerPos(const idVec3 &avOrigin, const idVec3 &avPosX, const idVec3 &avPosY, const idVec3 &avPosZ)
{
	// TODO
};

void CSound::LoadSoundSystemModule()
{
	UnloadSoundSystemModule();
	
	mpSoundSystemModule = Sys_LoadModule("soundsystem");
	
	if(!mpSoundSystemModule)
		mpSystem->Error("Failed to load the sound system module!");
	
	auto fnSoundSystemFactory{Sys_GetFactory(mpSoundSystemModule)};
	
	if(!fnSoundSystemFactory)
		mpSystem->Error("Failed to get the sound system module factory!");
	
	mpSoundSystem = reinterpret_cast<ISoundSystem*>(fnSoundSystemFactory(OGS_SOUNDSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpSoundSystem)
		mpSystem->Error("Failed to get the sound system interface!");
};

void CSound::UnloadSoundSystemModule()
{
	if(mpSoundSystem)
	{
		mpSoundSystem->Shutdown();
		mpSoundSystem = nullptr;
	};
	
	if(mpSoundSystemModule)
	{
		Sys_UnloadModule(mpSoundSystemModule);
		mpSoundSystemModule = nullptr;
	};
};