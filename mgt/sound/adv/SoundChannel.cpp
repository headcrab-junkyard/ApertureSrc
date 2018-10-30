/// @file

#include "SoundChannel.hpp"

extern idCVar s_playDefaultSound;

/*
========================
CSoundChannel::CSoundChannel
========================
*/
CSoundChannel::CSoundChannel()
{
	emitter = NULL;
	hardwareVoice = NULL;

	startTime = 0;
	endTime = 0;
	leadinSample = NULL;
	loopingSample = NULL;
	logicalChannel = SCHANNEL_ANY;
	allowSlow = false;
	soundShader = NULL;

	volumeFade.Clear();

	volumeDB = DB_SILENCE;
	currentAmplitude = 0.0f;
}

/*
========================
CSoundChannel::~CSoundChannel
========================
*/
CSoundChannel::~CSoundChannel()
{
}

/*
========================
CSoundChannel::CanMute
Never actually mute VO because we can't restart them precisely enough for lip syncing to not fuck up
========================
*/
bool CSoundChannel::CanMute() const
{
	return true;
}

/*
========================
CSoundChannel::Mute

A muted sound is considered still running, and can restart when a listener
gets close enough.
========================
*/
void CSoundChannel::Mute()
{
	if(hardwareVoice != NULL)
	{
		soundSystemLocal.FreeVoice(hardwareVoice);
		hardwareVoice = NULL;
	}
}

/*
========================
CSoundChannel::IsLooping
========================
*/
bool CSoundChannel::IsLooping() const
{
	return (parms.soundShaderFlags & SSF_LOOPING) != 0;
}

/*
========================
CSoundChannel::CheckForCompletion
========================
*/
bool CSoundChannel::CheckForCompletion(int currentTime)
{
	if(leadinSample == NULL)
	{
		return true;
	}
	// endTime of 0 indicates a sound should loop forever
	if(endTime > 0 && endTime < currentTime)
	{
		return true;
	}
	return false;
}

/*
========================
CSoundChannel::UpdateVolume
========================
*/
void CSoundChannel::UpdateVolume(int currentTime)
{
	idSoundWorldLocal *soundWorld = emitter->soundWorld;

	volumeDB = DB_SILENCE;
	currentAmplitude = 0.0f;

	if(leadinSample == NULL)
	{
		return;
	}
	if(startTime > currentTime)
	{
		return;
	}
	if(endTime > 0 && endTime < currentTime)
	{
		return;
	}

	// if you don't want to hear all the beeps from missing sounds
	if(leadinSample->IsDefault() && !s_playDefaultSound.GetBool())
	{
		return;
	}

	bool emitterIsListener = (emitter->emitterId == soundWorld->listener.id);

	// if it is a private sound, set the volume to zero unless we match the listener.id
	if(parms.soundShaderFlags & SSF_PRIVATE_SOUND)
	{
		if(!emitterIsListener)
		{
			return;
		}
	}
	if(parms.soundShaderFlags & SSF_ANTI_PRIVATE_SOUND)
	{
		if(emitterIsListener)
		{
			return;
		}
	}

	// volume fading
	float newVolumeDB = parms.volume;
	newVolumeDB += volumeFade.GetVolume(currentTime);
	newVolumeDB += soundWorld->volumeFade.GetVolume(currentTime);
	newVolumeDB += soundWorld->pauseFade.GetVolume(currentTime);
	if(parms.soundClass >= 0 && parms.soundClass < SOUND_MAX_CLASSES)
	{
		newVolumeDB += soundWorld->soundClassFade[parms.soundClass].GetVolume(currentTime);
	}

	bool global = (parms.soundShaderFlags & SSF_GLOBAL) != 0;

	// attenuation
	if(!global && !emitterIsListener)
	{
		float distance = (parms.soundShaderFlags & SSF_NO_OCCLUSION) == 0 ? emitter->spatializedDistance : emitter->directDistance;
		float mindist = parms.minDistance;
		float maxdist = parms.maxDistance;
		if(distance >= maxdist)
		{
			newVolumeDB = DB_SILENCE;
		}
		else if((distance > mindist) && (maxdist > mindist))
		{
			float f = (distance - mindist) / (maxdist - mindist);
			newVolumeDB += LinearToDB(Square(1.0f - f));
		}
	}

	if(soundSystemLocal.musicMuted && (parms.soundShaderFlags & SSF_MUSIC) != 0)
	{
		newVolumeDB = DB_SILENCE;
	}

	// store the new volume on the channel
	volumeDB = newVolumeDB;

	// keep track of the maximum volume
	float currentVolumeDB = newVolumeDB;
	if(hardwareVoice != NULL)
	{
		float amplitude = hardwareVoice->GetAmplitude();
		if(amplitude <= 0.0f)
		{
			currentVolumeDB = DB_SILENCE;
		}
		else
		{
			currentVolumeDB += LinearToDB(amplitude);
		}
		currentAmplitude = amplitude;
	}
}

/*
========================
CSoundChannel::UpdateHardware
========================
*/
void CSoundChannel::UpdateHardware(float volumeAdd, int currentTime)
{
	idSoundWorldLocal *soundWorld = emitter->soundWorld;

	if(soundWorld == NULL)
	{
		return;
	}
	if(leadinSample == NULL)
	{
		return;
	}
	if(startTime > currentTime)
	{
		return;
	}
	if(endTime > 0 && endTime < currentTime)
	{
		return;
	}

	// convert volumes from decibels to linear
	float volume = Max(0.0f, DBtoLinear(volumeDB + volumeAdd));

	if((parms.soundShaderFlags & SSF_UNCLAMPED) == 0)
	{
		volume = Min(1.0f, volume);
	}

	bool global = (parms.soundShaderFlags & SSF_GLOBAL) != 0;
	bool omni = (parms.soundShaderFlags & SSF_OMNIDIRECTIONAL) != 0;
	bool emitterIsListener = (emitter->emitterId == soundWorld->listener.id);

	int startOffset = 0;
	bool issueStart = false;

	if(hardwareVoice == NULL)
	{
		if(volume <= 0.00001f)
		{
			return;
		}

		hardwareVoice = soundSystemLocal.AllocateVoice(leadinSample, loopingSample);

		if(hardwareVoice == NULL)
		{
			return;
		}

		issueStart = true;
		startOffset = currentTime - startTime;
	}

	if(omni || global || emitterIsListener)
	{
		hardwareVoice->SetPosition(vec3_zero);
	}
	else
	{
		hardwareVoice->SetPosition((emitter->spatializedOrigin - soundWorld->listener.pos) * soundWorld->listener.axis.Transpose());
	}
	if(parms.soundShaderFlags & SSF_VO)
	{
		hardwareVoice->SetCenterChannel(s_centerFractionVO.GetFloat());
	}
	else
	{
		hardwareVoice->SetCenterChannel(0.0f);
	}

	extern idCVar timescale;

	hardwareVoice->SetGain(volume);
	hardwareVoice->SetInnerRadius(parms.minDistance * METERS_TO_DOOM);
	hardwareVoice->SetPitch(soundWorld->slowmoSpeed * idMath::ClampFloat(0.2f, 5.0f, timescale.GetFloat()));

	if(soundWorld->enviroSuitActive)
	{
		hardwareVoice->SetOcclusion(0.5f);
	}
	else
	{
		hardwareVoice->SetOcclusion(0.0f);
	}

	if(issueStart)
	{
		hardwareVoice->Start(startOffset, parms.soundShaderFlags | (parms.shakes == 0.0f ? SSF_NO_FLICKER : 0));
	}
	else
	{
		hardwareVoice->Update();
	}
}