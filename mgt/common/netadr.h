/*
 * This file is part of OGS Engine
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "const.h"

typedef enum netadrtype_s
{
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
	NA_IPX,
	NA_BROADCAST_IPX
} netadrtype_t;

typedef struct netadr_s
{
	netadrtype_t	type;
	
	byte	ip[4];
	byte	ipx[10];
	
	unsigned short	port;
	
	inline bool IsLocal() const {return type == NA_LOOPBACK;}
	
	inline bool operator==(const netadr_s &other) const
	{
		if(ip[0] == other.ip[0] && ip[1] == other.ip[1] && ip[2] == other.ip[2] && ip[3] == other.ip[3] && port == other.port)
			return true;
		return false;
	};
	
	inline bool CompareBase(const netadr_s &other) const
	{
		if(ip[0] == other.ip[0] && ip[1] == other.ip[1] && ip[2] == other.ip[2] && ip[3] == other.ip[3])
			return true;
		return false;
	};
	
	inline const char *ToString(bool abBaseOnly) const
	{
		static char s[64]{};
		if(abBaseOnly)
			sprintf(s, "%i.%i.%i.%i", ip[0], ip[1], ip[2], ip[3]);
		else
			sprintf(s, "%i.%i.%i.%i:%i", ip[0], ip[1], ip[2], ip[3], ntohs(port));
		return s;
	};
} netadr_t;