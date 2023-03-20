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

#pragma once

#include <list>
#include "CommonTypes.hpp"
//#include "engine/IEntityEventDispatcher.hpp"

interface IEntityEventListener;
using tEntityEventListenerList = std::list<IEntityEventListener*>; // TODO: use vector?

// TODO: temp
struct SAVERESTOREDATA
{
};

class CEntityEventDispatcher final
{
public:
	CEntityEventDispatcher();
	~CEntityEventDispatcher();
	
	void AddListener(IEntityEventListener *apListener);
	void RemoveListener(IEntityEventListener *apListener);
	
	int DispatchSpawn(edict_t *pent);
	void DispatchThink(edict_t *pent);
	void DispatchUse(edict_t *pent, edict_t *pother);
	void DispatchTouch(edict_t *pent, edict_t *pother);
	void DispatchBlocked(edict_t *pent, edict_t *pother);
	
	void DispatchSave(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData);
	int DispatchRestore(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, int globalentity);
	
	void DispatchFreePrivateData(edict_t *pent);
private:
	tEntityEventListenerList mlstListeners;
};