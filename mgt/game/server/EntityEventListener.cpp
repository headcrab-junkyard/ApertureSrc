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

#include "EntityEventListener.hpp"
//#include "debug/Debug.hpp"

CEntityEventListener::CEntityEventListener()
{
	//LogMsg(eMsgType_Info, "Constructing the entity listener component...");
};

CEntityEventListener::~CEntityEventListener()
{
	//LogMsg(eMsgType_Info, "Destructing the entity listener component...");
};

int CEntityEventListener::OnEntitySpawn(/*IEntity*/ edict_t *apEntity)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntitySpawn");
	return 0;
};

void CEntityEventListener::OnEntityThink(/*IEntity*/ edict_t *apEntity)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntityThink"); // TODO: with 900 or more entities in the world it would be hard...
};

void CEntityEventListener::OnEntityUse(/*IEntity*/ edict_t *apEntity, /*IEntity*/ edict_t *apUsedBy)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntityUse");
};

void CEntityEventListener::OnEntityTouch(edict_t *pent, edict_t *pother)
{
};

void CEntityEventListener::OnEntityBlocked(edict_t *pent, edict_t *pother)
{
};

/*
void CEntityEventListener::EntityKeyValue(IEntity *apEntity, KeyValueData *apKeyValueData)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::EntityKeyValue");
};
*/

void CEntityEventListener::OnEntitySave(/*IEntity*/ edict_t *apEntity, SAVERESTOREDATA *apSaveData)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntitySave");
};

int CEntityEventListener::OnEntityRestore(/*IEntity*/ edict_t *apEntity, SAVERESTOREDATA *apSaveData, int anGlobalEntity)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntityRestore");
	return 0;
};

void CEntityEventListener::OnFreeEntPrivateData(/*IEntity*/ edict_t *apEntity)
{
	//DevMsg(eMsgType_Info, "[TRACE] CEntityEventListener::OnEntityFreePrivateData");
};