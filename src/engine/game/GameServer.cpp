/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2020-2022 BlackPhrase
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

#include "quakedef.h"
#include "GameServer.hpp"

#include <networksystem/INetServer.hpp>

void CGameServer::Init()
{
};

void CGameServer::Shutdown()
{
};

/*
==================
SV_Frame

==================
*/
#ifdef FPS_20

void CGameServer::_Frame(double host_frametime)
{
	// run the world state
	gGlobalVariables.frametime = host_frametime;

	// read client messages
	SV_RunClients();

	// move things around and think
	// always pause in single player if in console or menus
	if(!sv.paused && (svs.maxclients > 1 || key_dest == key_game))
		GameFrame(host_frametime);
};

void CGameServer::Frame(double host_frametime)
{
	float save_host_frametime;
	float temp_host_frametime;

	// run the world state
	gGlobalVariables.frametime = host_frametime;

	// set the time and clear the general datagram
	SV_ClearDatagram();

	// check for new clients
	//SV_CheckForNewClients ();

	temp_host_frametime = save_host_frametime = host_frametime;
	while(temp_host_frametime > (1.0 / 72.0))
	{
		if(temp_host_frametime > 0.05)
			host_frametime = 0.05;
		else
			host_frametime = temp_host_frametime;
		temp_host_frametime -= host_frametime;
		_Frame(host_frametime);
	};
	host_frametime = save_host_frametime;

	// send all messages to the clients
	SV_SendClientMessages();
};

#else

//void CGameServer::Frame(float afTime)
void CGameServer::Frame(double host_frametime)
{
	// Run the world state
	gGlobalVariables.frametime = host_frametime;
	
	// Set the time and clear the general datagram
	//SV_ClearDatagram(); // TODO: remove?
	
	// Check timeouts
	CheckTimeouts();
	
	// Toggle the log buffer if full
	//SV_CheckLog(); // TODO: qw

	// Move things around and think
	// Always pause in single player if in console or menus
	if(!sv.paused && (svs.maxclients > 1 /*|| key_dest == key_game*/)) // TODO
		GameFrame(host_frametime);

	// Get packets
	ReadPackets();

	// Check for commands typed to the host
	//SV_GetConsoleCommands (); // TODO: handled by Host_GetConsoleCommands

	// Process console commands
	//Cbuf_Execute(); // TODO: qw

	//SV_CheckVars(); // TODO: qw

	// Send messages back to the clients that had packets read this frame
	SendClientMessages();

	// Send a heartbeat to the master if needed
	//Master_Heartbeat (); // TODO: qw

	// TODO: qw

	// Collect timing statistics
	/*
	end = Sys_FloatTime();
	mStats.active += end - start;
	if(++mStats.count == STATFRAMES)
	{
		mStats.latched_active = mStats.active;
		mStats.latched_idle = mStats.idle;
		mStats.latched_packets = mStats.packets;
		
		mStats.active = 0;
		mStats.idle = 0;
		mStats.packets = 0;
		mStats.count = 0;
	};
*/
};

#endif

void CGameServer::Activate()
{
	mpEventBroadcaster->OnActivate(mnMaxClients);
};

void CGameServer::Deactivate()
{
	mpEventBroadcaster->OnDeactivate();
};

void CGameServer::BroadcastPrintf(/*int level,*/ const char *asMsg, ...)
{
	va_list ArgList{};
	char sFmtMsg[1024]{};
	
	va_start(ArgList, asMsg);
	vsprintf(sFmtMsg, asMsg, ArgList);
	va_end(ArgList);
	
	for(auto It : mvClients)
		It->SendPrint(sFmtMsg);
};

void CGameServer::BroadcastCmd(const char *asCmd, ...)
{
	va_list ArgList{};
	char sFmtMsg[1024]{};
	
	va_start(ArgList, asMsg);
	vsprintf(sFmtMsg, asMsg, ArgList);
	va_end(ArgList);
	
	for(auto It : mvClients)
		It->SendCmd(sFmtMsg);
};

/*
================
SV_SendReconnect

Tell all the clients that the server is changing levels
================
*/
void CGameServer::ReconnectAllClients()
{
	//for(auto It : mvClients)
		//It->Reconnect();
	
	char data[128]{};
	sizebuf_t msg;

	msg.data = reinterpret_cast<byte*>(data);
	msg.cursize = 0;
	msg.maxsize = sizeof(data);

	//msg.WriteChar(svc_stufftext);
	//msg.WriteString("reconnect\n");
	//NET_SendToAll(&msg, 5);
	BroadcastCmd("reconnect\n"); // TODO: local client will also receive this command, so why should we manually execute it below?
	
	// TODO: find a better way to do that
	if(cls.state != ca_dedicated)
#ifdef QUAKE2
		Cbuf_InsertText("reconnect\n");
#else
		Cmd_ExecuteString("reconnect\n", src_command);
#endif
};

void CGameServer::DisconnectAllClients()
{
};

int CGameServer::GetActiveClientsNum() const
{
	int nCount{0};
	
	for(int i = 0; i < svs.maxclients; ++i)
		if(svs.clients[i].active)
			nCount++;
	
	return nCount;
};

/*
==================
SV_CheckTimeouts

If a packet has not been received from a client in sv_timeout.GetValue()
seconds, drop the conneciton.

When a client is normally dropped, the client_t goes into a zombie state
for a few seconds to make sure any final reliable message gets resent
if necessary
==================
*/
void CGameServer::CheckTimeouts()
{
	int i{0};
	CGameClient *cl{svs.clients};

	float fDropTime = realtime - sv_timeout.GetValue();
	int nClients{0};

	for(; i < svs.maxclients; i++, cl++)
	{
		if(cl->connected || cl->spawned)
		{
			//if(!cl->spectator) // TODO: no spectator field?
				//++nClients;
			
			if(cl->GetNetChan()->last_received < fDropTime)
			{
				//BroadcastPrintf(PRINT_HIGH, "%s timed out\n", cl->GetName()); // TODO
				cl->Drop(false, "Timed out");
				cl->active = false; // don't bother with zombie state
			};
		};

		//if(cl->active == true && realtime - cl->connection_started > zombietime.value) // TODO
			//cl->active = false; // Can now be reused
	};

	// TODO: non-GS behavior
	//if(sv.paused && !nClients)
	{
		// Nobody left, unpause the server
		//SV_TogglePause("Pause released since no players are left.\n"); // TODO
	};
};

/*
================
SV_Physics

================
*/
void CGameServer::GameFrame(double host_frametime)
{
	// Let the game know that a new frame has started
	gGlobalVariables.time = sv.time;
	
	gpGame->StartFrame();
	
	if(gGlobalVariables.force_retouch)
		--gGlobalVariables.force_retouch;

	sv.time += host_frametime;
};

/*
=================
SV_FilterPacket
=================
*/
bool CGameServer::FilterPacket(const netadr_t &net_from)
{
	uint in = *(uint *)net_from.ip;

	for(int i = 0; i < numipfilters; i++)
		if((in & ipfilters[i].mask) == ipfilters[i].compare)
			return sv_filterban.GetValue();

	return !sv_filterban.GetValue();
};

/*
=================
SV_SendBan
=================
*/
void CGameServer::SendBan(const netadr_t &net_from)
{
	char data[128]{};

	data[0] = data[1] = data[2] = data[3] = 0xff;
	data[4] = A2C_PRINT;
	data[5] = 0;
	
	Q_strcat(data, "\nbanned.\n");
	
	mpNetServer->SendPacket(Q_strlen(data), data, net_from);
};

/*
=================
SV_ConnectionlessPacket

A connectionless packet has four leading 0xff
characters to distinguish it from a game channel.
Clients that are in the game can still send
connectionless packets.
=================
*/
void CGameServer::HandleConnectionlessPacket(const INetAdr &net_from, const IReadBuffer &net_message)
{
	char *s;

	net_message.BeginReading();
	net_message.ReadLong(); // Skip the -1 marker

	//s = net_message.ReadStringLine(); // TODO
	
	//Cmd_TokenizeString(s);
	CCmdArgs Args(s);

	const char *c{Args.GetByIndex(0)};

	if(!Q_strcmp(c, "ping") || (c[0] == A2A_PING && (c[1] == 0 || c[1] == '\n')))
	{
		SVC_Ping(net_from);
		return;
	}
	if(c[0] == A2A_ACK && (c[1] == 0 || c[1] == '\n'))
	{
		gpSystem->Printf("A2A_ACK from %s\n", net_from.ToString());
		return;
	}
	else if(!Q_strcmp(c, "status"))
	{
		SVC_Status();
		return;
	}
	else if(!Q_strcmp(c, "log"))
	{
		SVC_Log(net_from, Args);
		return;
	}
	else if(!Q_strcmp(c, "connect"))
	{
		SVC_DirectConnect(net_from, Args);
		return;
	}
	else if(!Q_strcmp(c, "getchallenge"))
	{
		SVC_GetChallenge(net_from);
		return;
	}
	else if(!Q_strcmp(c, "rcon"))
		SVC_RemoteCommand(net_from, net_message, Args);
	else
		gpSystem->Printf("bad connectionless packet from %s:\n%s\n", net_from.ToString(), s);
};

/*
===================
SV_ExecuteClientMessage

The current net_message is parsed for the given client
===================
*/
void CGameServer::ExecuteClientMessage(CGameClient *cl, const IReadBuffer &aBuffer)
{
	int c;
	//char *s;

	// Calc ping time
	client_frame_t *frame{&cl->frames[cl->GetNetChan()->incoming_acknowledged & UPDATE_MASK]};
	frame->ping_time = realtime - frame->senttime;

	// Make sure the reply sequence number matches the incoming
	// sequence number 
	if(cl->GetNetChan()->incoming_sequence >= cl->GetNetChan()->outgoing_sequence)
		cl->GetNetChan()->outgoing_sequence = cl->GetNetChan()->incoming_sequence;
	else
		cl->send_message = false; // Don't reply, sequences have slipped		

	// Save time for ping calculations
	cl->frames[cl->GetNetChan()->outgoing_sequence & UPDATE_MASK].senttime = realtime;
	cl->frames[cl->GetNetChan()->outgoing_sequence & UPDATE_MASK].ping_time = -1;

	host_client = cl;
	sv_player = host_client->edict;
	pmove = &svpmove;

//	int seq_hash = (cl->GetNetChan()->incoming_sequence & 0xffff) ; // ^ QW_CHECK_HASH;
	int seq_hash = cl->GetNetChan()->incoming_sequence;
	
	// Mark time so clients will know how much to predict
	// other players
 	cl->localtime = sv.time;
	cl->delta_sequence = -1; // no delta unless requested
	
	while(1)
	{
		if(msg_badread)
		{
			gpSystem->Printf("SV_ReadClientMessage: badread\n");
			cl->Drop(false, "Bad message read");
			return;
		};
		
		c = aBuffer.ReadByte();
		
		// End of message(?)
		if(c == -1)
			break;
		
		switch(c)
		{
		default:
			// TODO
			INetMsgHandler *pHandler{NetMsgHandlerManager->GetHandlerForMsg(net_message.GetID())};
			
			//if(!mpGame->HandleClientMessage(cl->GetID(), aBuffer))
			if(!pHandler)
			{
				gpSystem->Printf("SV_ReadClientMessage: unknown command char (%d)\n", c);
				//cl->Drop(false, "Unknown command char"); // TODO: just ignore the message for now
				return;
			};
			
			pHandler->Handle(cl, aBuffer);
			break;
		
		case clc_nop:
			break;
		
		//clc_move
		
		case clc_stringcmd:
			CCLC_StringCmdMsgHandler handler;
			break;
		
		//clc_delta
		//clc_resourcelist
		//clc_tmove
		
		case clc_fileconsistency:
			CCLC_FileConsistencyMsgHandler handler;
			break;
		
		//clc_voicedata
		//clc_hltv
		
		//clc_cvarvalue
		
		case clc_cvarvalue2:
			CCLC_CVarValue2MsgHandler handler;
			break;
		};
	};
};

/*
=================
SV_ReadPackets
=================
*/
void CGameServer::ReadPackets()
{
	int i;
	CGameClient *cl;
	int qport;

	bool good{false};
	
	CNetAdr net_from;
	CReadBuffer net_message;
	
	while(mpNetServer->GetPacket(net_from, net_message))
	{
		if(FilterPacket(net_from))
		{
			SendBan(net_from); // Tell them we aren't listening...
			continue;
		};

		// Check for connectionless packet (0xffffffff) first
		if(*(int*)net_message.data == -1)
		{
			HandleConnectionlessPacket(net_message);
			continue;
		};

		// Read the qport out of the message so we can fix up
		// Stupid address translating routers
		MSG_BeginReading();
		net_message.ReadLong(); // Sequence number
		net_message.ReadLong(); // Sequence number
		qport = net_message.ReadShort() & 0xffff;

		// Check for packets from connected clients
		for(i = 0, cl = svs.clients; i < svs.maxclients; i++, cl++)
		{
			if(!cl->active)
				continue;
			if(!NET_CompareBaseAdr(net_from, cl->GetNetChan()->remote_address))
				continue;
			if(cl->GetNetChan()->qport != qport)
				continue;
			if(cl->GetNetChan()->remote_address.port != net_from.port)
			{
				gpSystem->DevPrintf("SV_ReadPackets: fixing up a translated port\n");
				cl->GetNetChan()->remote_address.port = net_from.port;
			};
			if(cl->GetNetChan()->Process(&net_message))
			{
				// This is a valid, sequenced packet, so process it
				mStats.packets++;
				good = true;
				cl->send_message = true; // Reply at end of frame
				ExecuteClientMessage(cl, net_message);
			};
			break;
		};

		if(i != svs.maxclients)
			continue;

		// Packet is not from a known client
		//gpSystem->Printf("%s:sequenced packet without connection\n", net_from.ToString());
	};
};

/*
=======================
SV_SendClientMessages
=======================
*/
void CGameServer::SendClientMessages()
{
	int i, j;
	client_t *c;

	// Update frags, names, etc
	SV_UpdateToReliableMessages();

	// Build individual updates
	for(i = 0, c = svs.clients; i < svs.maxclients; i++, c++)
	{
		if(!c->active)
			continue;

		if(c->drop)
		{
			c->Drop(false, "Dropped by the server!");
			c->drop = false;
			continue;
		};

		// TODO
		/*
		// Check to see if we have a backbuf to stick in the reliable
		if (c->num_backbuf)
		{
			// Will it fit?
			if(c->GetNetChan()->message.cursize + c->backbuf_size[0] < c->GetNetChan()->message.maxsize)
			{
				gpSystem->DevPrintf("%s: backbuf %d bytes\n", c->GetName(), c->backbuf_size[0]);

				// it'll fit
				SZ_Write(&c->GetNetChan()->message, c->backbuf_data[0], c->backbuf_size[0]);
				
				//move along, move along
				for (j = 1; j < c->num_backbuf; j++)
				{
					Q_memcpy(c->backbuf_data[j - 1], c->backbuf_data[j], c->backbuf_size[j]);
					c->backbuf_size[j - 1] = c->backbuf_size[j];
				};

				c->num_backbuf--;
				
				if(c->num_backbuf)
				{
					Q_memset(&c->backbuf, 0, sizeof(c->backbuf));
					c->backbuf.data = c->backbuf_data[c->num_backbuf - 1];
					c->backbuf.cursize = c->backbuf_size[c->num_backbuf - 1];
					c->backbuf.maxsize = sizeof(c->backbuf_data[c->num_backbuf - 1]);
				};
			};
		};
		*/

		// the player isn't totally in the game yet
		// send small keepalive messages if too much time has passed
		// send a full message when the next signon stage has been requested
		// some other message data (name changes, etc) may accumulate
		// between signon stages
		/*
		if(!c->sendsignon)
		{
			if(realtime - c->last_message > 5)
				c->SendNop();
			continue; // don't send out non-signon messages
		};
		*/

		// If the reliable message overflowed, drop the client
		if(c->GetNetChan()->message.overflowed)
		{
			c->GetNetChan()->message->Clear();
			c->datagram.Clear(&);
			//SV_BroadcastPrintf(PRINT_HIGH, "%s overflowed\n", c->name); // TODO
			gpSystem->Printf("WARNING: reliable overflow for %s\n", c->name);
			c->Drop(true, "Overflowed!");
			c->send_message = true;
			c->GetNetChan()->cleartime = 0; // don't choke this message
		};

		// Only send messages if the client has sent one
		// and the bandwidth is not choked
		if(!c->send_message)
			continue;

		c->send_message = false; // Try putting this after choke?

		if(!sv.paused && !c->GetNetChan()->CanPacket())
		{
			c->chokecount++;
			continue; // Bandwidth choke
		};

		/*
		if(c->GetNetChan()->message.cursize)
		{
			if(!c->GetNetChan()->CanPacket()) // TODO: was NET_CanSendMessage; Netchan_CanReliable?
			{
//				I_Printf("can't write\n");
				continue;
			};

			if (c->dropasap)
				SV_DropClient(false);	// went to another level
			else
			{
				if (NET_SendMessage (c->netchan, &c->netchan.message) == -1)
					SV_DropClient (true);	// if the message couldn't send, kick off
				SZ_Clear (&c->netchan.message);
				c->last_message = realtime;
				c->sendsignon = false;
			};
		};
		*/

		if(c->spawned)
			c->SendDatagram();
		else
			c->GetNetChan()->Transmit(0, nullptr); // Just update reliable
	};

	// Clear muzzle flashes
	SV_CleanupEnts();
};