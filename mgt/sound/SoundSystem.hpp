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
#include "engine/ISystem.hpp"
#include "engine/IMemory.hpp"
#include "qlibc/qlibc.h"
#include "cvardef.h"

#define MAX_CHANNELS 128

typedef struct cvar_s cvar_t;
typedef struct channel_s channel_t;

interface ISystem;
interface ICmdLine;
interface IMemory;
interface IUtils;
interface ICmdRegistry;
interface ICvarRegistry;
interface IFileSystem;
interface ISound;

extern cvar_t loadas8bit;
extern cvar_t volume;

extern ISound *gpSound;
extern ISystem *gpSystem;
extern IMemory *gpMemory;
extern IUtils *gpUtils;
extern IFileSystem *gpFileSystem;
extern ICmdLine *gpCmdLine;

extern void *mainwindow;

typedef struct sfx_s
{
	char name[MAX_QPATH];
	cache_user_t cache;
} sfx_t;

// !!! if this is changed, it much be changed in asm_i386.h too !!!
typedef struct
{
	int length;
	int loopstart;
	int speed;
	int width;
	int stereo;
	byte data[1]; // variable sized
} sfxcache_t;

typedef struct
{
	int rate;
	int width;
	int channels;
	int loopstart;
	int samples;
	int dataofs; // chunk starts this many bytes from file start
} wavinfo_t;

typedef struct
{
	qboolean gamealive;
	qboolean soundalive;
	qboolean splitbuffer;
	int channels;
	int samples;          // mono samples in buffer
	int submission_chunk; // don't mix less than this #
	int samplepos;        // in mono samples
	int samplebits;
	int speed;
	unsigned char *buffer;
} dma_t;

// !!! if this is changed, it much be changed in asm_i386.h too !!!
typedef struct channel_s
{
	sfx_t *sfx;      // sfx number
	int leftvol;     // 0-255 volume
	int rightvol;    // 0-255 volume
	int end;         // end time in global paintsamples
	int pos;         // sample position in sfx
	int looping;     // where to loop, -1 = no looping
	int entnum;      // to allow overriding a specific sound
	int entchannel;  //
	vec3_t origin;   // origin of sound effect
	vec_t dist_mult; // distance multiplier (attenuation/clipK)
	int master_vol;  // 0-255 master volume
} channel_t;

// !!! if this is changed, it much be changed in asm_i386.h too !!!
typedef struct
{
	int left;
	int right;
} portable_samplepair_t;

extern volatile dma_t *shm;
extern volatile dma_t sn;

extern int snd_blocked;

extern int paintedtime;

extern channel_t channels[MAX_CHANNELS];

extern int total_channels;

wavinfo_t GetWavinfo(const char *name, byte *wav, int wavlength);

sfxcache_t *S_LoadSound(sfx_t *s);

class CSound final : public ISound
{
public:
	CSound();
	~CSound();
	
	bool Init(CreateInterfaceFn afnEngineFactory, void *apWindow) override;
	void Shutdown() override;
	
	void Startup() override;
	
	void Update(float *, float *, float *, float *) override;
	void ExtraUpdate() override;
	
	void ClearBuffer() override;
	
	void BeginPrecaching() override;
	void EndPrecaching() override;
	
	sfx_t *PrecacheSound(const char *sample) override;
	
	void TouchSound(const char *sample) override;
	
	void LocalSound(const char *sound) override;
	
	void StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation) override;
	void StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation) override;
	
	void StopSound(int entnum, int entchannel) override;
	void StopAllSounds(bool clear) override;
private:
	void Update_();
	void UpdateAmbientSounds();
	
	void GetSoundtime();
	
	sfx_t *FindName(const char *name);
	
	// spatializes a channel
	void SND_Spatialize(channel_t *ch);
	
	ISystem *mpSystem{nullptr};
	ICmdLine *mpCmdLine{nullptr};
	IMemory *mpMemory{nullptr};
	ICmdRegistry *mpCmdRegistry{nullptr};
	ICvarRegistry *mpCvarRegistry{nullptr};
};