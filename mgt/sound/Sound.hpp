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

#pragma once

#include "sound/ISound.hpp"

typedef struct channel_s channel_t;

interface ISystem;
interface IMemory;
interface ICmdRegistry;
interface ICvarRegistry;

class CSound final : public ISound
{
public:
	CSound();
	~CSound();
	
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	void Update(float *, float *, float *, float *) override;
	void ExtraUpdate() override;
	
	void ClearBuffer() override;
	
	void BeginPrecaching() override;
	void EndPrecaching() override;
	
	sfx_t *PrecacheSound(const char *sample) override;
	
	void TouchSound(const char *sample) override;
	
	void StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation) override;
	void StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation) override;
	
	void StopSound(int entnum, int entchannel) override;
	void StopAllSounds(bool clear) override;
private:
	void Startup();
	
	void Update_();
	void UpdateAmbientSounds();
	
	void GetSoundtime();
	
	sfx_t *FindName(const char *name);
	
	// spatializes a channel
	void SND_Spatialize(channel_t *ch);
	
	ISystem *mpSystem{nullptr};
	IMemory *mpMemory{nullptr};
	ICmdRegistry *mpCmdRegistry{nullptr};
	ICvarRegistry *mpCvarRegistry{nullptr};
};