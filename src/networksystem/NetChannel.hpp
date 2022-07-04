/// @file

#pragma once

#include "networksystem/INetSocket.hpp"

interface ISystem;

class CNetChannel final : public INetChannel
{
public:
	CNetChannel(ISystem *apSystem);
	
	void SendPacket(int anLength, const void *apData, const netadr_t &aNetAdrTo) override;
	bool GetPacket(netadr_t &aNetAdrFrom, sizebuf_t &aNetMessage) override;
	
	void SendMsg(const INetMsg &aMsg, const netadr_t &aNetAdrTo) override;
	bool GetMsg(netadr_t &aNetAdrFrom, INetMsg &aMsg) override;
private:
	void SendLoopPacket(int length, const void *data, const netadr_t &to);
	bool GetLoopPacket(netadr_t &net_from, sizebuf_t &net_message);
private:
	static constexpr auto MAX_MSGLEN{1400};
	static constexpr auto MAX_LOOPBACK{4};

	struct loopmsg_t
	{
		byte data[MAX_MSGLEN];
		int datalen;
	};

	struct loopback_t
	{
		loopmsg_t msgs[MAX_LOOPBACK];
		int get, send;
	};

	loopback_t mLoopBack{};
	
	ISystem *mpSystem{nullptr};
	
	int ip_socket{-1};
	int ipx_socket{-1};
};