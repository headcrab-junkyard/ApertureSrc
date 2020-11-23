/*
Copyright (C) 2019-2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

using netchan_t = struct netchan_s;

class CNetChannel
{
public:
	//void Init();
	
	void Setup(netsrc_t sock, const netadr_t &adr, int qport);

	bool NeedReliable();
	
	void Transmit(int length, const byte *data);
	
	//void OutOfBand(int net_socket, netadr_t adr, int length, const byte *data);
	//void OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...);
	
	bool Process(sizebuf_t *msg);

	bool CanPacket();
	bool CanReliable();
private:
	netchan_t *chan{nullptr};
};