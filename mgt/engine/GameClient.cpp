/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2017-2019 BlackPhrase
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

#include "quakedef.h"
#include "GameClient.hpp"

CGameClient::CGameClient() = default;
CGameClient::~CGameClient() = default;

/*
=================
PF_sprint

single print to a specific client

sprint(clientent, value)
=================
*/
void CGameClient::Print(const char *s)
{
	auto entnum{NUM_FOR_EDICT(mpData->edict)};

	if(entnum < 1 || entnum > svs.maxclients)
	{
		gpSystem->Printf("tried to sprint to a non-client\n");
		return;
	};

	mpData = &svs.clients[entnum - 1];

	MSG_WriteChar(&mpData->netchan.message, svc_print);
	MSG_WriteString(&mpData->netchan.message, s);
};

/*
=================
PF_centerprint

single print to a specific client

centerprint(clientent, value)
=================
*/
void CGameClient::CenterPrint(const char *s)
{
	auto entnum{NUM_FOR_EDICT(mpData->edict)};

	if(entnum < 1 || entnum > svs.maxclients)
	{
		gpSystem->Printf("tried to sprint to a non-client\n");
		return;
	};

	mpData = &svs.clients[entnum - 1]; // TODO: WUT?

	MSG_WriteChar(&mpData->netchan.message, svc_centerprint);
	MSG_WriteString(&mpData->netchan.message, s);
};

/*
=================
PF_stuffcmd

Sends text over to the client's execution buffer

stuffcmd (clientent, value)
=================
*/
void CGameClient::SendStuffCmd(const char *str)
{
	auto entnum{NUM_FOR_EDICT(mpData->edict)};
	
	if(entnum < 1 || entnum > svs.maxclients)
		Host_Error("Parm 0 not a client"); // TODO

	Host_ClientCommands(mpData, "%s", str); // TODO: Host_SendClientCmd
};

void CGameClient::Printf(PrintType aeType, const char *asMsg, ...)
{
	// TODO
};

void CGameClient::SendCommand(const char *asText, ...)
{
	// TODO
};

void CGameClient::Disconnect(const char *asReason, ...)
{
	// TODO
};

int CGameClient::GetUserID() const
{
	return -1; // TODO
};

uint CGameClient::GetWONID() const
{
	return 0; // TODO
};

const char *CGameClient::GetAuthID() const
{
	return ""; // TODO
};

void CGameClient::QueryCvarValue(const char *asCvarName)
{
	// TODO
};

void CGameClient::QueryCvarValueEx(const char *asCvarName, int anRequestID)
{
	// TODO
};