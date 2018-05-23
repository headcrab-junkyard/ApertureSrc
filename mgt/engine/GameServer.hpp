/// @file

#pragma once

#include <vector>
#include "engine/IGameServer.hpp"

using tGameClientVec = std::vector<IGameClient*>;

class CGameServer final : public IGameServer
{
public:
	CGameServer();
	~CGameServer();
	
	void BroadcastPrintf(const char *asMsg, ...);
	void BroadcastCmd(const char *asCmd, ...);
	
	void ReconnectAllClients();
	void DisconnectAllClients();
	
	IGameClient *GetClient(int index) const override;
private:
	tGameClientVec mvClients;
};