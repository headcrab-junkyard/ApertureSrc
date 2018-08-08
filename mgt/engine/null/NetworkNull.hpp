/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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

#include "quakedef.h"
#include "network/INetwork.hpp"

class CNetworkNull final : public INetwork
{
public:
	CNetworkNull() = default;
	~CNetworkNull() = default;

	bool Init(CreateInterfaceFn afnEngineFactory) override {return true;}
	void Shutdown() override {}
	
	bool GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message) override {return true;}
	void SendPacket(netsrc_t sock, int length, void *data, netadr_t to) override {}
};

EXPOSE_SINGLE_INTERFACE(CNetworkNull, INetwork, MGT_NETWORK_INTERFACE_VERSION);