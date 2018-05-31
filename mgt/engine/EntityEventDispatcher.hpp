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

#pragma once

#include <list>
//#include "engine/IEntityEventDispatcher.hpp"

interface IEntityEventListener;
using tEntityEventListenerList = std::list<IEntityEventListener*>;

class CEntityEventDispatcher final
{
public:
	CEntityEventDispatcher();
	~CEntityEventDispatcher();
	
	void AddListener(IEntityEventListener *apListener);
	void RemoveListener(IEntityEventListener *apListener);
	
	int EntitySpawn(edict_t *pent);
	void EntityThink(edict_t *pent);
	void EntityUse(edict_t *pent, edict_t *pother);
	void EntityTouch(edict_t *pent, edict_t *pother);
	void EntityBlocked(edict_t *pent, edict_t *pother);
	
	void EntitySave(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData);
	int EntityRestore(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, int globalentity);
	
	void EntityFreePrivateData(edict_t *pent);
private:
	tEntityEventListenerList mlstListeners;
};