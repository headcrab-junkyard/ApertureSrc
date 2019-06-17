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
/// @brief input system module low-level interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

#ifdef _WIN32
#include <windef.h>
#endif

constexpr auto MGT_INPUTSYSTEM_INTERFACE_VERSION{"MGTInputSystem001Alpha"};

interface IInputEventListener;

interface IInputSystem : public IBaseInterface
{
	/// @param abConsoleTextMode - init in console text mode (skip gamepads support)
	virtual bool Init(bool abConsoleTextMode) = 0;
	
	///
	virtual void Shutdown() = 0;
	
	/// Restores all button and position states to defaults
	virtual void Reset() = 0; // ClearStates
	
	///
	virtual void Poll() = 0;
	
	///
	//virtual void SetActive(bool abActive) = 0;
	
	///
	//virtual bool IsActive() const = 0;
	
	///
	virtual void AttachToWindow(void *apWindow) = 0;
	
	///
	virtual void DetachFromWindow() = 0;
	
	///
	virtual void AddEventListener(IInputEventListener *apListener) = 0;
	
	///
	virtual void RemoveEventListener(IInputEventListener *apListener) = 0;
	
	///
	virtual bool IsKeyDown(eInputKey aeKey) const = 0;
	
	enum class AnalogCode : int
	{
		// TODO
	};
	
	virtual int GetAnalogValue(AnalogCode aeCode) const = 0;
	
	/// @return number of available gamepads
	virtual int GetGamepadCount() const = 0;
	
	///
	virtual void SetGamepadActive(int anGamepad, bool abActive) = 0;
};