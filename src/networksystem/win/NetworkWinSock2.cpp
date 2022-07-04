/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2022 BlackPhrase
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

#include "NetworkWinSock2.hpp"

WSADATA winsockdata{};

void CNetworkWinSock2::Init()
{
	auto wVersionRequested{MAKEWORD(1, 1)};

	int r{WSAStartup(wVersionRequested, &winsockdata)};

	if(r)
		mpSystem->Error("Winsock initialization failed.");
};

void CNetworkWinSock2::Shutdown()
{
	WSACleanup();
};