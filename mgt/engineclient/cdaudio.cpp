/// @file

#include "quakedef.h"
#include "icdaudio.h"

ICDAudio *gpCDAudio{nullptr};

int CDAudio_Init()
{
	gpCDAudio = new CCDAudioNull();
	
	return gpCDAudio->Init();
};

void CDAudio_Shutdown()
{
	gpCDAudio->Shutdown();
};

void CDAudio_Update()
{
	gpCDAudio->Update();
};

void CDAudio_Play(byte track, bool looping)
{
	gpCDAudio->Play(track, looping);
};

void CDAudio_Stop()
{
	gpCDAudio->Stop();
};

void CDAudio_Pause()
{
	gpCDAudio->Pause();
};

void CDAudio_Resume()
{
	gpCDAudio->Resume();
};