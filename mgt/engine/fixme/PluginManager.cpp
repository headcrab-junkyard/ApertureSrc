/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include <memory>
#include "PluginManager.hpp"
#include "PluginHandle.hpp"
//#include "debug/Debug.hpp"
#include "common/LibUtil.hpp"
#include "common/ArchTypes.hpp"
#include "common/PlatformTypes.hpp"
#include "common/StringList.hpp"
#include "filesystem/IFileSystem.hpp"

// TODO: move somewhere else
#if MGT_PLATFORM == MGT_PLATFORM_WINDOWS
	constexpr auto PLUGINDLL_EXT{".dll"};
#elif MGT_PLATFORM == MGT_PLATFORM_LINUX
	constexpr auto PLUGINDLL_EXT{".so"};
#elif MGT_PLATFORM == MGT_PLATFORM_MACOSX
	constexpr auto PLUGINDLL_EXT{".dlsym"};
#endif

CPluginManager::~CPluginManager()
{
	UnloadAll();
};

bool CPluginManager::LoadFromFolder(const tString &asPath, IFileSystem *apFileSystem)
{
	TStringList lstPlugins;
	apFileSystem->GetFilesInDirectory(asPath.c_str(), PLUGINDLL_EXT, lstPlugins);
	
	auto It = lstPlugins.mpHead;
	
	if(!It)
		return false;
	
	while(It)
	{
		LoadPlugin(tString(It->msString));
		It = It->mpNext;
	};
	
	return true;
};

CPluginHandle *CPluginManager::LoadPlugin(const tString &asPath)
{
	std::unique_ptr<CPluginHandle> pPlugin{std::make_unique<CPluginHandle>(mfnModuleFactory)};

	//DevMsg("Plugin Handler", "Loading a plugin from: %s", It.c_str());

	if(!pPlugin->LoadFromFile(asPath))
	{
		//DevError("Plugin Handler", "Failed to load a plugin... (%s)", It.c_str());
		//pPlugin.release();
		return nullptr;
	};
	
	//DevMsg("Plugin Handler", "Loaded plugin info:\n-Plugin Name: %s\n-Plugin Version: %s\n-Plugin Author: %s", pPlugin->GetInfo()->msName, pPlugin->GetInfo()->msVersion, pPlugin->GetInfo()->msAuthor);
	
	auto Plugin = pPlugin.release();
	mvPlugins.push_back(Plugin);
	return Plugin;
};

void CPluginManager::UnloadAll()
{
	//LogMsg("Plugin Handler", "Unloading all plugins...");

	for(auto && It : mvPlugins) // use weak_ptr here?
		if(It)                 // could be invalid...
			It->Unload();

	mvPlugins.clear();
};