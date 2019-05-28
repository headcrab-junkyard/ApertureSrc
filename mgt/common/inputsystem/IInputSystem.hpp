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
/// @brief input module interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"
//#include "input/IInputDevice.hpp"

#ifdef _WIN32
#include <windef.h>
#endif

constexpr auto MGT_INPUT_INTERFACE_VERSION{"MGTInput001Alpha"};

interface IInputEventListener;

struct SOSMessage
{
#ifdef _WIN32
	UINT mnMsg{0};
	LPARAM mnParam{0};
#else
	// TODO
#endif
};

interface IInput : public IBaseInterface
{
	///
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	/// Restores all button and position states to defaults
	//virtual void Clear() = 0; // ClearStates
	
	///
	//virtual void Frame() = 0;
	//virtual void Update(float afTimeStep) = 0;
	virtual void HandleOSMessage(const SOSMessage &apMsg) = 0; // TODO: IWindowMessageHandler
	
	///
	//virtual void SetActive(bool abActive) = 0;
	
	///
	//virtual bool IsActive() const = 0;
	
	///
	virtual void AddEventListener(IInputEventListener *apListener) = 0;
	
	///
	virtual void RemoveEventListener(IInputEventListener *apListener) = 0;
	
	///
	//virtual bool AddDevice(IInputDevice *apDevice) = 0;
	
	///
	//virtual IInputDevice *GetDeviceByType(IInputDevice::Type aeType) const = 0;
	
	///
	//virtual IInputDevice *GetDeviceByName(const char *asName) const = 0;
	
	///
	//virtual const char *Key_LookupBinding(const char *asBinding) const = 0;
};