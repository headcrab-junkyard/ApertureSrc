/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
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
/// @brief main control for any streaming sound output device

#include "quakedef.h"
#include "null/SoundNull.hpp"

cvar_t bgmvolume = { "bgmvolume", "1", true };
cvar_t volume = { "volume", "0.7", true };

void *gpSoundLib{nullptr};
ISound *gpSound{nullptr};

bool LoadSoundModule()
{
	gpSoundLib = Sys_LoadModule("sound");

	if(!gpSoundLib)
		return false;

	auto pfnSoundFactory{ Sys_GetFactory(gpSoundLib) };

	if(!pfnSoundFactory)
		return false;

	gpSound = (ISound *)pfnSoundFactory(MGT_SOUND_INTERFACE_VERSION, nullptr);

	if(!gpSound)
		return false;

	if(gpSound)
		if(!gpSound->Init(Sys_GetFactoryThis(), nullptr))
			return false;

	return true;
};

void UnloadSoundModule()
{
	if(gpSound)
		gpSound->Shutdown();

	if(gpSoundLib)
	{
		Sys_UnloadModule(gpSoundLib);
		gpSoundLib = nullptr;
	}
	//else
		//delete gpSound;

	gpSound = nullptr;
};

/*
================
S_Init
================
*/
void S_Init()
{
	Con_Printf("\nSound Initialization\n");
	//VOX_Init(); // TODO

	if(COM_CheckParm("-nosound"))
	{
		gpSound = new CSoundNull();
		return;
	};

	if(!LoadSoundModule())
		return;
};

// =======================================================================
// Shutdown sound engine
// =======================================================================
void S_Shutdown()
{
	UnloadSoundModule();
};

void S_Update(vec3_t origin, vec3_t v_forward, vec3_t v_right, vec3_t v_up)
{
};

/*
void S_AmbientOff()
{
};

void S_AmbientOn()
{
};
*/

void S_TouchSound(const char *sample)
{
};

void S_ClearBuffer()
{
};

void S_StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation)
{
};

void S_StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation)
{
};

void S_StopSound(int entnum, int entchannel)
{
};

sfx_t *S_PrecacheSound(const char *sample)
{
	return nullptr;
};

/*
void S_ClearPrecache()
{
};
*/

void S_StopAllSounds(bool clear)
{
};

void S_BeginPrecaching()
{
};

void S_EndPrecaching()
{
};

/*
void S_ExtraUpdate()
{
};
*/

void S_LocalSound(const char *s)
{
};