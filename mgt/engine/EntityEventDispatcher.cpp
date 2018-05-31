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

#include "EntityEventDispatcher.hpp"
#include "engine/IEntityEventListener.hpp"

CEntityEventDispatcher::CEntityEventDispatcher() = default;

CEntityEventDispatcher::~CEntityEventDispatcher()
{
	for(auto It : mlstListeners)
	{
		It->Release();
		// TODO
	};
}:

void CEntityEventDispatcher::AddListener(IEntityEventListener *apListener)
{
	if(!apListener)
		return;
	
	mlstListeners.push_back(apListener);
};

void CEntityEventDispatcher::RemoveListener(IEntityEventListener *apListener)
{
	if(!apListener)
		return;
	
	//mlstListeners.erase(apListener);
};

int CEntityEventDispatcher::EntitySpawn(edict_t *pent)
{
	for(auto It : mlstListeners)
		It->OnEntitySpawn(pent);
	
	return -1; // TODO
};

void CEntityEventDispatcher::EntityThink(edict_t *pent)
{
	for(auto It : mlstListeners)
		It->OnEntityThink(pent);
};

void CEntityEventDispatcher::EntityUse(edict_t *pent, edict_t *pother)
{
	for(auto It : mlstListeners)
		It->OnEntityUse(pent, other);
};

void CEntityEventDispatcher::EntityTouch(edict_t *pent, edict_t *pother)
{
	for(auto It : mlstListeners)
		It->OnEntityTouch(pent, pother);
};

void CEntityEventDispatcher::EntityBlocked(edict_t *pent, edict_t *pother)
{
	for(auto It : mlstListeners)
		It->OnEntityBlocked(pent, pother);
};

void CEntityEventDispatcher::EntitySave(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData)
{
	for(auto It : mlstListeners)
		It->OnEntitySave(pent, pSaveRestoreData);
};

int CEntityEventDispatcher::EntityRestore(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, int globalentity)
{
	for(auto It : mlstListeners)
		It->OnEntityRestore(pent, pSaveRestoreData, globalentity);
};

void CEntityEventDispatcher::EntityFreePrivateData(edict_t *pent)
{
	for(auto It : mlstListeners)
		It->OnEntityFreePrivateData(pent);
};