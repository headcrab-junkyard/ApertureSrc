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

#include "quakedef.h"
#include "Net.hpp"
#include "System.hpp"

CNetwork::CNetwork(CSys *apSystem) : mpSystem(apSystem){}

void CNetwork::Init()
{
	LoadModule();
	
	mpNetworkSystem->Init(Sys_GetFactoryThis());
};

void CNetwork::Shutdown()
{
	if(mpNetworkSystem)
	{
		mpNetworkSystem->Shutdown();
		mpNetworkSystem = nullptr;
	};
	
	UnloadModule();
};

void CNetwork::Config(bool multiplayer)
{
	mpNetworkSystem->Config(multiplayer);
};

void CNetwork::Sleep(int msec)
{
	//mpNetworkSystem->Sleep(msec); // TODO
};

bool CNetwork::GetPacket(netsrc_t sock, netadr_t &net_from, sizebuf_t &net_message)
{
	return mpNetworkSystem->GetPacket(sock, net_from, net_message);
};

void CNetwork::SendPacket(netsrc_t sock, int length, const void *data, const netadr_t &to)
{
	mpNetworkSystem->SendPacket(sock, length, data, to);
};

bool CNetwork::CompareAdr(const netadr_t &a, const netadr_t &b)
{
	return NET_CompareAdr(a, b);
};

bool CNetwork::CompareBaseAdr(const netadr_t &a, const netadr_t &b)
{
	return NET_CompareBaseAdr(a, b);
};

bool CNetwork::StringToAdr(const char *s, netadr_t &a)
{
	return mpNetworkSystem->StringToAdr(s, a);
};

void CNetwork::LoadModule()
{
	UnloadModule();
	
	mpNetworkSystemModule = Sys_LoadModule("networksystem");
	
	if(!mpNetworkSystemModule)
		mpSystem->Error("");
	
	auto fnNetworkSystemFactory{Sys_GetFactory(mpNetworkSystemModule)};
	
	if(!fnNetworkSystemFactory)
		mpSystem->Error("");
	
	mpNetworkSystem = reinterpret_cast<INetworkSystem*>(fnNetworkSystemFactory(OGS_NETWORKSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpNetworkSystem)
		mpSystem->Error("");
};

void CNetwork::UnloadModule()
{
	if(mpNetworkSystemModule)
	{
		Sys_UnloadModule(mpNetworkSystemModule);
		mpNetworkSystemModule = nullptr;
	};
};