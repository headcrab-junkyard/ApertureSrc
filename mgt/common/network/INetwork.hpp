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
/// @brief network module interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"
#include "netadr.h"
#include "enums.h"
#include "sizebuf.h"

#define PORT_ANY -1

// TODO
#ifndef MAX_MSGLEN
#define MAX_MSGLEN 8000   // max length of a reliable message
#endif

// TODO: temporary exposing this instead of the actual interface to reduce porting efforts
#define MAX_LATENT 32

constexpr auto MGT_NETWORK_INTERFACE_VERSION{"MGTNetwork003Alpha"};

typedef struct
{
	qboolean fatal_error;

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
	sizebuf_t message; // writing buffer to send to server
	byte message_buf[MAX_MSGLEN];

	int reliable_length;
	byte reliable_buf[MAX_MSGLEN]; // unacked reliable message

	// time and size data to calculate bandwidth
	int outgoing_size[MAX_LATENT];
	double outgoing_time[MAX_LATENT];
} netchan_t;

interface INetwork : public IBaseInterface
{
	/**/
	virtual bool Init(CreateInterfaceFn afnEngineFactory) = 0;
	
	/// Shuts down network, closes all UPD/TCP channels
	virtual void Shutdown() = 0;
	
	// TODO
	
	virtual bool GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message) = 0;
	virtual void SendPacket(netsrc_t sock, int length, void *data, netadr_t to) = 0;
	
	//
	
	/**
	* Must be called each system frame to do any async TCP stuff
	*/
	//virtual void Update(float afTimeStep /*double afTime*/) = 0;
	
	/// @return true if the network is in use
	//virtual bool IsEnabled() const = 0;
	
	/**/
	//virtual void SetMultiplayer(bool abMultiplayer) = 0; // Config
	
	/**
	* @return true = full MP mode, false = loopback SP mode
	*/
	//virtual bool IsMultiplayer() const = 0;
	
	/**
	* Address conversion
	*/
	//virtual INetAdr *StringToNetAdr(const char *asString)= 0;
	
	//virtual INetServer *StartServer(int anPort) = 0;
	//virtual INetClient *StartClient() = 0;
};