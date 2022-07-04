/// @file

#include "NetChannel.hpp"
#include "engine/ISystem.hpp"
#include "tier1/SizeBuffer.hpp"
#include "netadr.h"

#ifdef _WIN32
#	include <wsipx.h>
#endif

//=============================================================================

//void NetadrToSockadr (netadr_t *a, struct sockaddr *s) // TODO: windows q2
void NetadrToSockadr(const netadr_t *a, struct sockaddr_in *s)
{
	memset(s, 0, sizeof(*s));

	//*(int *)&s->sin_addr = *(int *)&a->ip;
	
	if (a->type == NA_BROADCAST)
	{
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_port = a->port;
		((struct sockaddr_in *)s)->sin_addr.s_addr = INADDR_BROADCAST;
	}
	else if (a->type == NA_IP)
	{
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_addr.s_addr = *(int *)&a->ip; // TODO: *(int *)&s->sin_addr = *(int *)&a->ip;
		((struct sockaddr_in *)s)->sin_port = a->port;
	}
#ifdef _WIN32
	else if (a->type == NA_IPX)
	{
		((struct sockaddr_ipx *)s)->sa_family = AF_IPX;
		memcpy(((struct sockaddr_ipx *)s)->sa_netnum, &a->ipx[0], 4);
		memcpy(((struct sockaddr_ipx *)s)->sa_nodenum, &a->ipx[4], 6);
		((struct sockaddr_ipx *)s)->sa_socket = a->port;
	}
	else if (a->type == NA_BROADCAST_IPX)
	{
		((struct sockaddr_ipx *)s)->sa_family = AF_IPX;
		memset(((struct sockaddr_ipx *)s)->sa_netnum, 0, 4);
		memset(((struct sockaddr_ipx *)s)->sa_nodenum, 0xff, 6);
		((struct sockaddr_ipx *)s)->sa_socket = a->port;
	};
#endif
};

//void SockadrToNetadr (struct sockaddr *s, netadr_t *a) // TODO: windows q2
void SockadrToNetadr(struct sockaddr/*_in*/ *s, netadr_t &a)
{
	if (s->sa_family == AF_INET)
	{
		a.type = NA_IP;
		*(int *)&a.ip = ((struct sockaddr_in *)s)->sin_addr.s_addr; // TODO: = *(int *)&s->sin_addr;
		a.port = ((struct sockaddr_in *)s)->sin_port;
	}
#ifdef _WIN32
	else if (s->sa_family == AF_IPX)
	{
		a.type = NA_IPX;
		memcpy(&a.ipx[0], ((struct sockaddr_ipx *)s)->sa_netnum, 4);
		memcpy(&a.ipx[4], ((struct sockaddr_ipx *)s)->sa_nodenum, 6);
		a.port = ((struct sockaddr_ipx *)s)->sa_socket;
	};
#endif
};

CNetChannel::CNetChannel(ISystem *apSystem) : mpSystem(apSystem){}

void CNetChannel::SendPacket(int length, const void *data, const netadr_t &to)
{	
	if(to.type == NA_LOOPBACK)
	{
		SendLoopPacket(length, data, to);
		return;
	};
	
	int net_socket;
	
	if(to.type == NA_BROADCAST)
	{
		net_socket = ip_socket;
		if(!net_socket)
			return;
	}
	else if (to.type == NA_IP)
	{
		net_socket = ip_socket;
		if(!net_socket)
			return;
	}
	else if(to.type == NA_IPX)
	{
		net_socket = ipx_socket;
		if(!net_socket)
			return;
	}
	else if(to.type == NA_BROADCAST_IPX)
	{
		net_socket = ipx_socket;
		if(!net_socket)
			return;
	}
	else
		mpSystem->Error(/*ERR_FATAL,*/ "NET_SendPacket: bad address type");
	
	struct sockaddr	addr; // TODO: sockaddr_in?
	
	NetadrToSockadr(&to, reinterpret_cast<struct sockaddr_in*>(&addr));
	
	int ret = sendto(net_socket, (const char*)data, length, 0, (struct sockaddr *)&addr, sizeof(addr));
	
	if(ret == -1)
	{
#ifdef _WIN32
		int err = WSAGetLastError();

		// wouldblock is silent
		if(err == WSAEWOULDBLOCK)
			return;

#ifndef SWDS
		if(err == WSAEADDRNOTAVAIL)
			mpSystem->DevPrintf("NET_SendPacket Warning: %i\n", err);
		else
#endif
			mpSystem->Printf("NET_SendPacket ERROR: %i\n", errno);
#elif __linux__
		if(errno == EWOULDBLOCK)
			return;
		if(errno == ECONNREFUSED)
			return;
		mpSystem->Printf("NET_SendPacket: %s\n", strerror(errno));
#endif
	};
};

bool CNetChannel::GetPacket(netadr_t &net_from, sizebuf_t &net_message)
{
	if(GetLoopPacket(net_from, net_message))
		return true;
	
	int ret;
	struct sockaddr from;
	int fromlen;
	int net_socket;
	int protocol;
	
#ifdef _WIN32
	for (protocol = 0 ; protocol < 2 ; protocol++)
	{
		if (protocol == 0)
			net_socket = ip_socket;
		else
			net_socket = ipx_socket;

		if (!net_socket)
			continue;
		
		fromlen = sizeof(from);
		ret = recvfrom(net_socket, (char *)net_message.data, sizeof(net_message.data), 0, (struct sockaddr *)&from, &fromlen);
		
		SockadrToNetadr(&from, net_from);

		if(ret == -1)
		{
			int err = WSAGetLastError();

			if(err == WSAEWOULDBLOCK)
				return false;
			if(err == WSAEMSGSIZE)
			{
				mpSystem->Printf("Warning:  Oversize packet from %s\n", net_from.ToString());
				return false;
			};

			mpSystem->Error("NET_GetPacket: %s", strerror(err));
		};
		
		net_message.cursize = ret;
		return true;
	};
#elif __linux__
	for (protocol = 0 ; protocol < 2 ; protocol++)
	{
		if (protocol == 0)
			net_socket = ip_socket;
		else
			net_socket = ipx_socket;

		if (!net_socket)
			continue;
		
		fromlen = sizeof(from);
		ret = recvfrom(net_socket, net_message.data, sizeof(net_message.data), 0, (struct sockaddr *)&from, &fromlen);
		
		if(ret == -1)
		{
			if(errno == EWOULDBLOCK)
				return false;
			if(errno == ECONNREFUSED)
				return false;
			mpSystem->Printf("NET_GetPacket: %s\n", strerror(errno));
			return false;
		};
	
		net_message.cursize = ret;
		return true;
	};
#endif
	
	return false;
};

void CNetChannel::SendMsg(const INetMsg &aMsg, const netadr_t &to)
{
};

bool CNetChannel::GetMsg(netadr_t &net_from, INetMsg &net_message)
{
	return false;
};

/*
=============================================================================

LOOPBACK BUFFERS FOR LOCAL PLAYER

=============================================================================
*/

void CNetChannel::SendLoopPacket(int length, const void *data, const netadr_t &to)
{
	loopback_t	*loop;

	loop = &mLoopBack; // [sock^1]; // TODO

	int i = loop->send & (MAX_LOOPBACK-1);
	loop->send++;

	memcpy (loop->msgs[i].data, data, length);
	loop->msgs[i].datalen = length;
};

bool CNetChannel::GetLoopPacket(netadr_t &net_from, sizebuf_t &net_message)
{
	loopback_t	*loop;

	loop = &mLoopBack;

	if (loop->send - loop->get > MAX_LOOPBACK)
		loop->get = loop->send - MAX_LOOPBACK;

	if (loop->get >= loop->send)
		return false;

	int i = loop->get & (MAX_LOOPBACK-1);
	loop->get++;

	memcpy (net_message.data, loop->msgs[i].data, loop->msgs[i].datalen);
	net_message.cursize = loop->msgs[i].datalen;

#ifdef _WIN32
	memset (&net_from, 0, sizeof(net_from));
	net_from.type = NA_LOOPBACK;
#else
	net_from = net_local_adr;
#endif

	return true;
};