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

/// @file

#pragma once

#include "ISoundSystem.hpp"

/*
===================================================================================

CSoundSystem

===================================================================================
*/
class CSoundSystem : public ISoundSystem
{
public:
	CSoundSystem() :
		soundTime( 0 ), currentSoundWorld( NULL ),
		muted( false ), musicMuted( false ), needsRestart( false )
	{}
		
	// all non-hardware initialization
	virtual void			Init();

	// shutdown routine
	virtual	void			Shutdown();

	virtual ISoundWorld *	AllocSoundWorld( IRenderWorld *rw );
	virtual void			FreeSoundWorld( ISoundWorld *sw );

	// specifying NULL will cause silence to be played
	virtual void			SetPlayingSoundWorld( ISoundWorld *soundWorld );

	// some tools, like the sound dialog, may be used in both the game and the editor
	// This can return NULL, so check!
	virtual ISoundWorld *	GetPlayingSoundWorld();

	// sends the current playing sound world information to the sound hardware
	virtual void			Render();

	// Mutes the SSG_MUSIC group
	virtual void			MuteBackgroundMusic( bool mute ) { musicMuted = mute; }

	// sets the final output volume to 0
	// This should only be used when the app is deactivated
	// Since otherwise there will be problems with different subsystems muting and unmuting at different times
	virtual void			SetMute( bool mute ) { muted = mute; }
	virtual bool			IsMuted() { return muted; }

	virtual void			OnReloadSound( const idDecl * sound );

	virtual void			StopAllSounds();

	virtual void			InitStreamBuffers();
	virtual void			FreeStreamBuffers();

	virtual void *			GetIXAudio2() const;

	// for the sound level meter window
	virtual cinData_t		ImageForTime( const int milliseconds, const bool waveform );

	// Free all sounds loaded during the last map load
	virtual	void			BeginLevelLoad();

	// We might want to defer the loading of new sounds to this point
	virtual	void			EndLevelLoad();

	// prints memory info
	virtual void			PrintMemInfo( MemInfo_t *mi );

	//-------------------------

	// Before a sound is reloaded, any active voices using it must
	// be stopped.  Returns true if any were playing, and should be
	// restarted after the sound is reloaded.
	void					StopVoicesWithSample( const idSoundSample * const sample );

	void					Restart();
	void					SetNeedsRestart() { needsRestart = true; }

	int						SoundTime() const;

	// may return NULL if there are no more voices left
	idSoundVoice *			AllocateVoice( const idSoundSample * leadinSample, const idSoundSample * loopingSample );
	void					FreeVoice( idSoundVoice * );

	idSoundSample *			LoadSample( const char * name );

	virtual void			Preload( idPreloadManifest & preload );

	struct bufferContext_t {
		bufferContext_t() :
			voice( NULL ),
			sample( NULL ),
			bufferNumber( 0 )
		{ }
		idSoundVoice_XAudio2 *	voice;
		idSoundSample_XAudio2 * sample;
		int bufferNumber;
	};

	// Get a stream buffer from the free pool, returns NULL if none are available
	bufferContext_t *			ObtainStreamBufferContext();
	void						ReleaseStreamBufferContext( bufferContext_t * p );

	idSysMutex					streamBufferMutex;
	idStaticList< bufferContext_t *, MAX_SOUND_BUFFERS > freeStreamBufferContexts;
	idStaticList< bufferContext_t *, MAX_SOUND_BUFFERS > activeStreamBufferContexts;
	idStaticList< bufferContext_t, MAX_SOUND_BUFFERS > bufferContexts;

	idSoundWorldLocal *			currentSoundWorld;
	idStaticList<idSoundWorldLocal *, 32>	soundWorlds;

	idList<idSoundSample *, TAG_AUDIO>		samples;
	idHashIndex					sampleHash;

	idSoundHardware				hardware;

	idRandom2					random;
	
	int							soundTime;
	bool						muted;
	bool						musicMuted;
	bool						needsRestart;

	bool						insideLevelLoad;
};

extern	CSoundSystem	soundSystemLocal;