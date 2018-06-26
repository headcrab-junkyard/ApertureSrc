/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2017-2018 BlackPhrase
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
/// @brief engine client interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_ENGINECLIENT_INTERFACE_VERSION{"MGTEngineClient001Alpha"};

interface ICmdArgs;

interface IEngineClient : public IBaseInterface
{
	///
	virtual bool Init(CreateInterfaceFn afnEngineFactory/*, tWinHandle ahWindow = 0*/) = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void ClearMemory() = 0;
	
	///
	virtual bool FilterTime(double frametime) const = 0;
	
	/// This allows to do something before the engine frame
	/// Returning false will prevent the engine from running the frame
	virtual bool PreFrame() = 0; // TODO: FrameBegin?
	
	///
	virtual void Frame() = 0;
	
	///
	//virtual void PostFrame() = 0;
	
	///
	virtual void ForwardCmdToServer(const ICmdArgs &apArgs) = 0;
	
	// TEMP CRAP
	
	/// Host ends the game
	virtual void HostEndGame() = 0;
	
	///
	virtual void HostError() = 0;
	
	///
	virtual void HostServerShutdown() = 0;
	
	///
	virtual void ConInit() = 0;
	
	///
	virtual void ConPrint(const char *msg) = 0;
};