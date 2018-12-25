/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
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

#include "common/CommonTypes.hpp"
//#include "network/NetworkTypes.hpp"

struct INetAdr;
struct INetMessage;
struct INetMsgHandler;

interface INetChannel
{
	virtual void Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport) = 0;

	virtual void Transmit(int length, byte *data) = 0;
	
	//virtual void OutOfBand(netadr_t adr, int length, byte *data) = 0;
	//virtual void OutOfBandPrint(netadr_t adr, const char *format, ...) = 0;
	
	virtual bool Process(INetMessage *net_message) = 0;

	virtual bool CanPacket() const = 0;
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
	//virtual bool CanSendPacket() const = 0;
	
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
	//virtual bool RegisterMessage(INetMessage *apMsg) = 0;
	
	/**/
	//virtual bool AddNetMsg(INetMsg *apMsg, bool abForceReliable = false) = 0;
	
	/**
	*/
	//virtual bool SendNetMsg(const INetMessage &apMsg, bool abForceReliable = false, bool abVoice = false) = 0;
	
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