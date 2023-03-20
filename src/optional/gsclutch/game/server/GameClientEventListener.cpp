/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2023 BlackPhrase
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

#include "GameClientEventListener.hpp"
#include "BaseGame.hpp"

CGameClientEventListener::CGameClientEventListener(CBaseGame *apGame) : mpGame(apGame){}

bool CGameClientEventListener::OnClientConnect(int anClientID, const char *asName, const char *asNetAdr, char asRejectReason[128])
{
	return gEntityInterface.pfnClientConnect(GetEntityForPlayer(anClientID), asName, asNetAdr, asRejectReason);
};

void CGameClientEventListener::OnClientDisconnect(int anClientID)
{
	gEntityInterface.pfnClientDisconnect(GetEntityForPlayer(anClientID));
};

void CGameClientEventListener::OnClientPutInServer(int anClientID)
{
	gEntityInterface.pfnClientPutInServer(GetEntityForPlayer(anClientID));
};

void CGameClientEventListener::OnClientCommand(int anClientID, const ICmdArgs &aArgs)
{
	if(aArgs.GetByIndex(0) == "kill")
	{
		OnClientKill(anClientID);
		return;
	};
	
	gEntityInterface.pfnClientCommand(GetEntityForPlayer(anClientID), aArgs.ToString());
};

void CGameClientEventListener::OnClientUserInfoChanged(int anClientID, char *asOldUserInfo)
{
	gEntityInterface.pfnClientUserInfoChanged(GetEntityForPlayer(anClientID), mpGame->GetPlayer(anClientID)->GetUserInfo()->ToString());
};

int CGameClientEventListener::OnInconsistentFileFound(int anClientID, const char *asFileName, char *asDisconnectMsg)
{
	return gEntityInterface.pfnInconsistentFile(GetEntityForPlayer(anClientID), asFileName, asDisconnectMsg);
};

void CGameClientEventListener::OnCvarValueReceived(int anClientID, int anRequestID, const char *asCvarName, const char *asCvarValue)
{
	// If no request ID and no cvar name provided - interpret that as a call to pfnCvarValue
	// TODO: should the request id be 0 here?
	if(anRequestID == -1 && !asCvarName && gNewDLLFuncs.pfnCvarValue)
	{
		gNewDLLFuncs.pfnCvarValue(GetEntityForPlayer(anClientID), asCvarValue);
		return;
	};
	
	if(gNewDLLFuncs.pfnCvarValue2)
		gNewDLLFuncs.pfnCvarValue2(GetEntityForPlayer(anClientID), anRequestID, asCvarName, asCvarValue);
};

void CGameClientEventListener::OnClientKill(int anClientID)
{
	gEntityInterface.pfnClientKill(GetEntityForPlayer(anClientID));
};

edict_t *CGameClientEventListener::GetEntityForPlayer(int anClientID) const
{
	return mpGame->GetPlayer(anClientID)->GetEntity();
};