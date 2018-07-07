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
/// @brief base game entity class

#pragma once

#include "game/server/IGameEntity.hpp"

typedef struct entvars_s entvars_t;

class CBaseEntity : public IGameEntity
{
public:
	CBaseEntity(IEntity *apEntity) = default;
	virtual ~CBaseEntity() = default;
	
	virtual int OnSpawn() override {}
	virtual void OnThink() override {}
	
	virtual void OnUse(IEntity *apOther) override {}
	
	virtual void OnTouch(IEntity *apOther) override {}
	virtual void OnBlocked(IEntity *apOther) override {}
	
	virtual void OnSave(SAVERESTOREDATA *apSaveRestoreData) override {}
	virtual int OnRestore(SAVERESTOREDATA *apSaveRestoreData, int nGlobalEntity) override {}
	
	const char *GetClassName() const;
	float GetHealth() const;
	
	//IEntity *GetEntity() const {return mpEntity;}
protected:
	IEntity *mpEntity{nullptr};
};