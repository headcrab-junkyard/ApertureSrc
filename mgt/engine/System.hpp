/// @file

#pragma once

#include "engine/ISystem.hpp"

class CSystem final : public ISystem
{
public:
	CSystem();
	~CSystem();
	
	double GetFloatTime() const override;
};

//extern CSystem *gpSystem;