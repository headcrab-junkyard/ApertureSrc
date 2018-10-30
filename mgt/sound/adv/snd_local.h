/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

#pragma once

#include "WaveFile.h"

// Maximum number of voices we can have allocated
#define MAX_HARDWARE_VOICES 48

// A single voice can play multiple channels (up to 5.1, but most commonly stereo)
// This is the maximum number of channels which can play simultaneously
// This is limited primarily by seeking on the optical drive, secondarily by memory consumption, and tertiarily by CPU time spent mixing
#define MAX_HARDWARE_CHANNELS 64

// We may need up to 3 buffers for each hardware voice if they are all long sounds
#define MAX_SOUND_BUFFERS (MAX_HARDWARE_VOICES * 3)

// Maximum number of channels in a sound sample
#define MAX_CHANNELS_PER_VOICE 8

/*
========================
MsecToSamples
SamplesToMsec
========================
*/
ID_INLINE_EXTERN uint32 MsecToSamples(uint32 msec, uint32 sampleRate)
{
	return (msec * (sampleRate / 100)) / 10;
}
ID_INLINE_EXTERN uint32 SamplesToMsec(uint32 samples, uint32 sampleRate)
{
	return sampleRate < 100 ? 0 : (samples * 10) / (sampleRate / 100);
}

/*
========================
DBtoLinear
LinearToDB
========================
*/
ID_INLINE_EXTERN float DBtoLinear(float db)
{
	return idMath::Pow(2.0f, db * (1.0f / 6.0f));
}
ID_INLINE_EXTERN float LinearToDB(float linear)
{
	return (linear > 0.0f) ? (idMath::Log(linear) * (6.0f / 0.693147181f)) : -999.0f;
}

// demo sound commands
typedef enum {
	SCMD_STATE, // followed by a load game state
	SCMD_PLACE_LISTENER,
	SCMD_ALLOC_EMITTER,

	SCMD_FREE,
	SCMD_UPDATE,
	SCMD_START,
	SCMD_MODIFY,
	SCMD_STOP,
	SCMD_FADE
} soundDemoCommand_t;

#include "SoundVoice.h"

#define OPERATION_SET 1

#include <dxsdkver.h>

#include <xaudio2.h>
#include <xaudio2fx.h>
#include <X3DAudio.h>
#include <xma2defs.h>
#include "XAudio2/XA2_SoundSample.h"
#include "XAudio2/XA2_SoundVoice.h"
#include "XAudio2/XA2_SoundHardware.h"

//------------------------
// Listener data
//------------------------
struct listener_t
{
	idMat3 axis; // orientation of the listener
	idVec3 pos;  // position in meters
	int id;      // the entity number, used to detect when a sound is local
	int area;    // area number the listener is in
};