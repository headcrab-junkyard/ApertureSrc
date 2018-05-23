/// @file

#pragma once

#include "engine/IGameClient.hpp"

class CGameClient final : public IGameClient
{
public:
	CGameClient();
	~CGameClient();
	
	void Disconnect(const char *asReason, ...) override;
	
	void Printf(const char *asMsg, ...) override;
private:
};