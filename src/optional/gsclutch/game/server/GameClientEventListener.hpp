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

#pragma once

#include <next/engine/IGameClientEventListener.hpp>

class CBaseGame;

class CGameClientEventListener final : public IGameClientEventListener
{
public:
	CGameClientEventListener(CBaseGame *apGame);
	
	bool OnClientConnect(int anClientID, const char *asName, const char *asNetAdr, char asRejectReason[128]) override;
	void OnClientDisconnect(int anClientID) override;
	
	void OnClientPutInServer(int anClientID) override;
	
	void OnClientCommand(int anClientID, const ICmdArgs &aArgs) override;
	
	void OnClientUserInfoChanged(int anClientID, char *asOldUserInfo) override;
	
	int OnInconsistentFileFound(int anClientID, const char *asFileName, char *asDisconnectMsg) override;
	
	void OnCvarValueReceived(int anClientID, int anRequestID, const char *asCvarName, const char *asCvarValue) override;
private:
	void OnClientKill(int anClientID);
	
	edict_t *GetEntityForPlayer(int anClientID) const;
private:
	CBaseGame *mpGame{nullptr};
};