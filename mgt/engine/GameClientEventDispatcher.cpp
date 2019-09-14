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

#include "GameClientEventDispatcher.hpp"
#include "engine/IGameClientEventListener.hpp"

// TODO: temp
CGameClientEventDispatcher *gpGameClientEventDispatcher{nullptr};

CGameClientEventDispatcher::CGameClientEventDispatcher() //= default;
{
	gpGameClientEventDispatcher = this; // TODO
};

CGameClientEventDispatcher::~CGameClientEventDispatcher()
{
	for(auto It : mlstListeners)
	{
		It->Release();
		// TODO
	};
};

void CGameClientEventDispatcher::AddListener(IGameClientEventListener *apListener)
{
	if(!apListener)
		return;
	
	mlstListeners.push_back(apListener);
};

void CGameClientEventDispatcher::RemoveListener(IGameClientEventListener *apListener)
{
	if(!apListener)
		return;
	
	//mlstListeners.erase(apListener);
};

bool CGameClientEventDispatcher::DispatchConnect(int anClientID, const char *name, const char *adr, char sRejectReason[128])
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		if(!It->OnClientConnect(anClientID, name, adr, sRejectReason))
			return false;
	
	// TODO: Post
	
	return true; // TODO
};

void CGameClientEventDispatcher::DispatchDisconnect(int anClientID)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientDisconnect(anClientID);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchKill(int anClientID)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientKill(anClientID);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchPutInServer(int anClientID)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientPutInServer(anClientID);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchCommand(int anClientID)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientCommand(anClientID);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchUserInfoChange(int anClientID, char *userinfo)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientUserInfoChanged(anClientID, userinfo);
	
	// TODO: Post
};