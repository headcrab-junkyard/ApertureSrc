#include "quakedef.h"
#include "Net.hpp"
#include "Sys.hpp"

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