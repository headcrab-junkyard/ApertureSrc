/// @file

#include "SoundFade.hpp"

/*
================================================================================================

	CSoundFade

================================================================================================
*/

/*
========================
CSoundFade::Clear
========================
*/
void CSoundFade::Clear()
{
	fadeStartTime = 0;
	fadeEndTime = 0;
	fadeStartVolume = 0.0f;
	fadeEndVolume = 0.0f;
}

/*
========================
CSoundFade::SetVolume
========================
*/
void CSoundFade::SetVolume(float to)
{
	fadeStartVolume = to;
	fadeEndVolume = to;
	fadeStartTime = 0;
	fadeEndTime = 0;
}

/*
========================
CSoundFade::Fade
========================
*/
void CSoundFade::Fade(float to, int length, int soundTime)
{
	int startTime = soundTime;
	// if it is already fading to this volume at this rate, don't change it
	if(fadeEndTime == startTime + length && fadeEndVolume == to)
	{
		return;
	}
	fadeStartVolume = GetVolume(soundTime);
	fadeEndVolume = to;
	fadeStartTime = startTime;
	fadeEndTime = startTime + length;
}

/*
========================
CSoundFade::GetVolume
========================
*/
float CSoundFade::GetVolume(const int soundTime) const
{
	const float fadeDuration = (fadeEndTime - fadeStartTime);
	const int currentTime = soundTime;
	const float playTime = (currentTime - fadeStartTime);
	if(fadeDuration <= 0.0f)
	{
		return fadeEndVolume;
	}
	else if(currentTime >= fadeEndTime)
	{
		return fadeEndVolume;
	}
	else if(currentTime > fadeStartTime)
	{
		return fadeStartVolume + (fadeEndVolume - fadeStartVolume) * playTime / fadeDuration;
	}
	else
	{
		return fadeStartVolume;
	}
}