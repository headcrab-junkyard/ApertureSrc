/// @file

#pragma once

#include "sound/ISoundWorld.hpp"

/*
===================================================================================

CSoundWorld

===================================================================================
*/

class CSoundWorld : public ISoundWorld
{
public:
							CSoundWorld();
	virtual					~CSoundWorld();

	//------------------------
	// Functions from idSoundWorld, implemented in SoundWorld.cpp
	//------------------------

	// Called at map start
	virtual void			ClearAllSoundEmitters();

	// stop all playing sounds
	virtual void			StopAllSounds();

	// get a new emitter that can play sounds in this world
	virtual ISoundEmitter *AllocSoundEmitter();

	// for load games
	virtual ISoundEmitter *EmitterForIndex( int index );

	// query data from all emitters in the world
	virtual float			CurrentShakeAmplitude();

	// where is the camera
	virtual void			PlaceListener( const idVec3 &origin, const idMat3 &axis, const int listenerId );

	// fade all sounds in the world with a given shader soundClass
	// to is in Db, over is in seconds
	virtual void			FadeSoundClasses( const int soundClass, const float to, const float over );

	// dumps the current state and begins archiving commands
	virtual void			StartWritingDemo( idDemoFile *demo );
	virtual void			StopWritingDemo();

	// read a sound command from a demo file
	virtual void			ProcessDemoCommand( idDemoFile *readDemo );

	// menu sounds
	virtual int				PlayShaderDirectly( const char *name, int channel = -1 );

	virtual void			Skip( int time );

	virtual void			Pause();
	virtual void			UnPause();
	virtual bool			IsPaused() { return isPaused; }

	virtual int				GetSoundTime();

	// avidump
	virtual void			AVIOpen( const char *path, const char *name );
	virtual void			AVIClose();

	// SaveGame Support
	virtual void			WriteToSaveGame( idFile *savefile );
	virtual void			ReadFromSaveGame( idFile *savefile );

	virtual void			SetSlowmoSpeed( float speed );
	virtual void			SetEnviroSuit( bool active );

	//=======================================

	//------------------------
	// Random stuff that's not exposed outside the sound system
	//------------------------
	void			Update();
	void			OnReloadSound( const idDecl *decl );

	CSoundChannel *	AllocSoundChannel();
	void				FreeSoundChannel( CSoundChannel * );

public:
	// even though all these variables are public, nobody outside the sound system includes SoundWorld_local.h
	// so this is equivalent to making it private and friending all the other classes in the sound system

	idSoundFade			volumeFade;						// master volume knob for the entire world
	idSoundFade			soundClassFade[SOUND_MAX_CLASSES];

	IRenderWorld *		renderWorld;	// for debug visualization and light amplitude sampling
	idDemoFile *		writeDemo;		// if not NULL, archive commands here

	float				currentCushionDB;	// channels at or below this level will be faded to 0
	float				shakeAmp;			// last calculated shake amplitude

	listener_t			listener;
	idList<CSoundEmitter *, TAG_AUDIO>	emitters;

	ISoundEmitter *	localSound;			// for PlayShaderDirectly()

	idBlockAlloc<idSoundEmitterLocal, 16>	emitterAllocator;
	idBlockAlloc<idSoundChannel, 16>		channelAllocator;

	idSoundFade				pauseFade;
	int						pausedTime;
	int						accumulatedPauseTime;
	bool					isPaused;

	float					slowmoSpeed;
	bool					enviroSuitActive;

public: 
	struct soundPortalTrace_t {
		int		portalArea;
		const soundPortalTrace_t * prevStack;
	};

	void			ResolveOrigin( const int stackDepth, const soundPortalTrace_t * prevStack, const int soundArea, const float dist, const idVec3 & soundOrigin, idSoundEmitterLocal * def );
};