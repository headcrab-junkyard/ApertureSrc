/// @file

#include "input/IInput.hpp"

class CInputNull final : public IInput
{
public:
	CInputNull() = default;
	~CInputNull() = default;
	
	bool Init() override {return true;}
	void Shutdown() override {}
	
	void Frame() override {}
};