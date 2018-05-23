/// @file

#include "quakedef.h"
#include "network/INetwork.hpp"

void *gpNetworkLib{nullptr};
INetwork *gpNetwork{nullptr};

bool InitNetwork()
{
	gpNetworkLib = Sys_LoadModule("network");
	
	if(!gpNetworkLib)
		return false;
	
	auto pfnNetworkFactory{Sys_GetFactory(gpNetworkLib)};
	
	if(!pfnNetworkFactory)
		return false;
	
	gpNetwork = pfnNetworkFactory(MGT_NETWORK_INTERFACE_VERSION, nullptr);
	
	if(!gpNetwork)
		return false;
};

void ShutdownNetwork()
{
	if(gpNetwork)
		gpNetwork->Shutdown();
	
	gpNetwork = nullptr;
	
	if(gpNetworkLib)
	{
		Sys_UnloadModule(gpNetworkLib);
		gpNetworkLib = nullptr;
	};
};

void NET_Init()
{
	if(!InitNetwork())
		gpNetwork = new CNetworkNull();
	
	gpNetwork->Init();
};

void NET_Shutdown()
{
	ShutdownNetwork();
};