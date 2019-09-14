/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
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

#include "quakedef.h"
#include "ModelManager.hpp"

EXPOSE_SINGLE_INTERFACE(CModelManager, IModelManager, MGT_MODELMANAGER_INTERFACE_VERSION);

CModelManager::CModelManager() = default;
CModelManager::~CModelManager() = default;

void CModelManager::TouchModel(const char *name)
{
	Mod_TouchModel(name);
};

model_t *CModelManager::GetByName(const char *name)
{
	return Mod_ForName(name);
};