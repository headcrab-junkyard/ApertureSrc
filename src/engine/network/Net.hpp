/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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
/// @brief engine's interface to the networking layer

#pragma once

#include "CommonTypes.hpp"
#include "enums.h"

interface INetworkSystem;
interface INetClient;
interface INetServer;

using netadr_t = struct netadr_s;
using sizebuf_t = struct sizebuf_s;

class CSystem;

class CNetwork
{
public:
	CNetwork(CSystem *apSystem);
	
	void Init();
	void Shutdown();

	void Config(bool abMultiplayer);

	void Sleep(int anMSec); // TODO: unused?
	
	//INetServer *GetServer() const;

	//INetClient *GetClient() const;

	bool GetPacket(netsrc_t sock, netadr_t &net_from, sizebuf_t &net_message);
	void SendPacket(netsrc_t sock, int length, const void *data, const netadr_t &to);

	bool CompareAdr(const netadr_t &a, const netadr_t &b);
	bool CompareBaseAdr(const netadr_t &a, const netadr_t &b);
	
	bool StringToAdr(const char *s, netadr_t &a);
private:
	void LoadModule();
	void UnloadModule();
	
	void *mpNetworkSystemModule{nullptr};
	
	INetworkSystem *mpNetworkSystem{nullptr};
	CSystem *mpSystem{nullptr};

	INetServer *mpServer{nullptr};
	INetClient *mpClient{nullptr};
};