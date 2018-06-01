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

#pragma once

#include "IGameClientEventListener.hpp"

class CGameClientEventListener final : public IGameClientEventListener
{
public:
	CGameClientEventListener();
	~CGameClientEventListener();
	
	void Release() override {delete this;}
	
	bool OnClientConnect(edict_t *pEntity, const char *name, const char *adr, char *sRejectReason[128]) override;
	void OnClientDisconnect(edict_t *pclent) override;
	
	void OnClientKill(edict_t *pclent) override;
	
	void OnClientPutInServer(edict_t *pclent) override;
	
	void OnClientCommand(edict_t *pclent) override;
	
	void OnClientUserInfoChanged(edict_t *pclent, char *userinfo) override;
};