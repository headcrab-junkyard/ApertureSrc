/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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
/// @brief entity event listener interface

#pragma once

#include "CommonTypes.hpp"

/*
struct SEntityEvent
{
	enum class Type : int
	{
		Spawn,
		Spawn_Post,
		Think,
		Think_Post,
		Use,
		Use_Post,
		Touch,
		Touch_Post,
		Blocked,
		Blocked_Post,
		Save,
		Save_Post,
		Restore,
		Restore_Post,
		//FreePrivateData,
		//FreePrivateData_Post
	};
	
	Type meType{};
	IEntity *mpEntity{nullptr};
};

struct SEntityUseEvent : SEntityEvent
{
	IEntity *mpUser{nullptr};
};

struct SEntityTouchEvent : SEntityEvent
{
	IEntity *mpOther{nullptr};
};

struct SEntityBlockedEvent : SEntityEvent
{
	IEntity *mpOther{nullptr};
};

struct SEntitySaveEvent : SEntityEvent
{
	SAVERESTOREDATA *mpData{nullptr};
};

struct SEntityRestoreEvent : SEntityEvent
{
	SAVERESTOREDATA *mpData{nullptr};
	int mnGlobalEntity{0};
};
*/

interface IEntityEventListener
{
	///
	virtual void Release() = 0;
	
	///
	virtual int OnEntitySpawn(edict_t *pent, bool abPost) = 0;
	
	///
	virtual void OnEntityThink(edict_t *pent, bool abPost) = 0;
	
	///
	virtual void OnEntityUse(edict_t *pent, edict_t *pother, bool abPost) = 0;
	
	///
	virtual void OnEntityTouch(edict_t *pent, edict_t *pother, bool abPost) = 0;
	
	///
	virtual void OnEntityBlocked(edict_t *pent, edict_t *pother, bool abPost) = 0;
	
	///
	virtual void OnEntitySave(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, bool abPost) = 0;
	
	///
	virtual int OnEntityRestore(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, int nGlobalEntity, bool abPost) = 0;
	
	///
	virtual void OnFreeEntPrivateData(edict_t *pent, bool abPost) = 0;
};