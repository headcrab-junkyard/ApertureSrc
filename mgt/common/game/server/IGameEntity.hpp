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
/// @brief game entity interface

#pragma once

#include "CommonTypes.hpp"

interface IEntity;

interface IGameEntity
{
	///
	//virtual void SetAbsBox() = 0;
	
	///
	//virtual void KeyValueData(KeyValueData *pkvd) = 0;
	
	///
	virtual int OnSpawn() = 0;
	
	///
	virtual void OnThink() = 0;
	
	///
	virtual void OnUse(IEntity *apOther) = 0;
	
	///
	virtual void OnTouch(IEntity *apOther) = 0;
	
	///
	virtual void OnBlocked(IEntity *apOther) = 0;
	
	///
	virtual void OnSave(SAVERESTOREDATA *apSaveRestoreData) = 0;
	
	///
	virtual int OnRestore(SAVERESTOREDATA *apSaveRestoreData, int nGlobalEntity) = 0;
	
	///
	//virtual IEntity *GetEntity() const = 0;
};