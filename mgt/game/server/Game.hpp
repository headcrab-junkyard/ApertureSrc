/// @file

#pragma once

#include "game/server/IGame.hpp"

class CGame final : public IGame
{
public:
	CGame() = default;
	~CGame() = default;
	
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	void Update() override;
private:
};