#include "quakedef.h"
#include "CDAudio.hpp"

int CCDAudio::Init()
{
	return CDAudio_Init();
};

void CCDAudio::Shutdown()
{
	CDAudio_Shutdown();
};

void CCDAudio::Update()
{
	CDAudio_Update();
};

void CCDAudio::Play(byte track, bool looping)
{
	CDAudio_Play(track, looping);
};

void CCDAudio::Stop()
{
	CDAudio_Stop();
};

void CCDAudio::Pause()
{
	CDAudio_Pause();
};

void CCDAudio::Resume()
{
	CDAudio_Resume();
};