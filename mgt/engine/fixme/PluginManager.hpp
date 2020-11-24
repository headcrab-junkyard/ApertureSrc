/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2015-2019 BlackPhrase
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
/// @brief plugin manager component

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

class CPluginList;

class CPluginHandle;
using tPluginHandleVec = std::vector<CPluginHandle*>;

struct IFileSystem;

class CPluginManager final
{
public:
	CPluginManager(CreateInterfaceFn afnModuleFactory)
		: mfnModuleFactory(afnModuleFactory){}
	~CPluginManager();

	bool LoadFromFolder(const tString &asPath, IFileSystem *apFileSystem);
	bool LoadFromList(CPluginList &apList);
	CPluginHandle *LoadPlugin(const tString &asPath);

	void UnloadAll();

	CPluginHandle *GetByName(const tString &asName) const;
	CPluginHandle *GetByIndex(size_t anID) const;
private:
	tPluginHandleVec mvPlugins;

	CreateInterfaceFn mfnModuleFactory{nullptr};
};