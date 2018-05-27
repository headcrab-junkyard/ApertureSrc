/// @file

#pragma once

#include "engine/ISystem.hpp"

class CSystem final : public ISystem
{
public:
	CSystem();
	~CSystem();

	void Error(const char *fmt, ...) override;

	double GetFloatTime() const override;
};

//extern CSystem *gpSystem;