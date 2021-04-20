/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2020 BlackPhrase
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

#include "CommonTypes.hpp"

constexpr auto OGS_CLIENTRESOURCEMANAGER_INTERFACE_VERSION{"OGSClientResourceManager001"};
constexpr auto OGS_SERVERRESOURCEMANAGER_INTERFACE_VERSION{"OGSServerResourceManager001"};

interface IResourceManager : public IBaseInterface
{
	// MODEL MANAGER
	
	///
	virtual int PrecacheModel(const char *asName) = 0;
	
	///
	virtual model_t *LoadModel(const char *name, int *index) = 0;
	
	///
	virtual model_t *GetModelByIndex(int nIndex) const = 0;
	
	//
	
	///
	virtual int PrecacheSound(const char *asName) = 0;
	
	///
	virtual uint GetApproxWavePlayLen(const char *sFilePath) const = 0;
	
	///
	virtual int PrecacheGeneric(const char *asName) = 0;
	
	///
	virtual int GetModelIndex(const char *asName) const = 0;
	
	///
	virtual int GetDecalIndex(const char *asName) const = 0;
};