/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2020 BlackPhrase
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
/// @brief engine interface

#pragma once

#include "CommonTypes.hpp"
#include "interface.h"

constexpr auto OGS_ENGINE_INTERFACE_VERSION{"OGSEngine005"};

interface IEngine : public IBaseInterface
{
	struct InitParams
	{
		CreateInterfaceFn fnLauncherFactory{nullptr}; ///< Factory function from launcher module to get interfaces from it
		
		const char *sGameDir{""}; /*(TODO: basedir?)*/ ///< Game folder to load resources and game module from
		const char *sCmdLine{""}; ///< Passed command line arguments string
		
		char *sPostRestartCmdLine{nullptr}; ///< This string will be executed after the engine restart
		
		bool bDedicated{false}; ///< Set this to true to run in dedicated mode (without a local client support)
		
		//void *pWindowPtr{nullptr}; ///< Custom window pointer
		//bool bEditorMode{false}; ///< Initialize in editor mode
	};
	
	/**
	* Initialize the engine
	*
	* @param aInitData - passed read-only struct that contains specified init settings
	* @return true if successfully (or already) initialized
	*/
	virtual bool Init(const InitParams &apInitParams) = 0;
	
	/// Shutdown the engine
	virtual void Shutdown() = 0;
	
	/// Run a single frame
	/// @return false if doesn't want to update anymore
	virtual bool Frame() = 0;
	
	/// Call Frame method in loop (can't be updated manually)
	//virtual void Run() = 0;
	
	/// Mark engine for quitting
	//virtual void Stop() = 0;
	
	/**
	* Can't run or shutdown before init
	*
	* @return true if core is already initialized
	*/
	//virtual bool IsInitialized() const = 0;
};