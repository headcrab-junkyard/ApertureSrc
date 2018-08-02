/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2017-2018 BlackPhrase
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

#include "ClientGame.hpp"
#include "memory/IMemory.hpp"
#include "console/IConsole.hpp"
#include "engine/IEngineSound.hpp"
#include "input/IInput.hpp"
#include "InputEventListener.hpp"

EXPOSE_SINGLE_INTERFACE(CClientGame, IClientGame, MGT_CLIENTGAME_INTERFACE_VERSION);

CClientGame::CClientGame() = default;
CClientGame::~CClientGame() = default;

bool CClientGame::Init(CreateInterfaceFn afnEngineFactory)
{
	// NOTE: Put your init routines here...
	
	mpMemory = (IMemory*)afnEngineFactory(MGT_MEMORY_INTERFACE_VERSION, nullptr);
	
	mpConsole = (IConsole*)afnEngineFactory(MGT_CONSOLE_INTERFACE_VERSION, nullptr);
	
	mpSound = (IEngineSound*)afnEngineFactory(MGT_ENGINESOUNDCLIENT_INTERFACE_VERSION, nullptr);
	
	mpInput = (IInput*)afnEngineFactory(MGT_INPUT_INTERFACE_VERSION, nullptr);
	
	if(!mpMemory || !mpConsole || !mpSound || !mpInput)
		return false;
	
	// Plug our input events listener in
	mpInputEventListener = std::make_unique<CInputEventListener>();
	mpInput->AddEventListener(mpInputEventListener.get());
	
	return true;
};

void CClientGame::Shutdown()
{
	// NOTE: Put your shutdown routines here...
	
	mpInput->RemoveEventListener(mpInputEventListener.get());
};

void CClientGame::Frame()
{
};