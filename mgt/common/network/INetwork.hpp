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
/// @brief network module interface

#pragma once

#include "CommonTypes.hpp"
#include "tier1/interface.h"
#include "netadr.h"
#include "enums.h"

constexpr auto PORT_ANY{-1};

constexpr auto OGS_NETWORKSYSTEM_INTERFACE_VERSION{"OGSNetworkSystem004"};

interface INetMsg;

interface INetworkSystem : public IBaseInterface
{
	/**/
	virtual bool Init(CreateInterfaceFn afnEngineFactory) = 0;
	
	/// Shuts down network, closes all UPD/TCP channels
	virtual void Shutdown() = 0;
	
	/**
	* Must be called each system frame to do any async TCP stuff
	*/
	//virtual void Update(float afTimeStep /*double afTime*/) = 0;
	
	///
	virtual void Config(bool abEnableNetworking) = 0;
	//virtual void SetMultiplayer(bool abMultiplayer) = 0;
	
	/// @return true = full MP mode, false = loopback SP mode
	//virtual bool IsMultiplayer() const = 0;
	
	// TODO
	
	///
	virtual void SendPacket(netsrc_t aeSock, int anLength, const void *apData, const netadr_t &aNetAdrTo) = 0;
	
	///
	virtual bool GetPacket(netsrc_t aeSock, netadr_t &aNetAdrFrom, sizebuf_t &aNetMessage) = 0;
	
	///
	virtual void SendMsg(netsrc_t aeSock, const INetMsg &aMsg, const netadr_t &aNetAdrTo) = 0;
	
	///
	virtual bool GetMsg(netsrc_t aeSock, netadr_t &aNetAdrFrom, INetMsg &aMsg) = 0;
	
	///
	//virtual INetSocket *GetSocket(netsrc_t aeType) = 0;
	
	/**
	* Address conversion
	*/
	//virtual INetAdr *StringToNetAdr(const char *asString)= 0;
	virtual bool StringToAdr(const char *s, netadr_t &a) = 0;
	
	//
	
	/// @return true if the network is in use
	//virtual bool IsEnabled() const = 0;
	
	//virtual INetServer *StartServer(int anPort) = 0;
	//virtual INetClient *StartClient() = 0;
};