/// @file

#include "quakedef.h"
#include "icdaudio.h"

#include "null/CDAudioNull.hpp"

ICDAudio *gpCDAudio{ nullptr };

int CDAudio_Init()
{
	gpCDAudio = new CCDAudioNull();

	if(gpCDAudio)
		return gpCDAudio->Init();

	return 0;
};

void CDAudio_Shutdown()
{
	if(gpCDAudio)
		gpCDAudio->Shutdown();
};

void CDAudio_Update()
{
	if(gpCDAudio)
		gpCDAudio->Frame();
};

void CDAudio_Play(byte track, bool looping)
{
	//if(gpCDAudio)
		//gpCDAudio->Play(track, looping);
};

void CDAudio_Stop()
{
	//if(gpCDAudio)
		//gpCDAudio->Stop();
};

void CDAudio_Pause()
{
	if(gpCDAudio)
		gpCDAudio->Pause();
};

void CDAudio_Resume()
{
	if(gpCDAudio)
		gpCDAudio->Resume();
};