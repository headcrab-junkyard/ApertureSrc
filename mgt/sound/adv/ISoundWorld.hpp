/// @file

#pragma once

/*
===============================================================================

	SOUND WORLD

There can be multiple independent sound worlds, just as there can be multiple
independent render worlds.  The prime example is the editor sound preview
option existing simultaniously with a live game.
===============================================================================
*/

struct ISoundWorld
{
	//virtual					~ISoundWorld() = default;

	// call at each map start
	virtual void			ClearAllSoundEmitters() = 0;
	virtual void			StopAllSounds() = 0;

	// get a new emitter that can play sounds in this world
	virtual ISoundEmitter *AllocSoundEmitter() = 0;

	// for load games, index 0 will return NULL
	virtual ISoundEmitter *EmitterForIndex( int index ) = 0;

	// query sound samples from all emitters reaching a given listener
	virtual float			CurrentShakeAmplitude() = 0;

	// where is the camera/microphone
	// listenerId allows listener-private and antiPrivate sounds to be filtered
	virtual void			PlaceListener( const idVec3 &origin, const idMat3 &axis, const int listenerId ) = 0;

	// fade all sounds in the world with a given shader soundClass
	// to is in Db, over is in seconds
	virtual void			FadeSoundClasses( const int soundClass, const float to, const float over ) = 0;

	// menu sounds
	virtual	int				PlayShaderDirectly( const char * name, int channel = -1 ) = 0;

	// dumps the current state and begins archiving commands
	virtual void			StartWritingDemo( idDemoFile *demo ) = 0;
	virtual void			StopWritingDemo() = 0;

	// read a sound command from a demo file
	virtual void			ProcessDemoCommand( idDemoFile *demo ) = 0;

	// when cinematics are skipped, we need to advance sound time this much
	virtual void			Skip( int time ) = 0;

	// pause and unpause the sound world
	virtual void			Pause() = 0;
	virtual void			UnPause() = 0;
	virtual bool			IsPaused() = 0;

	// Write the sound output to multiple wav files.  Note that this does not use the
	// work done by AsyncUpdate, it mixes explicitly in the foreground every PlaceOrigin(),
	// under the assumption that we are rendering out screenshots and the gameTime is going
	// much slower than real time.
	// path should not include an extension, and the generated filenames will be:
	// <path>_left.raw, <path>_right.raw, or <path>_51left.raw, <path>_51right.raw, 
	// <path>_51center.raw, <path>_51lfe.raw, <path>_51backleft.raw, <path>_51backright.raw, 
	// If only two channel mixing is enabled, the left and right .raw files will also be
	// combined into a stereo .wav file.
	virtual void			AVIOpen( const char *path, const char *name ) = 0;
	virtual void			AVIClose() = 0;

	// SaveGame / demo Support
	virtual void			WriteToSaveGame( IFile *savefile ) = 0;
	virtual void			ReadFromSaveGame( IFile *savefile ) = 0;

	virtual void			SetSlowmoSpeed( float speed ) = 0;
	virtual void			SetEnviroSuit( bool active ) = 0;
};