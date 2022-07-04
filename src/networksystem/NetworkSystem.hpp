/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2022 BlackPhrase
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

#pragma once

#include <memory>

#include "networksystem/INetworkSystem.hpp"

interface ISystem;
class CNetChannel;

class CNetworkSystem final : public INetworkSystem
{
public:
	CNetworkSystem();
	~CNetworkSystem();
	
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	//void Frame(float afTimeStep) override;
	
	void Config(bool abEnableNetworking) override;
	
	INetServer *StartServer(int anPort) override;
	INetClient *StartClient() override;
	
	//void SendPacket(netsrc_t aeSock, int anLength, const void *apData, const netadr_t &aNetAdrTo) override;
	//bool GetPacket(netsrc_t aeSock, netadr_t &aNetAdrFrom, sizebuf_t &aNetMessage) override;
	
	//void SendMsg(netsrc_t aeSock, const INetMsg &aMsg, const netadr_t &aNetAdrTo) override;
	//bool GetMsg(netsrc_t aeSock, netadr_t &aNetAdrFrom, INetMsg &aNetMessage) override;
	
	bool StringToAdr(const char *asStr, netadr_t &aAdr) override;
private:
	void OpenIP();
	void OpenIPX();
	
	int IPSocket(char *asNetInterface, int anPort);
	int IPXSocket(int anPort);
private:
	std::unique_ptr<CNetServer> mpServer;
	std::unique_ptr<CNetClient> mpClient;
	
	ISystem *mpSystem{nullptr};
	
	bool mbEnableNetworking{false};
};