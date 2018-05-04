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
/// @brief direct (low level) engine core interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_ENGINECORE_INTERFACE_VERSION{"MGTEngineCore003Alpha"};

struct IEngineEventListener;
struct IEnginePlugin;

interface IEngineCore : public IBaseInterface
{
	struct TInitData
	{
		CreateInterfaceFn fnLauncherFactory{nullptr}; ///< Factory function from launcher module to get interfaces from it
		
		//const char *sGameFolder{""};	///< Game folder to load resources and game module from
		const char *sCmdLine{""};		///< Passed command line
		
		//bool bDedicated{false}; ///< Set to true to run in dedicated mode
	};

	/**
	* Initialize the engine
	*
	* @param aInitData - passed read-only struct that contains specified init settings
	* @return true if successfully (or already) initialized
	*/
	virtual bool Init(const TInitData &aInitData) = 0;
	
	/// Shutdown the engine
	virtual void Shutdown() = 0;
	
	/**
	* Run a single frame
	* Manual way to update the engine
	*
	* @return true if ???
	*/
	virtual bool Frame() = 0;
	
	///
	//virtual void DevLog(const char *asMsg, ...) = 0;
	
	/**
	* Can't run or shutdown before init
	*
	* @return true if core is already initialized
	*/
	virtual bool IsInitialized() const = 0;
	
	//virtual bool AddPluginToInit(const char *asPath) const = 0;
	//virtual ModuleFactoryFn LoadPlugin(const char *asPath/*, bool abRequired*/) const = 0;
	//virtual ModuleFactoryFn GetPlugin(const char *asName) const = 0;
	
	/**
	*/
	virtual bool AddListener(IEngineEventListener *apListener) = 0;
	virtual void RemoveListener(IEngineEventListener *apListener) = 0;
	
	//virtual void GetEngineSettings(TEngineSettings &apSettings) = 0;
	
	// FPS Locking
	
	//virtual void SetFPSLock(int anFPS) = 0; // SetMaxFPS; -1 to unlock
	//virtual int GetFPSLock() = 0; // GetMaxFPS
	
	//virtual bool IsFPSLocked() = 0;
};