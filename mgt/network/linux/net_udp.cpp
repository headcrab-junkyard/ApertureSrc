/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <errno.h>

#if defined(sun)
#include <unistd.h>
#endif

#ifdef sun
#include <sys/filio.h>
#endif

#ifdef NeXT
#include <libc.h>
#endif

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

int UDP_OpenSocket(int port)
{
	int newsocket;
	struct sockaddr_in address;
	bool _true = true;
	int i;

	if((newsocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		Sys_Error("UDP_OpenSocket: socket:", strerror(errno));
	if(ioctl(newsocket, FIONBIO, (char *)&_true) == -1)
		Sys_Error("UDP_OpenSocket: ioctl FIONBIO:", strerror(errno));
	address.sin_family = AF_INET;
	//ZOID -- check for interface binding option
	if((i = COM_CheckParm("-ip")) != 0 && i < com_argc)
	{
		address.sin_addr.s_addr = inet_addr(com_argv[i + 1]);
		Con_Printf("Binding to IP Interface Address of %s\n",
		           inet_ntoa(address.sin_addr));
	}
	else
		address.sin_addr.s_addr = INADDR_ANY;
	if(port == PORT_ANY)
		address.sin_port = 0;
	else
		address.sin_port = htons((short)port);
	if(bind(newsocket, (void *)&address, sizeof(address)) == -1)
		Sys_Error("UDP_OpenSocket: bind: %s", strerror(errno));

	return newsocket;
}

void NET_GetLocalAddress()
{
	char buff[MAXHOSTNAMELEN];

	gethostname(buff, MAXHOSTNAMELEN);
	buff[MAXHOSTNAMELEN - 1] = 0;

	NET_StringToAdr(buff, &net_local_adr);

	namelen = sizeof(address);
	if(getsockname(net_socket, (struct sockaddr *)&address, &namelen) == -1)
		Sys_Error("NET_Init: getsockname:", strerror(errno));
}