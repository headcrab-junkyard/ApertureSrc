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

CGameClientEventDispatcher::CGameClientEventDispatcher() = default;

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

bool CGameClientEventDispatcher::ClientConnect(edict_t *pEntity, const char *name, const char *adr, char *sRejectReason[128])
{
	for(auto It : mlstListeners)
		if(!It->OnClientConnect(pEntity, name, adr, sRejectReason))
			return false;
	
	return true; // TODO
};

void CGameClientEventDispatcher::ClientDisconnect(edict_t *pclent)
{
	for(auto It : mlstListeners)
		It->OnClientDisconnect(pclent);
};

void CGameClientEventDispatcher::ClientKill(edict_t *pclent)
{
	for(auto It : mlstListeners)
		It->OnClientKill(pclent);
};

void CGameClientEventDispatcher::ClientPutInServer(edict_t *pclent)
{
	for(auto It : mlstListeners)
		It->OnClientPutInServer(pclent);
};

void CGameClientEventDispatcher::ClientCommand(edict_t *pclent)
{
	for(auto It : mlstListeners)
		It->OnClientCommand(pclent);
};

void CGameClientEventDispatcher::ClientUserInfoChanged(edict_t *pclent, char *userinfo)
{
	for(auto It : mlstListeners)
		It->OnClientUserInfoChanged(pclent, userinfo);
};