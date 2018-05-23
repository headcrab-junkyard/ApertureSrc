/// @file

#include "Sound.hpp"

EXPOSE_SINGLE_INTERFACE(CSound, ISound, MGT_SOUND_INTERFACE_VERSION);

CSound::CSound() = default;
CSound::~CSound() = default;

bool CSound::Init()
{
	return true;
};

void CSound::Shutdown()
{
};

void CSound::Update(float, float, float, float)
{
};