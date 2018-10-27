/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief sound module interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_SOUND_INTERFACE_VERSION{"MGTSound001Alpha"};

struct ISoundWorld;

typedef struct sfx_s sfx_t;

interface ISound : public IBaseInterface
{
	/// All non-hardware initialization
	virtual bool Init(CreateInterfaceFn afnEngineFactory, void *apWindow) = 0; // TODO
	
	/// Shutdown routine
	virtual void Shutdown() = 0;
	
	///
	virtual void Startup() = 0;
	
	///
	virtual void Update(float*, float*, float*, float*) = 0;
	//virtual void Update(float afTimeStep) = 0;
	
	///
	virtual void ExtraUpdate() = 0;
	
	///
	virtual void ClearBuffer() = 0;
	
	///
	virtual void BeginPrecaching() = 0;
	
	///
	virtual void EndPrecaching() = 0;
	
	///
	virtual sfx_t *PrecacheSound(const char *sample) = 0;
	
	///
	virtual void TouchSound(const char *sample) = 0;
	
	///
	virtual void LocalSound(const char *sound) = 0;
	
	///
	virtual void StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation) = 0;
	
	///
	virtual void StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation) = 0;
	
	///
	virtual void StopSound(int entnum, int entchannel) = 0;
	
	///
	virtual void StopAllSounds(bool clear) = 0;
	
	///
	//virtual void MuteBackgroundMusic(bool abMute) = 0;
	
	/// Sets the final output volume to 0
	/// This should only be used when the app is deactivated
	/// Since otherwise there will be problems with different subsystems muting and unmuting at different times
	//virtual void SetMute(bool abMute) = 0;
	
	///
	//virtual bool IsMuted() const = 0;
	
	/// The renderWorld is used for visualization and light amplitude sampling
	//virtual ISoundWorld *AllocWorld(/*IRenderWorld *apRenderWorld*/) = 0;
	
	///
	//virtual void FreeWorld(ISoundWorld *apWorld) = 0;

	/// Specifying nullptr will cause silence to be played
	//virtual void SetWorld(ISoundWorld *apWorld) = 0;

	/// Some tools, like the sound dialog, may be used in both the game and the editor
	/// This can return nullptr, so check!
	//virtual ISoundWorld *GetCurrentWorld() const = 0;
};