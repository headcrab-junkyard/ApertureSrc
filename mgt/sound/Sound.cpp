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

#ifdef _WIN32
#include "winquake.h"
#endif

#include "qlibc/qlibc.h"
#include "bspfile.h"
#include "cvardef.h"

#include "Sound.hpp"
#include "engine/ISystem.hpp"
#include "engine/ICmdLine.hpp"
#include "engine/ICmdArgs.hpp"
#include "engine/ICmdRegistry.hpp"
#include "engine/ICvarRegistry.hpp"
#include "engine/IUtils.hpp"
#include "filesystem/IFileSystem.hpp"

ISound *gpSound{nullptr};
ISystem *gpSystem{nullptr};
IMemory *gpMemory{nullptr};
IUtils *gpUtils{nullptr};
IFileSystem *gpFileSystem{nullptr};
ICmdLine *gpCmdLine{nullptr};

void *mainwindow{nullptr};

// =======================================================================
// Internal sound data & structures
// =======================================================================

bool snd_initialized{false};

int sound_started{0};

int snd_blocked{0};

static bool snd_ambient{1};

bool fakedma{false};

int soundtime;   // sample PAIRS
int paintedtime; // sample PAIRS

#define MAX_DYNAMIC_CHANNELS 8

// 0 to MAX_DYNAMIC_CHANNELS-1	= normal entity sounds
// MAX_DYNAMIC_CHANNELS to MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS -1 = water, etc
// MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS to total_channels = static sounds
channel_t channels[MAX_CHANNELS]{};

int total_channels{0};

#define MAX_SFX 512
sfx_t *known_sfx; // hunk allocated [MAX_SFX]
int num_sfx;

sfx_t *ambient_sfx[NUM_AMBIENTS];

vec3_t listener_origin;
vec3_t listener_forward;
vec3_t listener_right;
vec3_t listener_up;

// pointer should go away
volatile dma_t *shm{nullptr};
volatile dma_t sn;

float sound_nominal_clip_dist = 1000.0f;

cvar_t bgmvolume = { "bgmvolume", "1", true };
cvar_t volume = { "volume", "0.7", true };

cvar_t nosound = { "nosound", "0" };
cvar_t precache = { "precache", "1" };
cvar_t loadas8bit = { "loadas8bit", "0" };
cvar_t bgmbuffer = { "bgmbuffer", "4096" };
cvar_t ambient_level = { "ambient_level", "0.3" };
cvar_t ambient_fade = { "ambient_fade", "100" };
cvar_t snd_noextraupdate = { "snd_noextraupdate", "0" };
cvar_t snd_show = { "snd_show", "0" };
cvar_t _snd_mixahead = { "_snd_mixahead", "0.1", true };

void SND_InitScaletable();

void SNDDMA_Submit();

// initializes cycling through a DMA buffer and returns information on it
bool SNDDMA_Init();

// gets the current DMA position
int SNDDMA_GetDMAPos();

// shutdown the DMA xfer.
void SNDDMA_Shutdown();

// picks a channel based on priorities, empty slots, number of channels
channel_t *SND_PickChannel(int entnum, int entchannel);

/*
=================
SND_PickChannel
=================
*/
channel_t *SND_PickChannel(int entnum, int entchannel)
{
	int ch_idx;
	int first_to_die;
	int life_left;

	// Check for replacement sound, or find the best one to replace
	first_to_die = -1;
	life_left = 0x7fffffff;
	for(ch_idx = NUM_AMBIENTS; ch_idx < NUM_AMBIENTS + MAX_DYNAMIC_CHANNELS; ch_idx++)
	{
		if(entchannel != 0 // channel 0 never overrides
		   && channels[ch_idx].entnum == entnum && (channels[ch_idx].entchannel == entchannel || entchannel == -1))
		{ // allways override sound from same entity
			first_to_die = ch_idx;
			break;
		}

		// don't let monster sounds override player sounds
		//if(channels[ch_idx].entnum == cl.viewentity && entnum != cl.viewentity && channels[ch_idx].sfx) // TODO: again...
			//continue;

		if(channels[ch_idx].end - paintedtime < life_left)
		{
			life_left = channels[ch_idx].end - paintedtime;
			first_to_die = ch_idx;
		}
	}

	if(first_to_die == -1)
		return nullptr;

	if(channels[first_to_die].sfx)
		channels[first_to_die].sfx = nullptr;

	return &channels[first_to_die];
}

void S_ClearPrecache()
{
};

void S_PaintChannels(int endtime);

void S_AmbientOff();
void S_AmbientOn();

/*
===============================================================================

console functions

===============================================================================
*/

void S_Play(const ICmdArgs &apArgs)
{
	static int hash = 345;
	int i;
	char name[256];
	sfx_t *sfx;

	i = 1;
	while(i < apArgs.GetCount())
	{
		if(!Q_strrchr(apArgs.GetByIndex(i), '.'))
		{
			Q_strcpy(name, apArgs.GetByIndex(i));
			Q_strcat(name, ".wav");
		}
		else
			Q_strcpy(name, apArgs.GetByIndex(i));
		
		sfx = gpSound->PrecacheSound(name);
		gpSound->StartDynamicSound(hash++, 0, sfx, listener_origin, 1.0, 1.0);
		i++;
	};
};

void S_PlayVol(const ICmdArgs &apArgs)
{
	static int hash = 543;
	int i;
	float vol;
	char name[256];
	sfx_t *sfx;

	i = 1;
	while(i < apArgs.GetCount())
	{
		if(!Q_strrchr(apArgs.GetByIndex(i), '.'))
		{
			Q_strcpy(name, apArgs.GetByIndex(i));
			Q_strcat(name, ".wav");
		}
		else
			Q_strcpy(name, apArgs.GetByIndex(i));
		
		sfx = gpSound->PrecacheSound(name);
		vol = Q_atof(apArgs.GetByIndex(i + 1));
		gpSound->StartDynamicSound(hash++, 0, sfx, listener_origin, vol, 1.0);
		i += 2;
	};
};

void S_StopAllSoundsC(const ICmdArgs &apArgs)
{
	gpSound->StopAllSounds(true);
};

void S_SoundList(const ICmdArgs &apArgs)
{
	int i;
	sfx_t *sfx;
	sfxcache_t *sc;
	int size, total;

	total = 0;
	for(sfx = known_sfx, i = 0; i < num_sfx; i++, sfx++)
	{
		sc = (sfxcache_t*)gpMemory->Cache_Check(&sfx->cache);
		if(!sc)
			continue;
		size = sc->length * sc->width * (sc->stereo + 1);
		total += size;
		if(sc->loopstart >= 0)
			gpSystem->Printf("L");
		else
			gpSystem->Printf(" ");
		gpSystem->Printf("(%2db) %6i : %s\n", sc->width * 8, size, sfx->name);
	};
	gpSystem->Printf("Total resident: %i\n", total);
};

void S_SoundInfo_f(const ICmdArgs &apArgs)
{
	if(!sound_started || !shm)
	{
		gpSystem->Printf("sound system not started\n");
		return;
	};

	gpSystem->Printf("%5d stereo\n", shm->channels - 1);
	gpSystem->Printf("%5d samples\n", shm->samples);
	gpSystem->Printf("%5d samplepos\n", shm->samplepos);
	gpSystem->Printf("%5d samplebits\n", shm->samplebits);
	gpSystem->Printf("%5d submission_chunk\n", shm->submission_chunk);
	gpSystem->Printf("%5d speed\n", shm->speed);
	gpSystem->Printf("0x%x dma buffer\n", shm->buffer);
	gpSystem->Printf("%5d total_channels\n", total_channels);
};

EXPOSE_SINGLE_INTERFACE(CSound, ISound, MGT_SOUND_INTERFACE_VERSION);

CSound::CSound() = default;
CSound::~CSound() = default;

/*
================
S_Init
================
*/
bool CSound::Init(CreateInterfaceFn afnEngineFactory, void *apWindow)
{
	mpSystem = (ISystem*)afnEngineFactory(MGT_SYSTEM_INTERFACE_VERSION, nullptr);
	mpCmdLine = (ICmdLine*)afnEngineFactory(MGT_CMDLINE_INTERFACE_VERSION, nullptr);
	mpMemory = (IMemory*)afnEngineFactory(MGT_MEMORY_INTERFACE_VERSION, nullptr);
	gpUtils = (IUtils*)afnEngineFactory(MGT_UTILS_INTERFACE_VERSION, nullptr);
	mpCmdRegistry = (ICmdRegistry*)afnEngineFactory(MGT_CMDREGISTRY_INTERFACE_VERSION, nullptr);
	mpCvarRegistry = (ICvarRegistry*)afnEngineFactory(MGT_CVARREGISTRY_INTERFACE_VERSION, nullptr);
	gpFileSystem = (IFileSystem*)afnEngineFactory(MGT_FILESYSTEM_INTERFACE_VERSION, nullptr);
	
	if(!mpSystem)
	{
		printf("ISystem query failed!\n");
		return false;
	};
	
	if(!mpCmdLine)
	{
		printf("ICmdLine query failed!\n");
		return false;
	};
	
	if(!mpMemory)
	{
		printf("IMemory query failed!\n");
		return false;
	};
	
	if(!gpUtils)
	{
		printf("IUtils query failed!\n");
		return false;
	};
	
	if(!mpCmdRegistry)
	{
		printf("ICmdRegistry query failed!\n");
		return false;
	};
	
	if(!mpCvarRegistry)
	{
		printf("ICvarRegistry query failed!\n");
		return false;
	};
	
	gpSound = this;
	gpSystem = mpSystem;
	gpMemory = mpMemory;
	gpCmdLine = mpCmdLine;
	
	mainwindow = apWindow;
	
	mpSystem->Printf("\nSound Initialization\n");
	//VOX_Init(); // TODO

	if(mpCmdLine->HasArg("-simsound"))
		fakedma = true;

	mpCmdRegistry->Add("play", S_Play);
	mpCmdRegistry->Add("playvol", S_PlayVol);
	mpCmdRegistry->Add("stopsound", S_StopAllSoundsC);
	mpCmdRegistry->Add("soundlist", S_SoundList);
	mpCmdRegistry->Add("soundinfo", S_SoundInfo_f);

	mpCvarRegistry->Register(&nosound);
	mpCvarRegistry->Register(&volume);
	mpCvarRegistry->Register(&precache);
	mpCvarRegistry->Register(&loadas8bit);
	mpCvarRegistry->Register(&bgmvolume);
	mpCvarRegistry->Register(&bgmbuffer);
	mpCvarRegistry->Register(&ambient_level);
	mpCvarRegistry->Register(&ambient_fade);
	mpCvarRegistry->Register(&snd_noextraupdate);
	mpCvarRegistry->Register(&snd_show);
	mpCvarRegistry->Register(&_snd_mixahead);

	// TODO
	/*
	if(host_parms.memsize < 0x800000)
	{
		Cvar_Set("loadas8bit", "1");
		mpSystem->Printf("loading all sounds as 8bit\n");
	}
	*/

	snd_initialized = true;

	Startup();

	SND_InitScaletable();

	known_sfx = (sfx_t*)mpMemory->Hunk_AllocName(MAX_SFX * sizeof(sfx_t), "sfx_t");
	num_sfx = 0;

	// create a piece of DMA memory

	if(fakedma)
	{
		shm = (volatile dma_t *)mpMemory->Hunk_AllocName(sizeof(*shm), "shm");
		shm->splitbuffer = 0;
		shm->samplebits = 16;
		shm->speed = 22050;
		shm->channels = 2;
		shm->samples = 32768;
		shm->samplepos = 0;
		shm->soundalive = true;
		shm->gamealive = true;
		shm->submission_chunk = 1;
		shm->buffer = (unsigned char*)mpMemory->Hunk_AllocName(1 << 16, "shmbuf");
	};

	mpSystem->Printf("Sound sampling rate: %i\n", shm->speed);

	// provides a tick sound until washed clean

	//	if (shm->buffer)
	//		shm->buffer[4] = shm->buffer[5] = 0x7f;	// force a pop for debugging

	ambient_sfx[AMBIENT_WATER] = PrecacheSound("ambience/water1.wav");
	ambient_sfx[AMBIENT_SKY] = PrecacheSound("ambience/wind2.wav");

	StopAllSounds(true);
	return true;
};

// =======================================================================
// Shutdown sound engine
// =======================================================================
void CSound::Shutdown()
{
	if(!sound_started)
		return;

	if(shm)
		shm->gamealive = 0;

	shm = 0;
	sound_started = 0;

	if(!fakedma)
		SNDDMA_Shutdown();
	
	gpSound = nullptr;
};

/*
================
S_Startup
================
*/
void CSound::Startup()
{
	int rc;

	if(!snd_initialized)
		return;

	if(!fakedma)
	{
		rc = SNDDMA_Init();

		if(!rc)
		{
#ifndef _WIN32
			mpSystem->Printf("S_Startup: SNDDMA_Init failed.\n");
#endif
			sound_started = 0;
			return;
		};
	};

	sound_started = 1;
};

/*
============
S_Update

Called once each time through the main loop
============
*/
//void S_Update(vec3_t origin, vec3_t forward, vec3_t right, vec3_t up)
void CSound::Update(float *origin, float *forward, float *right, float *up)
{
	int i, j;
	int total;
	channel_t *ch;
	channel_t *combine;

	if(!sound_started || (snd_blocked > 0))
		return;

	VectorCopy(origin, listener_origin);
	VectorCopy(forward, listener_forward);
	VectorCopy(right, listener_right);
	VectorCopy(up, listener_up);

	// update general area ambient sound sources
	UpdateAmbientSounds();

	combine = nullptr;

	// update spatialization for static and dynamic sounds
	ch = channels + NUM_AMBIENTS;
	for(i = NUM_AMBIENTS; i < total_channels; i++, ch++)
	{
		if(!ch->sfx)
			continue;
		
		SND_Spatialize(ch); // respatialize channel
		
		if(!ch->leftvol && !ch->rightvol)
			continue;

		// try to combine static sounds with a previous channel of the same
		// sound effect so we don't mix five torches every frame

		if(i >= MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS)
		{
			// see if it can just use the last one
			if(combine && combine->sfx == ch->sfx)
			{
				combine->leftvol += ch->leftvol;
				combine->rightvol += ch->rightvol;
				ch->leftvol = ch->rightvol = 0;
				continue;
			};
			
			// search for one
			combine = channels + MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS;
			for(j = MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS; j < i; j++, combine++)
				if(combine->sfx == ch->sfx)
					break;

			if(j == total_channels)
			{
				combine = nullptr;
			}
			else
			{
				if(combine != ch)
				{
					combine->leftvol += ch->leftvol;
					combine->rightvol += ch->rightvol;
					ch->leftvol = ch->rightvol = 0;
				};
				continue;
			};
		};
	};

	//
	// debugging output
	//
	if(snd_show.value)
	{
		total = 0;
		ch = channels;
		for(i = 0; i < total_channels; i++, ch++)
			if(ch->sfx && (ch->leftvol || ch->rightvol))
			{
				//mpSystem->Printf ("%3i %3i %s\n", ch->leftvol, ch->rightvol, ch->sfx->name);
				total++;
			};

		mpSystem->Printf("----(%i)----\n", total);
	};

	// mix some sound
	Update_();
};

void CSound::ExtraUpdate()
{
#ifdef _WIN32
	//IN_Accumulate(); // TODO
#endif

	if(snd_noextraupdate.value)
		return; // don't pollute timings
	
	Update_();
};

void CSound::ClearBuffer()
{
	int clear;

#ifdef _WIN32
	if(!sound_started || !shm || (!shm->buffer && !pDSBuf))
#else
	if(!sound_started || !shm || !shm->buffer)
#endif
		return;

	if(shm->samplebits == 8)
		clear = 0x80;
	else
		clear = 0;

#ifdef _WIN32
	if(pDSBuf)
	{
		DWORD dwSize;
		DWORD *pData;
		int reps;
		HRESULT hresult;

		reps = 0;

		while((hresult = pDSBuf->Lock(0, gSndBufSize, (void **)&pData, &dwSize, nullptr, nullptr, 0)) != DS_OK)
		{
			if(hresult != DSERR_BUFFERLOST)
			{
				mpSystem->Printf("S_ClearBuffer: DS::Lock Sound Buffer Failed\n");
				Shutdown();
				return;
			}

			if(++reps > 10000)
			{
				mpSystem->Printf("S_ClearBuffer: DS: couldn't restore buffer\n");
				Shutdown();
				return;
			}
		}

		Q_memset(pData, clear, shm->samples * shm->samplebits / 8);

		pDSBuf->Unlock(pData, dwSize, nullptr, 0);
	}
	else
#endif
	{
		Q_memset(shm->buffer, clear, shm->samples * shm->samplebits / 8);
	}
};

// =======================================================================
// Load a sound
// =======================================================================

void CSound::BeginPrecaching()
{
};

void CSound::EndPrecaching()
{
};

/*
==================
S_PrecacheSound

==================
*/
sfx_t *CSound::PrecacheSound(const char *name)
{
	sfx_t *sfx;

	if(!sound_started || nosound.value)
		return nullptr;

	sfx = FindName(name);

	// cache it in
	if(precache.value)
		S_LoadSound(sfx);

	return sfx;
};

/*
==================
S_TouchSound

==================
*/
void CSound::TouchSound(const char *name)
{
	sfx_t *sfx;

	if(!sound_started)
		return;

	sfx = FindName(name);
	mpMemory->Cache_Check(&sfx->cache);
};

// =======================================================================
// Start a sound effect
// =======================================================================

void CSound::LocalSound(const char *sound)
{
	sfx_t *sfx;

	if(nosound.value)
		return;
	
	if(!sound_started)
		return;

	sfx = PrecacheSound(sound);
	if(!sfx)
	{
		mpSystem->Printf("S_LocalSound: can't cache %s\n", sound);
		return;
	};
	
	//StartDynamicSound(cl.viewentity, -1, sfx, vec3_origin, 1, 1); // TODO: also uses viewentity, not a good place for that...
};

/*
=================
S_StaticSound
=================
*/
void CSound::StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation)
{
	channel_t *ss;
	sfxcache_t *sc;

	if(!sfx)
		return;

	if(total_channels == MAX_CHANNELS)
	{
		mpSystem->Printf("total_channels == MAX_CHANNELS\n");
		return;
	}

	ss = &channels[total_channels];
	total_channels++;

	sc = S_LoadSound(sfx);
	if(!sc)
		return;

	if(sc->loopstart == -1)
	{
		mpSystem->Printf("Sound %s not looped\n", sfx->name);
		return;
	}

	ss->sfx = sfx;
	VectorCopy(origin, ss->origin);
	ss->master_vol = vol;
	ss->dist_mult = (attenuation / 64) / sound_nominal_clip_dist;
	ss->end = paintedtime + sc->length;

	SND_Spatialize(ss);
};

void CSound::StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation)
{
	channel_t *target_chan, *check;
	sfxcache_t *sc;
	int vol;
	int ch_idx;
	int skip;

	if(!sound_started)
		return;

	if(!sfx)
		return;

	if(nosound.value)
		return;

	vol = fvol * 255;

	// pick a channel to play on
	target_chan = SND_PickChannel(entnum, entchannel);
	if(!target_chan)
		return;

	// spatialize
	memset(target_chan, 0, sizeof(*target_chan));
	VectorCopy(origin, target_chan->origin);
	target_chan->dist_mult = attenuation / sound_nominal_clip_dist;
	target_chan->master_vol = vol;
	target_chan->entnum = entnum;
	target_chan->entchannel = entchannel;

	SND_Spatialize(target_chan);

	if(!target_chan->leftvol && !target_chan->rightvol)
		return; // not audible at all

	// new channel
	sc = S_LoadSound(sfx);
	if(!sc)
	{
		target_chan->sfx = nullptr;
		return; // couldn't load the sound's data
	}

	target_chan->sfx = sfx;
	target_chan->pos = 0.0;
	target_chan->end = paintedtime + sc->length;

	// if an identical sound has also been started this frame, offset the pos
	// a bit to keep it from just making the first one louder
	check = &channels[NUM_AMBIENTS];
	for(ch_idx = NUM_AMBIENTS; ch_idx < NUM_AMBIENTS + MAX_DYNAMIC_CHANNELS; ch_idx++, check++)
	{
		if(check == target_chan)
			continue;
		if(check->sfx == sfx && !check->pos)
		{
			skip = rand() % (int)(0.1 * shm->speed);
			if(skip >= target_chan->end)
				skip = target_chan->end - 1;
			target_chan->pos += skip;
			target_chan->end -= skip;
			break;
		}
	}
};

void CSound::StopSound(int entnum, int entchannel)
{
	int i;

	for(i = 0; i < MAX_DYNAMIC_CHANNELS; i++)
	{
		if(channels[i].entnum == entnum && channels[i].entchannel == entchannel)
		{
			channels[i].end = 0;
			channels[i].sfx = nullptr;
			return;
		}
	}
};

void CSound::StopAllSounds(bool clear)
{
	int i;

	if(!sound_started)
		return;

	total_channels = MAX_DYNAMIC_CHANNELS + NUM_AMBIENTS; // no statics

	for(i = 0; i < MAX_CHANNELS; i++)
		if(channels[i].sfx)
			channels[i].sfx = nullptr;

	Q_memset(channels, 0, MAX_CHANNELS * sizeof(channel_t));

	if(clear)
		ClearBuffer();
};

void CSound::Update_()
{
	unsigned int endtime;
	int samps;

	if(!sound_started || (snd_blocked > 0))
		return;

	// Updates DMA time
	GetSoundtime();

	// check to make sure that we haven't overshot
	if(paintedtime < soundtime)
	{
		//mpSystem->Printf ("S_Update_ : overflow\n");
		paintedtime = soundtime;
	};

	// mix ahead of current position
	endtime = soundtime + _snd_mixahead.value * shm->speed;
	samps = shm->samples >> (shm->channels - 1);
	if(endtime - soundtime > samps)
		endtime = soundtime + samps;

#ifdef _WIN32
	// if the buffer was lost or stopped, restore it and/or restart it
	{
		DWORD dwStatus;

		if(pDSBuf)
		{
			if(pDSBuf->GetStatus(&dwStatus) != DS_OK)
				mpSystem->Printf("Couldn't get sound buffer status\n");

			if(dwStatus & DSBSTATUS_BUFFERLOST)
				pDSBuf->Restore();

			if(!(dwStatus & DSBSTATUS_PLAYING))
				pDSBuf->Play(0, 0, DSBPLAY_LOOPING);
		}
	}
#endif

	S_PaintChannels(endtime);

	SNDDMA_Submit();
};

/*
===================
S_UpdateAmbientSounds
===================
*/
void CSound::UpdateAmbientSounds()
{
	// TODO: thing thing requires interaction with model geometry interaction, probably should be moved somewhere else?
/*
	mleaf_t *l;
	float vol;
	int ambient_channel;
	channel_t *chan;

	if(!snd_ambient)
		return;

	// calc ambient sound levels
	if(!cl.worldmodel)
		return;

	l = Mod_PointInLeaf(listener_origin, cl.worldmodel);
	if(!l || !ambient_level.value)
	{
		for(ambient_channel = 0; ambient_channel < NUM_AMBIENTS; ambient_channel++)
			channels[ambient_channel].sfx = nullptr;
		return;
	}

	for(ambient_channel = 0; ambient_channel < NUM_AMBIENTS; ambient_channel++)
	{
		chan = &channels[ambient_channel];
		chan->sfx = ambient_sfx[ambient_channel];

		vol = ambient_level.value * l->ambient_sound_level[ambient_channel];
		if(vol < 8)
			vol = 0;

		// don't adjust volume too fast
		if(chan->master_vol < vol)
		{
			chan->master_vol += host_frametime * ambient_fade.value;
			if(chan->master_vol > vol)
				chan->master_vol = vol;
		}
		else if(chan->master_vol > vol)
		{
			chan->master_vol -= host_frametime * ambient_fade.value;
			if(chan->master_vol < vol)
				chan->master_vol = vol;
		}

		chan->leftvol = chan->rightvol = chan->master_vol;
	}
*/
}

void CSound::GetSoundtime()
{
	int samplepos;
	static int buffers;
	static int oldsamplepos;
	int fullsamples;

	fullsamples = shm->samples / shm->channels;

// it is possible to miscount buffers if it has wrapped twice between
// calls to S_Update.  Oh well.
#ifdef __sun__
	soundtime = SNDDMA_GetSamples();
#else
	samplepos = SNDDMA_GetDMAPos();

	if(samplepos < oldsamplepos)
	{
		buffers++; // buffer wrapped

		if(paintedtime > 0x40000000)
		{ // time to chop things off to avoid 32 bit limits
			buffers = 0;
			paintedtime = fullsamples;
			StopAllSounds(true);
		}
	}
	oldsamplepos = samplepos;

	soundtime = buffers * fullsamples + samplepos / shm->channels;
#endif
}

/*
==================
S_FindName

==================
*/
sfx_t *CSound::FindName(const char *name)
{
	int i;
	sfx_t *sfx;

	if(!name)
		mpSystem->Error("S_FindName: NULL\n");

	if(Q_strlen(name) >= MAX_QPATH)
		mpSystem->Error("Sound name too long: %s", name);

	// see if already loaded
	for(i = 0; i < num_sfx; i++)
		if(!Q_strcmp(known_sfx[i].name, name))
		{
			return &known_sfx[i];
		}

	if(num_sfx == MAX_SFX)
		mpSystem->Error("S_FindName: out of sfx_t");

	sfx = &known_sfx[i];
	strcpy(sfx->name, name);

	num_sfx++;

	return sfx;
};

/*
=================
SND_Spatialize
=================
*/
void CSound::SND_Spatialize(channel_t *ch)
{
	float dot;
	float dist;
	float lscale, rscale, scale;
	vec3_t source_vec;
	sfx_t *snd;

	// anything coming from the view entity will always be full volume
	// TODO: should this check be here? what is we don't have any viewentities in our game?
	/*
	if(ch->entnum == cl.viewentity)
	{
		ch->leftvol = ch->master_vol;
		ch->rightvol = ch->master_vol;
		return;
	}
	*/

	// calculate stereo seperation and distance attenuation

	snd = ch->sfx;
	VectorSubtract(ch->origin, listener_origin, source_vec);

	dist = VectorNormalize(source_vec) * ch->dist_mult;

	dot = DotProduct(listener_right, source_vec);

	if(shm->channels == 1)
	{
		rscale = 1.0;
		lscale = 1.0;
	}
	else
	{
		rscale = 1.0 + dot;
		lscale = 1.0 - dot;
	}

	// add in distance effect
	scale = (1.0 - dist) * rscale;
	ch->rightvol = (int)(ch->master_vol * scale);
	if(ch->rightvol < 0)
		ch->rightvol = 0;

	scale = (1.0 - dist) * lscale;
	ch->leftvol = (int)(ch->master_vol * scale);
	if(ch->leftvol < 0)
		ch->leftvol = 0;
};