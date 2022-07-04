/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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

#include "NetworkSystem.hpp"
//#include "NetChannel.hpp"
#include "NetClient.hpp"
#include "NetServer.hpp"

#include <engine/ISystem.hpp>

//=============================================================================

void SockadrToNetadr(struct sockaddr/*_in*/ *s, netadr_t &a);

EXPOSE_SINGLE_INTERFACE(CNetworkSystem, INetworkSystem, OGS_NETWORKSYSTEM_INTERFACE_VERSION);

CNetworkSystem::CNetworkSystem() = default;
CNetworkSystem::~CNetworkSystem() = default;

/*
====================
NET_Init
====================
*/
bool CNetworkSystem::Init(CreateInterfaceFn afnEngineFactory)
{
	mpSystem = reinterpret_cast<ISystem*>afnEngineFactory(OGS_SYSTEM_INTERFACE_VERSION, nullptr);
	
	if(!mpSystem)
	{
		printf("ISystem query failed!\n");
		return false;
	};
	
	mpImpl->Init();

	mpSystem->Printf("Base networking initialized.\n");
	return true;
};

/*
====================
NET_Shutdown
====================
*/
void CNetworkSystem::Shutdown()
{
	Config(false); // close sockets
	
	mpImpl->Shutdown();
};

/*
void CNetworkSystem::Frame(float afTimeStep)
{
};
*/

/*
====================
NET_Config

A single player game will only use the loopback code
====================
*/
void CNetworkSystem::Config(bool abEnableNetworking)
{
	if(mbEnableNetworking == abEnableNetworking)
		return;
	
	mbEnableNetworking = abEnableNetworking;
	
	if(!abEnableNetworking)
	{
		// Shut down any existing sockets
		for(int i = 0; i < 2; ++i)
		{
			if(ip_sockets[i])
			{
#ifdef _WIN32
				closesocket(ip_sockets[i]);
#else
				close(ip_sockets[i]);
#endif
				ip_sockets[i] = 0;
			};
			
			if(ipx_sockets[i])
			{
#ifdef _WIN32
				closesocket(ipx_sockets[i]);
#else
				close(ipx_sockets[i]);
#endif
				ipx_sockets[i] = 0;
			};
		};
	}
	else
	{	// Open sockets
		if(!noudp) // TODO: win-only check
			OpenIP();
		if(!noipx) // TODO: win-only check
			OpenIPX();
	};
};

INetServer *CNetworkSystem::StartServer(int anPort)
{
	if(!mpServer)
		mpServer = std::make_unique<CNetServer>(anPort);
	
	return mpServer.get();
};

INetClient *CNetworkSystem::StartClient()
{
	if(!mpClient)
		mpClient = std::make_unique<CNetClient>();
	
	return mpClient.get();
};

/*
=============
NET_StringToAdr

idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
bool CNetworkSystem::StringToAdr(const char *s, netadr_t &a)
{
	struct hostent *h;
	struct sockaddr_in sadr;
	char *colon;
	char copy[128];

	memset(&sadr, 0, sizeof(sadr));
	sadr.sin_family = AF_INET;

	sadr.sin_port = 0;

	strcpy(copy, s);
	// strip off a trailing :port if present
	for(colon = copy; *colon; colon++)
		if(*colon == ':')
		{
			*colon = 0;
			sadr.sin_port = htons((short)atoi(colon + 1)); // TODO: (short)?
		};

	if(copy[0] >= '0' && copy[0] <= '9')
	{
		*(int *)&sadr.sin_addr = inet_addr(copy);
	}
	else
	{
#ifdef _WIN32
		if((h = gethostbyname(copy)) == 0)
#else
		if(!(h = gethostbyname(copy)))
#endif
			return 0;
		*(int *)&sadr.sin_addr = *(int *)h->h_addr_list[0];
	};

	SockadrToNetadr(&sadr, a);

	return true;
};

//=============================================================================

/*
====================
NET_Socket
====================
*/
int CNetworkSystem::IPSocket(char *net_interface, int port)
{
	int					newsocket;
	struct sockaddr_in	address;
	bool			_true = true;
	int					i = 1;
	int					err;

	if ((newsocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		err = WSAGetLastError();
		if (err != WSAEAFNOSUPPORT)
			Con_Printf ("WARNING: UDP_OpenSocket: socket: %s", NET_ErrorString());
		return 0;
	};

	// make it non-blocking
	if (ioctlsocket (newsocket, FIONBIO, &_true) == -1)
	{
		Con_Printf ("WARNING: UDP_OpenSocket: ioctl FIONBIO: %s\n", NET_ErrorString());
		return 0;
	};

	// make it broadcast capable
	if (setsockopt(newsocket, SOL_SOCKET, SO_BROADCAST, (char *)&i, sizeof(i)) == -1)
	{
		Con_Printf ("WARNING: UDP_OpenSocket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString());
		return 0;
	};

	if (!net_interface || !net_interface[0] || !stricmp(net_interface, "localhost"))
		address.sin_addr.s_addr = INADDR_ANY;
	else
		NET_StringToSockaddr (net_interface, (struct sockaddr *)&address);

	if (port == PORT_ANY)
		address.sin_port = 0;
	else
		address.sin_port = htons((short)port);

	address.sin_family = AF_INET;

	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
	{
		Con_Printf ("WARNING: UDP_OpenSocket: bind: %s\n", NET_ErrorString());
		closesocket (newsocket);
		return 0;
	};

	return newsocket;
};

/*
====================
NET_OpenIP
====================
*/
void CNetworkSystem::OpenIP()
{
	int		port;

	if (!ip_sockets[NS_SERVER])
	{
		port = ip_hostport.value;
		if (!port)
		{
			port = hostport.value;
			if (!port)
				port = Cvar_VariableValue("port"); // TODO: was port.value
		};
		ip_sockets[NS_SERVER] = NET_IPSocket (ip.string, port);
		if (!ip_sockets[NS_SERVER] && isDedicated)
			Host_Error (/*ERR_FATAL,*/ "Couldn't allocate dedicated server IP port");
	};

	// dedicated servers don't need client ports
	if (isDedicated)
		return;

	if (!ip_sockets[NS_CLIENT])
	{
		port = ip_clientport.value;
		if (!port)
		{
			port = clientport.value;
			if (!port)
				port = PORT_ANY;
		};
		ip_sockets[NS_CLIENT] = NET_IPSocket (ip.string, port);
		if (!ip_sockets[NS_CLIENT])
			ip_sockets[NS_CLIENT] = NET_IPSocket (ip.string, PORT_ANY);
	};
};

/*
====================
IPX_Socket
====================
*/
#ifdef _WIN32
int CNetworkSystem::IPXSocket (int port)
{
	int					newsocket;
	struct sockaddr_ipx	address;
	int					_true = 1;
	int					err;

	if ((newsocket = socket (PF_IPX, SOCK_DGRAM, NSPROTO_IPX)) == -1)
	{
		err = WSAGetLastError();
		if (err != WSAEAFNOSUPPORT)
			Con_Printf ("WARNING: IPX_Socket: socket: %s\n", NET_ErrorString());
		return 0;
	};

	// make it non-blocking
	if (ioctlsocket (newsocket, FIONBIO, &_true) == -1)
	{
		Con_Printf ("WARNING: IPX_Socket: ioctl FIONBIO: %s\n", NET_ErrorString());
		return 0;
	};

	// make it broadcast capable
	if (setsockopt(newsocket, SOL_SOCKET, SO_BROADCAST, (char *)&_true, sizeof(_true)) == -1)
	{
		Con_Printf ("WARNING: IPX_Socket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString());
		return 0;
	};

	address.sa_family = AF_IPX;
	memset (address.sa_netnum, 0, 4);
	memset (address.sa_nodenum, 0, 6);
	if (port == PORT_ANY)
		address.sa_socket = 0;
	else
		address.sa_socket = htons((short)port);

	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
	{
		Con_Printf ("WARNING: IPX_Socket: bind: %s\n", NET_ErrorString());
		closesocket (newsocket);
		return 0;
	};

	return newsocket;
};
#endif // _WIN32

/*
====================
NET_OpenIPX
====================
*/
void CNetworkSystem::OpenIPX ()
{
#ifdef _WIN32
	int		port;
	int		dedicated;

	dedicated = Cvar_VariableValue ("dedicated");

	if (!ipx_sockets[NS_SERVER])
	{
		port = ipx_hostport.value;
		if (!port)
		{
			port = hostport.value;
			if (!port)
				port = Cvar_VariableValue("port"); // TODO: was port.value
		};
		ipx_sockets[NS_SERVER] = NET_IPXSocket (port);
	};

	// dedicated servers don't need client ports
	if (dedicated)
		return;

	if (!ipx_sockets[NS_CLIENT])
	{
		port = ipx_clientport.value;
		if (!port)
		{
			port = clientport.value;
			if (!port)
				port = PORT_ANY;
		};
		ipx_sockets[NS_CLIENT] = NET_IPXSocket (port);
		if (!ipx_sockets[NS_CLIENT])
			ipx_sockets[NS_CLIENT] = NET_IPXSocket (PORT_ANY);
	};
#endif // _WIN32
};