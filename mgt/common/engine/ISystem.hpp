/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2015-2018 BlackPhrase
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
/// @brief system interface

#pragma once

#include "CommonTypes.hpp"

constexpr auto MGT_SYSTEM_INTERFACE_VERSION{"MGTSystem003Alpha"};

interface ISystemEventListener;

// IPlatform?
interface ISystem : public IBaseInterface
{
	///
	virtual void AddEventListener(ISystemEventListener *apListener) = 0;
	
	///
	virtual void RemoveEventListener(ISystemEventListener *apListener) = 0;
	
	///
	virtual void Printf(const char *asMsg, ...) = 0;
	
	///
	virtual void DevPrintf(const char *asMsg, ...) = 0;
	
	///
	virtual void SafePrintf(const char *asMsg, ...) = 0; // TODO: temp?
	
	///
	virtual void Warning(const char *asMsg, ...) = 0;
	
	/// An error will cause the entire program to exit
	virtual void Error(const char *asMsg, ...) = 0;
	
	///
	virtual void Quit() = 0;
	
	///
	virtual double GetFloatTime() const = 0;
};