/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2021 BlackPhrase
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
/// @brief system interface

#pragma once

#include "CommonTypes.hpp"
#include "tier1/interface.h"

constexpr auto OGS_SYSTEM_INTERFACE_VERSION{"OGSSystem003"};

interface ISystemEventListener;
interface ICmdArgs;

interface IRegistry
{
	///
	virtual bool SetString(const char *asKey, const char *asValue) = 0;
	
	///
	virtual bool GetString(const char *asKey, char *asValue, int anLen) const = 0;
	
	///
	virtual bool SetInteger(const char *asKey, int anValue) = 0;
	
	///
	virtual bool GetInteger(const char *asKey, int &value) const = 0;
	
	///
	virtual bool DeleteKey(const char *keyName) = 0;
};

// IPlatform?
interface ISystem : public IBaseInterface
{
	///
	virtual void AddEventListener(ISystemEventListener *apListener) = 0;
	
	///
	virtual void RemoveEventListener(ISystemEventListener *apListener) = 0;
	
	/// the system console is shown when a dedicated server is running
	//virtual void DisplaySystemConsole(bool abShow) = 0;

	///
	virtual void Printf(const char *asMsg, ...) = 0;
	
	///
	virtual void DevPrintf(const char *asMsg, ...) = 0;
	
	///
	//virtual void SafePrintf(const char *asMsg, ...) = 0; // TODO: temp?
	
	///
	virtual void Warning(const char *asMsg, ...) = 0;
	
	/// An error will cause the entire program to exit
	virtual void Error(const char *asMsg, ...) = 0;
	
	///
	virtual void Quit() = 0;
	
	///
	//virtual void ShellExecute(const char *asCmd, const char *asFile) = 0;
	
	///
	virtual double GetFloatTime() const = 0;
	
	///
	virtual double GetRealTime() const = 0;
	
	enum class PlatformType : int
	{
		Unknown = -1,
		
		Windows,
		Linux,
		MacOS,
		Android,
		Emscripten
	};
	
	struct Info
	{
		PlatformType mePlatform{PlatformType::Unknown};
		uint mnProcessorCount{0};
	};
	
	///
	//virtual void GetInfo(Info &apInfo) = 0;
	
	///
	//virtual IRegistry *GetRegistry() const = 0;
	
	///
	virtual ICmdArgs *GetStartupArgs() const = 0;
};