/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2019, 2023 BlackPhrase
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

#include "EntityEventDispatcher.hpp"
#include "engine/IEntityEventListener.hpp"

// TODO: temp
CEntityEventDispatcher *gpEntityEventDispatcher{nullptr};

CEntityEventDispatcher::CEntityEventDispatcher() //= default;
{
	gpEntityEventDispatcher = this; // TODO
};

CEntityEventDispatcher::~CEntityEventDispatcher()
{
	for(auto It : mlstListeners)
	{
		It->Release();
		// TODO
	};
};

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

int CEntityEventDispatcher::DispatchSpawn(edict_t *apEntity)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntitySpawn(apEntity);
	
	// TODO: Post
	
	return -1; // TODO
};

void CEntityEventDispatcher::DispatchThink(edict_t *apEntity)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntityThink(apEntity);
	
	// TODO: Post
};

void CEntityEventDispatcher::DispatchUse(edict_t *apEntity, edict_t *pother)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntityUse(apEntity, pother);
	
	// TODO: Post
};

void CEntityEventDispatcher::DispatchTouch(edict_t *apEntity, edict_t *pother)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntityTouch(apEntity, pother);
	
	// TODO: Post
};

void CEntityEventDispatcher::DispatchBlocked(edict_t *apEntity, edict_t *pother)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntityBlocked(apEntity, pother);
	
	// TODO: Post
};

void CEntityEventDispatcher::DispatchSave(edict_t *apEntity, SAVERESTOREDATA *pSaveRestoreData)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntitySave(apEntity, pSaveRestoreData);
	
	// TODO: Post
};

int CEntityEventDispatcher::DispatchRestore(edict_t *apEntity, SAVERESTOREDATA *pSaveRestoreData, int globalentity)
{
	// TODO: Pre
	
	for(auto It : mlstListeners)
		It->OnEntityRestore(apEntity, pSaveRestoreData, globalentity);
	
	// TODO: Post
};

void CEntityEventDispatcher::DispatchFreePrivateData(edict_t *apEntity)
{
	// TODO: OnEntityFreePrivateData_Pre(apEntity);
	
	for(auto It : mlstListeners)
		It->OnFreeEntPrivateData(apEntity); // TODO: OnEntityFreePrivateData(apEntity);
	
	// TODO: OnEntityFreePrivateData_Post(apEntity);
};