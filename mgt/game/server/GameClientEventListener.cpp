/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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

#include "GameClientEventListener.hpp"

CGameClientEventListener::CGameClientEventListener() = default;
CGameClientEventListener::~CGameClientEventListener() = default;

bool CGameClientEventListener::OnClientConnect(edict_t *pEntity, const char *name, const char *adr, char *sRejectReason[128])
{
	// TODO
	return true;
};

void CGameClientEventListener::OnClientDisconnect(edict_t *pclent)
{
	// TODO
};

void CGameClientEventListener::OnClientKill(edict_t *pclent)
{
	// TODO
};

void CGameClientEventListener::OnClientPutInServer(edict_t *pclent)
{
	// TODO
};

void CGameClientEventListener::OnClientCommand(edict_t *pclent)
{
	// TODO
};

void CGameClientEventListener::OnClientUserInfoChanged(edict_t *pclent, char *userinfo)
{
	// TODO
};