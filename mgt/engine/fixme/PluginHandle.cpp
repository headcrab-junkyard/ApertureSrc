/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2016-2018 BlackPhrase
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

#include "PluginHandle.hpp"
#include "common/LibUtil.hpp"
#include "core/IEnginePlugin.hpp"
//#include "core/PluginInfo.hpp"

CPluginHandle::~CPluginHandle()
{
	Unload();
};

bool CPluginHandle::LoadFromFile(const tString &asPath)
{
	msPrevPath = asPath;

	if(!(mpHandle = LibUtil::LoadLibrary(asPath.c_str())))
		return false;

	CreateInterfaceFn fnCreateInterface = nullptr; //Sys_GetFactory((CSysModule *)mpHandle);
	
	if(fnCreateInterface)
	{
		mpPlugin = (IEnginePlugin *)fnCreateInterface(MGT_ENGINEPLUGIN_INTERFACE_VERSION, nullptr);
		
		if(!mpPlugin)
			return false;

		if(!mpPlugin->Load(mfnModuleFactory))
			return false;

		mpPlugin->GetInfo(mInfo);
	};
	
	meState = State::Working;
	return true;
};

void CPluginHandle::Unload()
{
	if(mpPlugin)
		mpPlugin->Unload();

	mpPlugin = nullptr;

	if(mpHandle)
		LibUtil::FreeLibrary(mpHandle);

	mpHandle = nullptr;
};

bool CPluginHandle::Reload()
{
	meState = State::Reloading;
	
	Unload();

	if(LoadFromFile(msPrevPath))
		return true;
	
	return false;
};