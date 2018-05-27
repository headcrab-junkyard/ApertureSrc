/// @file

#include "sound/ISound.hpp"

class CSoundNull final : public ISound
{
public:
	CSoundNull() = default;
	~CSoundNull() = default;

	bool Init() override { return true; }
	void Shutdown() override {}
	void Update(float, float, float, float) override {}
};