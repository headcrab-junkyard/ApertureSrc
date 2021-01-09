#include "quakedef.h"
#include "NetChannel.hpp"

/*
void CNetChannel::Init()
{
	Netchan_Init();
};
*/

void CNetChannel::Setup(netsrc_t sock, const netadr_t &adr, int qport)
{
	//Netchan_Setup(sock, chan, adr, qport);
};

bool CNetChannel::NeedReliable() const
{
	return false; //return Netchan_NeedReliable(chan);
};

void CNetChannel::Transmit(int length, const byte *data)
{
	//Netchan_Transmit(chan, length, data);
};

/*
void CNetChannel::OutOfBand(int net_socket, netadr_t adr, int length, const byte *data)
{
	Netchan_OutOfBand(net_socket, adr, length, data);
};

void CNetChannel::OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...)
{
	Netchan_OutOfBandPrint(net_socket, adr, format);
};
*/

bool CNetChannel::Process(sizebuf_t *msg)
{
	return true; //return Netchan_Process(chan, msg);
};

bool CNetChannel::CanPacket() const
{
	return true; //return Netchan_CanPacket(chan);
};

bool CNetChannel::CanReliable() const
{
	return true; //return Netchan_CanReliable(chan);
};