/*
*	This file is part of Magenta Engine
*
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

/// @file
/// @brief

#include "quakedef.h"

netadr_t net_from;
sizebuf_t net_message;

void NET_Init()
{
};

void NET_Shutdown()
{
};

bool NET_GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
	return true;
};

void NET_SendPacket(netsrc_t sock, int length, void *data, netadr_t to)
{
};

bool NET_CompareAdr(netadr_t a, netadr_t b)
{
	return true;
};

char *NET_AdrToString(netadr_t a)
{
	return nullptr;
};

bool NET_StringToAdr(char *s, netadr_t *a)
{
	return true;
};