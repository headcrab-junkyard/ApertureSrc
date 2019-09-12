/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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
/// @brief engine's interface to the networking layer

#pragma once

//#define	MAX_MSGLEN		1400		// max length of a message
//#define	PACKET_HEADER	10			// two ints and a short

#include "enums.h"
#include "netadr.h"

extern netadr_t net_local_adr;
extern netadr_t net_from; // address of who sent the packet
extern sizebuf_t net_message;

//extern	int		net_socket;

void NET_Init();
void NET_Shutdown();

void NET_Config(qboolean multiplayer);

bool NET_GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message);
void NET_SendPacket(netsrc_t sock, int length, void *data, netadr_t to);

bool NET_CompareAdr(netadr_t a, netadr_t b);
bool NET_CompareBaseAdr(netadr_t a, netadr_t b);
qboolean NET_IsLocalAddress(netadr_t adr);
char *NET_AdrToString(netadr_t a);
//char		*NET_BaseAdrToString (netadr_t a);
bool NET_StringToAdr(char *s, netadr_t *a);
//void		NET_Sleep(int msec);

//============================================================================

#define OLD_AVG 0.99 // total = oldtotal*OLD_AVG + new*(1-OLD_AVG)

#include "network/INetwork.hpp"

extern int net_drop; // packets dropped before this one

void Netchan_Init();
void Netchan_Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport);

void Netchan_Transmit(netchan_t *chan, int length, byte *data);
void Netchan_OutOfBand(int net_socket, netadr_t adr, int length, byte *data);
void Netchan_OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...);
bool Netchan_Process(netchan_t *chan, sizebuf_t *net_message);

bool Netchan_CanPacket(netchan_t *chan); // TODO: ???
bool Netchan_CanReliable(netchan_t *chan);