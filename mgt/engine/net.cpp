/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

// net_ws.c

#include <memory>
#include "quakedef.h"
#include "network/INetwork.hpp"

// TODO
#include "null/NetworkNull.hpp"

void *gpNetworkLib{nullptr};
INetwork *gpNetwork{nullptr};

// TODO

//netadr_t net_local_adr;

netadr_t net_from;
sizebuf_t net_message;

/*
#ifdef _WIN32

int net_socket;

#define MAX_UDP_PACKET (MAX_MSGLEN * 2) // one more than msg + header

#elif __linux__

int net_socket;      // non blocking, for receives
int net_send_socket; // blocking, for sends

#define MAX_UDP_PACKET 8192

int gethostname(char *, int);
int close(int);

#endif

byte net_message_buffer[MAX_UDP_PACKET];

#ifdef _WIN32
WSADATA winsockdata;
#endif

*/

//=============================================================================

bool LoadNetworkModule()
{
	gpNetworkLib = Sys_LoadModule("network");

	if(!gpNetworkLib)
		return false;

	auto pfnNetworkFactory{Sys_GetFactory(gpNetworkLib)};

	if(!pfnNetworkFactory)
		return false;

	gpNetwork = (INetwork*)pfnNetworkFactory(MGT_NETWORK_INTERFACE_VERSION, nullptr);

	if(!gpNetwork)
		return false;
};

void UnloadNetworkModule()
{
	if(gpNetworkLib)
	{
		Sys_UnloadModule(gpNetworkLib);
		gpNetworkLib = nullptr;
	};
};

//=============================================================================

/*
void NetadrToSockadr(netadr_t *a, struct sockaddr_in *s)
{
};

void SockadrToNetadr(struct sockaddr_in *s, netadr_t *a)
{
};
*/

bool NET_CompareBaseAdr(netadr_t a, netadr_t b)
{
	return false; // TODO: gpNetwork->CompareBaseAdr(a, b);
};

bool NET_CompareAdr(netadr_t a, netadr_t b)
{
	return gpNetwork->CompareAdr(&a, &b);
};

char *NET_AdrToString(netadr_t a)
{
	return gpNetwork->AdrToString(&a);
};

/*
char *NET_BaseAdrToString(netadr_t a)
{
	return s;
};
*/

/*
bool NET_StringToAdr(char *s, netadr_t *a)
{
	return true;
};
*/

bool NET_GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
	return gpNetwork->GetPacket(sock, net_from, net_message);
};

void NET_SendPacket(netsrc_t sock, int length, void *data, netadr_t to)
{
	gpNetwork->SendPacket(sock, length, data, to);
};

/*
int UDP_OpenSocket(int port)
{
};

void NET_GetLocalAddress()
{
};
*/

void NET_Init(/*int port*/)
{
	static std::unique_ptr<CNetworkNull> pNetworkNull;
	
	if(!LoadNetworkModule())
	{
		if(!pNetworkNull)
			pNetworkNull = std::make_unique<CNetworkNull>();
		
		gpNetwork = pNetworkNull.get();
	};

	if(!gpNetwork->Init(Sys_GetFactoryThis()))
		return;
};

void NET_Shutdown()
{
	if(gpNetwork)
		gpNetwork->Shutdown();

	gpNetwork = nullptr;
	
	UnloadNetworkModule();
};

/*
// Returns true if we can't bind the address locally--in other words,
// the IP is NOT one of our interfaces.
bool NET_IsClientLegal(netadr_t *adr)
{
	struct sockaddr_in sadr;
	int newsocket;

#if 0
	if (adr->ip[0] == 127)
		return false; // no local connections period

	NetadrToSockadr (adr, &sadr);

	if ((newsocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		Sys_Error ("NET_IsClientLegal: socket:", strerror(errno));

	sadr.sin_port = 0;

	if( bind (newsocket, (void *)&sadr, sizeof(sadr)) == -1) 
	{
		// It is not a local address
		close(newsocket);
		return true;
	}
	close(newsocket);
	return false;
#else
	return true;
#endif
}
*/