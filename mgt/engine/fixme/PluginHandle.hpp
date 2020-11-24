/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2019 BlackPhrase
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
/// @brief representation of a remote plugin module

#pragma once

//#include "resources/IResource.hpp"
#include "common/CommonTypes.hpp"
#include "common/Interface.hpp"
#include "core/TPluginInfo.hpp"

struct IEnginePlugin;

class CPluginHandle final //: public IResource
{
public:
	CPluginHandle(CreateInterfaceFn afnModuleFactory)
		: mfnModuleFactory(afnModuleFactory){}
	~CPluginHandle();

	bool LoadFromFile(const tString &asPath);
	void Unload();
	bool Reload();

	/// Return plugin's info struct
	const TPluginInfo &GetInfo() const {return mInfo;}
	
	/// Get plugin callbacks (is it really needed?)
	//IEnginePlugin *GetCallbacks() const {return mpPlugin;}

	enum class State : int
	{
		Invalid = -1,
		Error = 0,
		Paused,
		Loading,
		Working,
		Reloading,
		Unloading
	};
	
	State GetState() const {return meState;}
private:
	void *mpHandle{nullptr};

	IEnginePlugin *mpPlugin{nullptr};
	TPluginInfo mInfo; // or use CPluginInfo class here
	//std::unique_ptr<CPluginInfo> mpInfo;

	tString msPrevPath{""};

	CreateInterfaceFn mfnModuleFactory{nullptr};

	State meState{State::Invalid};
};