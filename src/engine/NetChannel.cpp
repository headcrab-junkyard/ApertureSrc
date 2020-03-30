#include "quakedef.h"
#include "NetChannel.hpp"

void CNetChannel::Init()
{
	Netchan_Init();
};

void CNetChannel::Setup(netsrc_t sock, netchan_t *chan, const netadr_t &adr, int qport)
{
	Netchan_Setup(sock, chan, adr, qport);
};

bool CNetChannel::NeedReliable(netchan_t *chan)
{
	return Netchan_NeedReliable(chan);
};

void CNetChannel::Transmit(netchan_t *chan, int length, const byte *data)
{
	Netchan_Transmit(chan, length, data);
};

void CNetChannel::OutOfBand(int net_socket, netadr_t adr, int length, const byte *data)
{
	Netchan_OutOfBand(net_socket, adr, length, data);
};

void CNetChannel::OutOfBandPrint(int net_socket, netadr_t adr, const char *format, ...)
{
	Netchan_OutOfBandPrint(net_socket, adr, format);
};

bool CNetChannel::Process(netchan_t *chan, sizebuf_t *msg)
{
	return Netchan_Process(chan, msg);
};

bool CNetChannel::CanPacket(netchan_t *chan)
{
	return Netchan_CanPacket(chan);
};

bool CNetChannel::CanReliable(netchan_t *chan)
{
	return Netchan_CanReliable(chan);
};