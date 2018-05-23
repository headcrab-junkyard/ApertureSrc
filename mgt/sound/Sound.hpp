/// @file

#pragma once

#include "sound/ISound.hpp"

class CSound final : public ISound
{
public:
	CSound();
	~CSound();
	
	bool Init() override;
	void Shutdown() override;
	
	void Update(float, float, float, float) override;
private:
};