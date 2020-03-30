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

class CNet
{
public:
	void Init();
	void Shutdown();

	void Config(bool multiplayer);

	void Sleep(int msec);
	
	bool GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message);
	void SendPacket(netsrc_t sock, int length, const void *data, netadr_t to);

	bool CompareAdr(const netadr_t &a, const netadr_t &b);
	bool CompareBaseAdr(const netadr_t &a, const netadr_t &b);
	
	bool IsLocalAddress(const netadr_t &adr);
	
	const char *AdrToString(const netadr_t &a);
	bool StringToAdr(const char *s, netadr_t *a);
};