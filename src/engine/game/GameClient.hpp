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

#include <CommonTypes.hpp>
//#include <engine/IGameClient.hpp>

constexpr auto NUM_PING_TIMES{16};
constexpr auto NUM_SPAWN_PARMS{16};

using sizebuf_t = struct sizebuf_s;

struct client_frame_t
{
	// received from client

	// reply
	double				senttime;
	float				ping_time;
	packet_entities_t	entities;
};

interface INetMsg;

class CGameClient //final : public IGameClient
{
public:
	//CGameClient();
	//~CGameClient();
	
	void Printf(const char *asMsg, ...); // override
	
	void SendCmd(const char *asCmd, ...); //override;
	void SendMsg(const INetMsg &aMsg);
	//void SendSnapshot(); // TODO: q3
	
	//void WriteDownload(const INetMsg &aMsg); // TODO: IWriteBuffer?
	
	void Drop(bool abCrash, const char *asReason, ...) override; // TODO: Disconnect
	
	const char *GetName() const override {return name;}
	int GetUserID() const override {return userid;}
	uint GetWONID() const override {return wonid;}
	const char *GetAuthID() const override {return authid;}
	
	void QueryCvarValue(const char *asCvarName) override;
	void QueryCvarValueEx(const char *asCvarName, int anRequestID) override;
public:
	void SendNop();
	void SendDatagram();
	
	INetChan *GetNetChan() const;
public:
	//sizebuf_t message{};
	
	bool active{false}; // false = client is free
	bool spawned{false}; // client is fully in game (false = don't send datagrams)
	bool connected; // has been assigned to a client_t, but not in game yet
	bool drop;      // has been told to go to another level
	
	//int				lossage;			// loss percentage

	bool sendinfo; // at end of frame, send info to all
	                   // this prevents malicious multiple broadcasts

	int userid;                     // identifying number
	char userinfo[MAX_INFO_STRING]; // infostring (name, etc)

	usercmd_t lastcmd; // for filling in big drops and partial predictions
	double localtime;  // of last message

	char name[32];  // for printing to other people (extracted from userinfo)
	
	int topcolor;
	int bottomcolor;

	client_frame_t frames[UPDATE_BACKUP]; // updates can be deltad from here
	
	float ping_times[NUM_PING_TIMES];
	int num_pings; // ping_times[num_pings%NUM_PING_TIMES]

	// the datagram is written to after every frame, but only cleared
	// when it is sent out to the client.  overflow is tolerated.
	sizebuf_t datagram;
	byte datagram_buf[MAX_DATAGRAM];

	double connection_started; // or time of disconnect for zombies
	bool send_message;     // set on frames a datagram arived on

	// spawn parms are carried from level to level
	float spawn_parms[NUM_SPAWN_PARMS];

	// client known data for deltas
	int old_frags;

	//===== NETWORK ============
	int chokecount;
	int delta_sequence; // -1 = no compression
	//netchan_t netchan; // TODO
	
	// MGT
	
	int protocol{0};
};

/*
class CGamePlayer
{
public:
	void SetViewEntity(const IEntity *apViewEnt) override {mpViewEnt = apViewEnt;}
	
	IEntity *GetEntity() const override {return mpEntity;}
private:
	IEntity *mpEntity{nullptr};
	const IEntity *mpViewEnt{nullptr};
};
*/