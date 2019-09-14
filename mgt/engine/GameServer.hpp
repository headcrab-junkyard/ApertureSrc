/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

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