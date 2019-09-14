/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
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

#include "engine/IGameClient.hpp"

class CGameClient final : public IGameClient
{
public:
	CGameClient();
	~CGameClient();

	void Print(const char *asMsg); // TODO
	void CenterPrint(const char *asMsg); // TODO
	void SendStuffCmd(const char *asCmd); // TODO
	
	void Printf(PrintType aeType, const char *asMsg, ...) override;
	
	void SendCommand(const char *asText, ...) override;
	
	void Disconnect(const char *asReason, ...) override;

	int GetUserID() const override;
	uint GetWONID() const override;
	const char *GetAuthID() const override;
	
	void QueryCvarValue(const char *asCvarName) override;
	void QueryCvarValueEx(const char *asCvarName, int anRequestID) override;
	
	void SetViewEntity(const IEntity *apViewEnt) override {mpViewEnt = apViewEnt;}
	
	IEntity *GetEntity() const override {return mpEntity;}
private:
	IEntity *mpEntity{nullptr};
	const IEntity *mpViewEnt{nullptr};
	
	client_t *mpData{nullptr}; // TODO: temp?
};