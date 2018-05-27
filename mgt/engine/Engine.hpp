/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2017-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "quakedef.h" // TODO: host
#include "engine/IEngine.hpp"

struct IGame;

class CEngine final : public IEngine
{
public:
	CEngine();
	~CEngine();

	bool Init(const SInitData &apInitData) override;
	//bool Init(bool dedicated, const char *basedir, const char *cmdline) override;
	void Shutdown() override;

	bool Frame() override;

	void EndGame(const char *asMsg, ...); // TODO: wtf? move to game manager or something...
private:
	bool InitCore();
	bool InitClient();

	bool InitGame(const char *asName);
	void ShutdownGame();

	//void NewGame(const tString &asMapName, bool abLoadGame);
	//void EndGame(const tString &asMessage, ...);

	//bool IsLocalGame() const;

	// Get info about current game running - get interface with getters
	//IGameInfo *GetCurrentGameInfo() const; //or TGameInfo - get struct

	//void ShutdownServer(bool abCrash);

	//void WriteServerConfig(const char *asName);

	quakeparms_t mParms{};

	//void *mpCoreLib{nullptr}; // mpEngineCoreLib
	//IEngineCore *mpCore{nullptr};

	//void *mpClientLib{nullptr}; // mpEngineClientLib
	//IEngineClient *mpClient{nullptr};

	// temp support for game dll
	//void *mpGameLib{nullptr};
	//IGame *mpGame{nullptr};
};