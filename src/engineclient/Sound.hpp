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

#pragma once

#include <memory>

#include "CommonTypes.hpp"
#include "soundsystem/ISoundSystem.hpp"

interface ISoundSystem;

class CCDAudio;

class CSound
{
public:
	void Init();
	void Shutdown();
	
	void Update();
	
	void SetListenerPos(const idVec3 &avOrigin, const idVec3 &avPosX, const idVec3 &avPosY, const idVec3 &avPosZ);
private:
	void LoadSoundSystemModule();
	void UnloadSoundSystemModule();
private:
	std::unique_ptr<CCDAudio> mpCDAudio;
	
	void *mpSoundSystemModule{nullptr};
	
	ISoundSystem *mpSoundSystem{nullptr};
};

class CSoundSystemNull : public ISoundSystem
{
public:
	bool Init(CreateInterfaceFn afnEngineFactory, void *apWindow) override;
	void Shutdown() override;

	void Update(float afTimeStep) override;
};