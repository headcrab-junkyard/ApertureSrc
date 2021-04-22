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
/// @brief network message interface

#pragma once

#include "CommonTypes.hpp"

interface INetChannel;
interface IReadBuffer;
interface IWriteBuffer;

interface INetMsg
{
	/**
	*/
	//virtual void Release() = 0;
	
	/// @return true if parsing was OK
	virtual bool ReadFromBuffer(/*const*/ IReadBuffer &aBuffer) = 0;
	
	/// @return true if writing was OK
	virtual bool WriteToBuffer(IWriteBuffer &aBuffer) = 0;
	
	/** Calles the recently set handler to process this message */
	//virtual bool Process() = 0;
	
	/** Net channel this message is from/for */
	//virtual void SetNetChannel(INetChannel *apChannel) = 0;
	
	/**
	*/
	//virtual INetChannel *GetNetChannel() const = 0;
	
	/** Set to true if it's a reliable message */
	//virtual void SetReliable(bool abReliable) = 0;
	
	/** True, if message needs reliable handling */
	//virtual bool IsReliable() const = 0;
	
	/** Returns module specific header tag eg svc_serverinfo */
	//virtual int GetType() const = 0;
	
	/** Returns network message name, eg "svc_serverinfo" */
	virtual const char *GetName() const = 0;
	
	/** Returns a human readable string about message content */
	//virtual const char *ToString() const = 0;
};