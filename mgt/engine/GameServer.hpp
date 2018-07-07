/// @file

#pragma once

#include <vector>
#include "engine/IGameServer.hpp"

using tGameClientVec = std::vector<IGameClient *>;

class CGameServer final : public IGameServer
{
public:
	CGameServer(server_t *apsv, server_static_t *apsvs);
	~CGameServer();

	void BroadcastPrintf(const char *asMsg, ...);
	void BroadcastCmd(const char *asCmd, ...);

	void ReconnectAllClients();
	void DisconnectAllClients();

	IGameClient *GetClient(int index) const override;
private:
	tGameClientVec mvClients;
	
	server_t *mpsv{nullptr};
	server_static_t *mpsvs{nullptr};
};