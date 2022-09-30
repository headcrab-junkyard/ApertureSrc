/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2005 Id Software, Inc.
 * Copyright (C) 2018-2022 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include <vector>

#include <CommonTypes.hpp>
#include <common/netadr.h>

//#include <next/engine/IGameServer.hpp>

/// MAX_CHALLENGES is made large to prevent a denial
/// of service attack that could cycle all of them
/// out before legitimate users connected
constexpr auto MAX_CHALLENGES{1024};

using client_t = struct client_s;

//=============================================================================

struct challenge_t
{
	netadr_t adr;
	int challenge;
	int time; // time the last packet was sent to the autherize server
	// TODO: q3
	//int			pingTime;			// time the challenge response was sent to client
	//int			firstTime;			// time the adr was first used, for authorize timeout checks
	//bool	connected;
};

//=============================================================================

struct svstats_t
{
	double active;
	double idle;
	int count;
	int packets;

	double latched_active;
	double latched_idle;
	int latched_packets;
};

//=============================================================================

interface IGameClient;
using tGameClientVec = std::vector<IGameClient*>;

interface INetServer;

class CGameServer //final : public IGameServer
{
public:
	//CGameServer(server_t *apsv, server_static_t *apsvs);
	//~CGameServer();
	
	void Init();
	void Shutdown();
	
	void Frame(float afTime);
	
	void Activate();
	void Deactivate();
	
	void AddEventListener(IGameServerEventListener *apListener) override;
	void RemoveEventListener(IGameServerEventListener *apListener) override;
	
	void AddClientEventListener(IGameClientEventListener *apListener) override;
	void RemoveClientEventListener(IGameClientEventListener *apListener) override;
	
	int RegisterUserMsg(const char *asName, int anSize) override; 
	bool RegisterNetMsg(INetMsg *apMsg) override;
	
	bool SetNetMsgHandler(const char *asMsg, INetMsgHandler *apHandler) override;
	
	void ForceExactFile(const char *asName) override;
	void ForceModelBounds(const char *asNae, const vec3_t &avMins, const vec3_t &avMaxs) override;
	
	void BroadcastPrintf(/*int level,*/ const char *asMsg, ...) override;
	void BroadcastCmd(const char *asCmd, ...);
	
	void MessageEnd(INetMsg *apMsg) override;
	
	IGameClient *CreateFakeClient(const char *asName) override;
	
	void ReconnectAllClients();
	//void DisconnectAllClients();
	
	int GetActiveClientsNum() const;
	
	IGameClient *GetClientByName(const char *asName) const override;
	IGameClient *GetClientByIndex(int anIndex) const override {return mvClients.at(anIndex);}
	
	IInfoBuffer *GetServerInfo() const override;
	IInfoBuffer *GetPhysicsInfo() const override;
private:
	void CheckTimeouts();
	
	bool FilterPacket(const netadr_t &net_from);
	void SendBan(const netadr_t &net_from);
	
	void HandleConnectionlessPacket(const IReadBuffer &aBuffer);
	void ExecuteClientMessage(CGameClient *cl, const IReadBuffer &aBuffer)
	
	void ReadPackets();
	
	void SendClientMessages();
private:
	//server_t *mpsv{nullptr};
	//server_static_t *mpsvs{nullptr};
public:
	challenge_t challenges[MAX_CHALLENGES]; // to prevent invalid IPs from connecting
	
	svstats_t mStats;
	
	tGameClientVec mvClients;
	//CGameClient *clients{nullptr}; // [maxclients]
	
	INetServer *mpNetServer{nullptr};
	
	int maxclients{0};
	
	bool active{false};
};