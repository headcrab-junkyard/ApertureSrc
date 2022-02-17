/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2020-2021 BlackPhrase
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

void CGameServer::Init()
{
};

void CGameServer::Shutdown()
{
};

void CGameServer::Frame(float afTime)
{
};

void CGameServer::Activate()
{
	mpEventBroadcaster->OnServerActivate();
};

void CGameServer::Deactivate()
{
	mpEventBroadcaster->OnServerDeactivate();
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
};

void CGameServer::ReconnectAllClients()
{
	for(auto It : mvClients)
		It->Reconnect();
};

int CGameServer::GetActiveClientsNum() const
{
	int nCount{0};
	
	for(int i = 0; i < svs.maxclients; ++i)
		if(svs.clients[i].active)
			nCount++;
	
	return nCount;
};