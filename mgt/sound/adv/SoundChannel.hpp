/// @file

#pragma once

class CSoundEmitter;

/*
================================================
idSoundChannel
================================================
*/
class CSoundChannel
{
public:
	// only allocated by the soundWorld block allocator
	CSoundChannel();
	~CSoundChannel();
	
	bool	CanMute() const;

	void	Mute();
	bool	CheckForCompletion( int currentTime );

	void	UpdateVolume( int currentTime );
	void	UpdateHardware( float volumeAdd, int currentTime );

	// returns true if this channel is marked as looping
	bool	IsLooping() const;
	
	CSoundEmitter *emitter;

	int startTime;
	int endTime;
	int logicalChannel;
	bool allowSlow;

	soundShaderParms_t parms; // combines shader parms and per-channel overrides
	const idSoundShader *soundShader;
	
	idSoundSample *leadinSample;
	idSoundSample *loopingSample;
	
	idSoundFade volumeFade;

	float volumeDB;         // last volume at which this channel will play (calculated in UpdateVolume)
	float currentAmplitude; // current amplitude on the hardware voice

	// hardwareVoice will be freed and NULL'd when a sound is out of range,
	// and reallocated when it comes back in range
	idSoundVoice *hardwareVoice;
};