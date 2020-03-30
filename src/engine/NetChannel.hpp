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

class CNetChannel
{
public:
	void Init();
	
	void Setup(netsrc_t sock, netchan_t *chan, const netadr_t &adr, int qport);

	bool NeedReliable(netchan_t *chan);
	
	void Transmit(netchan_t *chan, int length, const byte *data);
	
	void OutOfBand(int net_socket, netadr_t adr, int length, const byte *data);
	void OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...);
	
	bool Process(netchan_t *chan, sizebuf_t *msg);

	bool CanPacket(netchan_t *chan);
	bool CanReliable(netchan_t *chan);
};