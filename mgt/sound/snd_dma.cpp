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

#include "vox.h"

vec_t sound_nominal_clip_dist = 1000.0f;

int desired_speed = 11025;
int desired_bits = 16;

// ====================================================================
// User-setable variables
// ====================================================================

//
// Fake dma is a synchronous faking of the DMA progress used for
// isolating performance in the renderer.  The fakedma_updates is
// number of times S_Update() is called per second.
//

int fakedma_updates = 15;

void S_AmbientOff()
{
	snd_ambient = false;
}

void S_AmbientOn()
{
	snd_ambient = true;
}

//=============================================================================

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
		if(channels[ch_idx].entnum == cl.viewentity && entnum != cl.viewentity && channels[ch_idx].sfx)
			continue;

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

//=============================================================================

void S_ClearPrecache()
{
};