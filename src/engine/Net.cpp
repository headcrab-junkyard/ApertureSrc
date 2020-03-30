#include "quakedef.h"
#include "Net.hpp"

void CNet::Init()
{
	NET_Init();
};

void CNet::Shutdown()
{
	NET_Shutdown();
};

void CNet::Config(bool multiplayer)
{
	NET_Config(multiplayer);
};

void CNet::Sleep(int msec)
{
	NET_Sleep(msec);
};

bool CNet::GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
	return NET_GetPacket(sock, net_from, net_message);
};

void CNet::SendPacket(netsrc_t sock, int length, const void *data, netadr_t to)
{
	NET_SendPacket(sock, length, data, to);
};

bool CNet::CompareAdr(const netadr_t &a, const netadr_t &b)
{
	return NET_CompareAdr(a, b);
};

bool CNet::CompareBaseAdr(const netadr_t &a, const netadr_t &b)
{
	return NET_CompareBaseAdr(a, b);
};

bool CNet::IsLocalAddress(const netadr_t &adr)
{
	return NET_IsLocalAddress(adr);
};

const char *CNet::AdrToString(const netadr_t &a)
{
	return NET_AdrToString(a);
};

bool CNet::StringToAdr(const char *s, netadr_t *a)
{
	return NET_StringToAdr(s, a);
};