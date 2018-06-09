/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2018 BlackPhrase
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

#include <memory>
#include "game/server/IGame.hpp"

interface ISystem;
class CSystemEventListener;

class CGame final : public IGame
{
public:
	CGame() = default;
	~CGame() = default;
	
	bool Init(CreateInterfaceFn afnEngineFactory) override; // DLLInit
	void Shutdown() override; // DLLShutdown
	
	//void PostInit();
	
	//bool GameInit();
	//void GameShutdown();
	
	//void OnServerActivate(edict_t *apEdictList, uint anEdictCount, uint anMaxPlayers);
	//void OnServerDeactivate();
	
	//bool ShouldHideServer();
	
	void Update() override; // Frame
	
	const char *GetDescription() const override;
private:
	void GameInit();
	void RegisterEncoders();
	
	std::unique_ptr<CSystemEventListener> mpSystemEventListener;
	
	ISystem *mpSystem{nullptr};
};