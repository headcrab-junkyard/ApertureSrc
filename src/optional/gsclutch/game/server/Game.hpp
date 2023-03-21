/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019, 2023 BlackPhrase
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

#include <memory>
#include <vector>

#include <next/game/server/IGame.hpp>
#include <next/engine/IGameServerEventListener.hpp>

interface ISystem;
interface ISystemEventListener;

interface IGameServer;
interface IGameClientEventListener;

class CGamePlayer;
using tGamePlayerVec = std::vector<CGamePlayer*>;

class CGame : public IGame, public IGameServerEventListener
{
public: // IGame interface methods
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	void Frame(double afFrameTime) override;
	
	const char *GetDescription() const override;
public: // IGameServerEventListener interface methods
	void OnServerActivate(int anMaxEntities, int anMaxClients) override;
	void OnServerDeactivate() override;
private:
	CGamePlayer *GetPlayer(int anClientID) const {return mvPlayers.at(anClientID);}
private:
	tGamePlayerVec mvPlayers;
	
	std::unique_ptr<ISystemEventListener> mpSystemEventListener;
	std::unique_ptr<IGameClientEventListener> mpGameClientEventListener;
	
	ISystem *mpSystem{nullptr};
	IGameServer *mpGameServer{nullptr};
};