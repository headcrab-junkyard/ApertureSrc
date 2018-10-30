/// @file

#pragma once

/*
===============================================================================

	SOUND SYSTEM

===============================================================================
*/

struct soundDecoderInfo_t
{
	CStr name{""};
	CStr format{""};
	
	int numChannels{0};
	int numSamplesPerSecond{0};
	int num44kHzSamples{0};
	int numBytes{0};
	
	bool looping{false};
	
	float lastVolume{0.0f};
	
	int start44kHzTime{0};
	int current44kHzTime{0};
};

struct ISoundSystem
{
	// The renderWorld is used for visualization and light amplitude sampling.
	virtual ISoundWorld *	AllocSoundWorld( IRenderWorld *rw ) = 0;
	virtual void			FreeSoundWorld( ISoundWorld *sw ) = 0;

	// Specifying NULL will cause silence to be played.
	virtual void			SetPlayingSoundWorld( ISoundWorld *soundWorld ) = 0;

	// Some tools, like the sound dialog, may be used in both the game and the editor
	// This can return NULL, so check!
	virtual ISoundWorld *	GetPlayingSoundWorld() = 0;
	
	/// Sends the current playing sound world information to the sound hardware
	virtual void Render() = 0;

	virtual void			MuteBackgroundMusic( bool mute ) = 0;

	// Sets the final output volume to 0.
	virtual void			SetMute( bool mute ) = 0;
	virtual bool			IsMuted() = 0;
	
	/// Called by the decl system when a sound decl is reloaded
	virtual void OnReloadSound(const idDecl *sound) = 0;

	/// May be called to free memory for level loads
	virtual void InitStreamBuffers() = 0;
	virtual void FreeStreamBuffers() = 0;

	/// video playback needs to get this
	virtual void *GetIXAudio2() const = 0;

	/// for the sound level meter window
	virtual cinData_t ImageForTime(const int milliseconds, const bool waveform) = 0;

	/// Free all sounds loaded during the last map load
	virtual void BeginLevelLoad() = 0;

	/// Load all sounds marked as used this level
	virtual void EndLevelLoad() = 0;

	virtual void Preload(idPreloadManifest &preload) = 0;

	/// prints memory info
	virtual void PrintMemInfo(MemInfo_t *mi) = 0;
};

extern ISoundSystem	*soundSystem;