/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
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

#include "network/INetChannel.hpp"

struct INetwork;

class CNetChannel final : public INetChannel
{
public:
	CNetChannel(INetwork *apNetwork);
	~CNetChannel();
	
	void Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport) override;

	void Transmit(int length, byte *data) override;
	
	//void OutOfBand(netadr_t adr, int length, byte *data) override;
	//void OutOfBandPrint(netadr_t adr, const char *format, ...) override;
	
	bool Process(INetMessage *net_message) override;

	bool CanPacket() const override;
	bool CanReliable() const override;
private:
	INetwork *mpNetwork{nullptr};
	netchan_t *mpData{nullptr};
};