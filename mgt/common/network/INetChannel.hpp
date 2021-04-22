/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2019 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief one-way communication path interface between two machines

#pragma once

#include "CommonTypes.hpp"
//#include "networksystem/NetworkTypes.hpp"
#include "enums.h"
#include "netadr.h"
#include "tier1/SizeBuffer.hpp"

// TODO
#ifndef MAX_MSGLEN
#define MAX_MSGLEN 8000   // max length of a reliable message
#endif

// TODO: temporarily exposing this instead of the actual interface to reduce porting efforts
#define MAX_LATENT 32

interface INetAdr;
interface INetMsg;
interface INetMsgHandler;

typedef struct netchan_s
{
	bool fatal_error;

	netsrc_t sock;

	float last_received; // for timeouts

	// the statistics are cleared at each client begin, because
	// the server connecting process gives a bogus picture of the data
	float frame_latency; // rolling average
	float frame_rate;

	int drop_count; // dropped packets, cleared each level
	int good_count; // cleared each level

	netadr_t remote_address;
	int qport;

	// bandwidth estimator
	double cleartime; // if realtime > nc->cleartime, free to go
	double rate;      // seconds / byte

	// sequencing variables
	int incoming_sequence;
	int incoming_acknowledged;
	int incoming_reliable_acknowledged; // single bit

	int incoming_reliable_sequence; // single bit, maintained local

	int outgoing_sequence;
	int reliable_sequence;      // single bit
	int last_reliable_sequence; // sequence number of last send

	// reliable staging and holding areas
	CSizeBuffer message; // writing buffer to send to server
	byte message_buf[MAX_MSGLEN];

	int reliable_length;
	byte reliable_buf[MAX_MSGLEN]; // unacked reliable message

	// time and size data to calculate bandwidth
	int outgoing_size[MAX_LATENT];
	double outgoing_time[MAX_LATENT];
} netchan_t;

interface INetChannel
{
	///
	virtual void Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport) = 0;

	///
	virtual void Transmit(int length, byte *data) = 0;
	
	//virtual void OutOfBand(netadr_t adr, int length, byte *data) = 0;
	//virtual void OutOfBandPrint(netadr_t adr, const char *format, ...) = 0;
	
	///
	virtual void SendMsg(const INetMsg &apMsg) = 0;
	
	///
	virtual bool ProcessMsg(INetMsg *net_message) = 0;

	///
	virtual bool CanPacket() const = 0; // TODO: CanSendPacket?
	
	///
	virtual bool CanReliable() const = 0;

	/**/
	//virtual void Shutdown(const char *asReason) = 0;
	
	/**/
	//virtual void Clear() = 0;
	
	/**/
	//virtual void Reset() = 0;
	
	///
	//virtual void ConnectTo(const INetAdr &aAdr) = 0;
	
	///
	//virtual void Disconnect() = 0;
	
	/**/
	//virtual void SetDataRate(float afRate) = 0;
	
	/**/
	//virtual bool IsOverflowed() const = 0;
	
	///
	//virtual const INetAdr &GetLocalAdr() const = 0;
	
	/**/
	//virtual const INetAdr &GetRemoteAdr() const = 0;
	
	///
	//virtual INetMsgHandler *GetMsgHandler() const = 0;
	
	/// Connection states
	enum class eConnectionState : int
	{
		Disconnected = 0,
		Connecting,
		ConnectionFailed,
		Connected
	};

	/**/
	//virtual eConnectionState GetConnectionState() const = 0;
	
	///
	//virtual bool RegisterMessage(INetMsg *apMsg) = 0;
	
	/**/
	//virtual bool AddNetMsg(INetMsg *apMsg, bool abForceReliable = false) = 0;
	
	/**
	*/
	//virtual bool SendMsg(const INetMsg &apMsg, bool abForceReliable = false, bool abVoice = false) = 0;
	
	/**/
	//virtual void SetCompressionActive(bool abActive) = 0;
	
	/**/
	//virtual void SetChallenge(uint anChallenge) = 0;
	
	/**/
	//virtual uint GetChallenge() const = 0;
	
	/**/
	//virtual int GetNumBitsWritten(bool abReliable) = 0;
	
	/**/
	//virtual void SetTimeout(float afSeconds) = 0;
	
	/**/
	//virtual bool IsTimedOut() const = 0;
};