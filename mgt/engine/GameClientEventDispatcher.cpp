/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
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

bool CGameClientEventDispatcher::DispatchConnect(edict_t *pEntity, const char *name, const char *adr, char *sRejectReason[128])
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		if(!It->OnClientConnect(pEntity, name, adr, sRejectReason))
			return false;
	
	// TODO: Post
	
	return true; // TODO
};

void CGameClientEventDispatcher::DispatchDisconnect(edict_t *pclent)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientDisconnect(pclent);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchKill(edict_t *pclent)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientKill(pclent);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchPutInServer(edict_t *pclent)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientPutInServer(pclent);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchCommand(edict_t *pclent)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientCommand(pclent);
	
	// TODO: Post
};

void CGameClientEventDispatcher::DispatchUserInfoChange(edict_t *pclent, char *userinfo)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnClientUserInfoChanged(pclent, userinfo);
	
	// TODO: Post
};