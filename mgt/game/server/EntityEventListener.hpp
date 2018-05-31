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

#include "engine/IEntityEventListener.hpp"

class CEntityEventListener final : public IEntityEventListener
{
public:
	CEntityEventListener();
	~CEntityEventListener();
	
	void Release() override {delete this;}

	int OnEntitySpawn(/*IEntity*/ edict_t *apEntity) override;
	void OnEntityThink(/*IEntity*/ edict_t *apEntity) override;
	void OnEntityUse(/*IEntity*/ edict_t *apEntity, /*IEntity*/ edict_t *apUsedBy) override;
	void OnEntityTouch(edict_t *pent, edict_t *pother) override;
	void OnEntityBlocked(edict_t *pent, edict_t *pother) override;

	//void EntityKeyValue(/*IEntity*/ edict_t *apEntity, KeyValueData *apKeyValueData) override;

	void OnEntitySave(/*IEntity*/ edict_t *apEntity, SAVERESTOREDATA *apSaveData) override;
	int OnEntityRestore(/*IEntity*/ edict_t *apEntity, SAVERESTOREDATA *apSaveData, int anGlobalEntity) override;

	void OnFreeEntPrivateData(/*IEntity*/ edict_t *apEntity) override; // OnEntityFreePrivateData
};