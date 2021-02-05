/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2020 BlackPhrase
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

#include <cstring>

#include "qlibc/qlibc.h"
#include "NetChannel.hpp"
#include "network/INetwork.hpp"
#include "network/INetMsg.hpp"
#include "engine/ISystem.hpp"

#ifdef _WIN32
#include "winquake.h"
#endif

#define PACKET_HEADER 8

/*

packet header
-------------
31	sequence
1	does this message contain a reliable payload
31	acknowledge sequence
1	acknowledge receipt of even/odd message
16  qport

The remote connection never knows if it missed a reliable message, the
local side detects that it has been dropped by seeing a sequence acknowledge
higher thatn the last reliable sequence, but without the correct evon/odd
bit for the reliable set.

If the sender notices that a reliable message has been dropped, it will be
retransmitted.  It will not be retransmitted again until a message after
the retransmit has been acknowledged and the reliable still failed to get there.

if the sequence number is -1, the packet should be handled without a netcon

The reliable message can be added to at any time by doing
netchan->message->Write* (<data>).

If the message buffer is overflowed, either by a single message, or by
multiple frames worth piling up while the last reliable transmit goes
unacknowledged, the netchan signals a fatal error.

Reliable messages are allways placed first in a packet, then the unreliable
message is included if there is sufficient room.

To the receiver, there is no distinction between the reliable and unreliable
parts of the message, they are just processed out as a single larger message.

Illogical packet sequence numbers cause the packet to be dropped, but do
not kill the connection.  This, combined with the tight window of valid
reliable acknowledgement numbers provides protection against malicious
address spoofing.

The qport field is a workaround for bad address translating routers that
sometimes remap the client's source port on a packet during gameplay.

If the base part of the net address matches and the qport matches, then the
channel matches even if the IP port differs.  The IP port should be updated
to the new value before sending out any replies.


*/

int net_drop;

CNetChannel::CNetChannel(ISystem *apSystem, INetwork *apNetwork) : mpSystem(apSystem), mpNetwork(apNetwork){}
CNetChannel::~CNetChannel() = default;

/*
==============
Netchan_Setup

called to open a channel to a remote system
==============
*/
void CNetChannel::Setup(netsrc_t sock, netchan_t *chan, netadr_t adr, int qport)
{
	Q_memset(chan, 0, sizeof(*chan));

	chan->sock = sock;
	chan->remote_address = adr;
	chan->last_received = mpSystem->GetRealTime(); // TODO: not present in q3

	chan->message.data = chan->message_buf; // TODO: not present in q3
	chan->message.allowoverflow = true; // TODO: not present in q3
	chan->message.maxsize = sizeof(chan->message_buf); // TODO: not present in q3

	chan->qport = qport;

	chan->rate = 1.0 / 2500; // TODO: not present in q3
	
	//chan->incomingSequence = 0; // TODO: q3
	//chan->outgoingSequence = 1; // TODO: q3
};

#ifdef SWDS // TODO
bool ServerPaused();
#endif

/*
===============
Netchan_Transmit

tries to send an unreliable message to a connection, and handles the
transmition / retransmition of the reliable messages.

A 0 length will still generate a packet and deal with the reliable messages.
================
*/
void CNetChannel::Transmit(int length, byte *data)
{
	CSizeBuffer send;
	byte send_buf[MAX_MSGLEN + PACKET_HEADER];
	bool send_reliable;
	unsigned w1, w2;
	int i;

	// check for message overflow
	if(mpData->message.overflowed)
	{
		mpData->fatal_error = true;
		mpSystem->Printf("%s:Outgoing message overflow\n", mpData->remote_address.ToString());
		return;
	};

	// if the remote side dropped the last reliable message, resend it
	send_reliable = false;

	if(mpData->incoming_acknowledged > mpData->last_reliable_sequence && mpData->incoming_reliable_acknowledged != mpData->reliable_sequence)
		send_reliable = true;

	// if the reliable transmit buffer is empty, copy the current message out
	if(!mpData->reliable_length && mpData->message.cursize)
	{
		Q_memcpy(mpData->reliable_buf, mpData->message_buf, mpData->message.cursize);
		mpData->reliable_length = mpData->message.cursize;
		mpData->message.cursize = 0;
		mpData->reliable_sequence ^= 1;
		send_reliable = true;
	};

	// write the packet header
	send.data = send_buf;
	send.maxsize = sizeof(send_buf);
	send.cursize = 0;

	w1 = mpData->outgoing_sequence | (send_reliable << 31);
	w2 = mpData->incoming_sequence | (mpData->incoming_reliable_sequence << 31);

	mpData->outgoing_sequence++;

	send.WriteLong(w1);
	send.WriteLong(w2);

	// send the qport if we are a client
	//if (mpData->sock == NS_CLIENT) // TODO
		//send.WriteShort (cls.qport);

	// copy the reliable message to the packet first
	if(send_reliable)
	{
		send.Write(mpData->reliable_buf, mpData->reliable_length);
		mpData->last_reliable_sequence = mpData->outgoing_sequence;
	};

	// add the unreliable part if space is available
	if(send.maxsize - send.cursize >= length)
		send.Write(data, length);

	// send the datagram
	i = mpData->outgoing_sequence & (MAX_LATENT - 1);
	mpData->outgoing_size[i] = send.cursize;
	mpData->outgoing_time[i] = mpSystem->GetRealTime();

//#ifndef SWDS // TODO
	//zoid, no input in demo playback mode
	//if (!cls.demoplayback)
//#endif
	mpNetwork->SendPacket(mpData->sock, send.cursize, send.data, mpData->remote_address);

	if(mpData->cleartime < mpSystem->GetRealTime())
		mpData->cleartime = mpSystem->GetRealTime() + send.cursize * mpData->rate;
	else
		mpData->cleartime += send.cursize * mpData->rate;

#ifdef SWDS // TODO
	if(ServerPaused())
		mpData->cleartime = mpSystem->GetRealTime();
#endif

	if(net_showpackets.value)
		mpSystem->Printf("--> s=%i(%i) a=%i(%i) %i\n", mpData->outgoing_sequence, send_reliable, mpData->incoming_sequence, mpData->incoming_reliable_sequence, send.cursize);
};

void CNetChannel::SendMsg(const INetMsg &apMsg)
{
};

/*
=================
Netchan_Process

called when the current net_message is from remote_address
modifies net_message so that it points to the packet payload
=================
*/
bool CNetChannel::ProcessMsg(INetMsg *net_message)
{
	unsigned sequence, sequence_ack;
	unsigned reliable_ack, reliable_message;

	int qport;

	int i;

	if(
//#ifndef SWDS // TODO
	// !cls.demoplayback &&
//#endif
	net_from != mpData->remote_address)
		return false;

	// XOR unscramble all data in the packet after the header
	//Netchan_UnScramblePacket( msg );

	// get sequence numbers
	MSG_BeginReading();
	sequence = net_message->ReadLong();
	sequence_ack = net_message->ReadLong();

	// read the qport if we are a server
	if(mpData->sock == NS_SERVER)
		qport = net_message->ReadShort();

	reliable_message = sequence >> 31;
	reliable_ack = sequence_ack >> 31;

	sequence &= ~(1 << 31);
	sequence_ack &= ~(1 << 31);

	if(net_showpackets.value)
		mpSystem->Printf("<-- s=%i(%i) a=%i(%i) %i\n", sequence, reliable_message, sequence_ack, reliable_ack, net_message->cursize);

// get a rate estimation
#if 0
	if (mpData->outgoing_sequence - sequence_ack < MAX_LATENT)
	{
		int				i;
		double			time, rate;
	
		i = sequence_ack & (MAX_LATENT - 1);
		time = mpSystem->GetRealTime() - mpData->outgoing_time[i];
		time -= 0.1;	// subtract 100 ms
		if (time <= 0)
		{	// gotta be a digital link for <100 ms ping
			if (mpData->rate > 1.0/5000)
				mpData->rate = 1.0/5000;
		}
		else
		{
			if (mpData->outgoing_size[i] < 512)
			{	// only deal with small messages
				rate = mpData->outgoing_size[i]/time;
				if (rate > 5000)
					rate = 5000;
				rate = 1.0/rate;
				if (mpData->rate > rate)
					mpData->rate = rate;
			};
		};
	};
#endif

	//
	// discard stale or duplicated packets
	//
	if(sequence <= (unsigned)mpData->incoming_sequence)
	{
		if(net_showdrop.value)
			mpSystem->Printf("%s:Out of order packet %i at %i\n", mpData->remote_address->ToString(), sequence, mpData->incoming_sequence);
		return false;
	};

	//
	// dropped packets don't keep the message from being used
	//
	net_drop = sequence - (mpData->incoming_sequence + 1);
	if(net_drop > 0)
	{
		mpData->drop_count += 1;

		if(net_showdrop.value)
			mpSystem->Printf("%s:Dropped %i packets at %i\n", mpData->remote_address.ToString(), sequence - (mpData->incoming_sequence + 1), sequence);
	};

	//
	// if the current outgoing reliable message has been acknowledged
	// clear the buffer to make way for the next
	//
	if(reliable_ack == (unsigned)mpData->reliable_sequence)
		mpData->reliable_length = 0; // it has been received

	//
	// if this message contains a reliable message, bump incoming_reliable_sequence
	//
	mpData->incoming_sequence = sequence;
	mpData->incoming_acknowledged = sequence_ack;
	mpData->incoming_reliable_acknowledged = reliable_ack;
	if(reliable_message)
		mpData->incoming_reliable_sequence ^= 1;

	//
	// the message can now be read from the current message pointer
	// update statistics counters
	//
	mpData->frame_latency = mpData->frame_latency * OLD_AVG + (mpData->outgoing_sequence - sequence_ack) * (1.0 - OLD_AVG);
	mpData->frame_rate = mpData->frame_rate * OLD_AVG + (mpSystem->GetRealTime() - mpData->last_received) * (1.0 - OLD_AVG);
	mpData->good_count += 1;

	mpData->last_received = mpSystem->GetRealTime();

	return true;
};

/*
===============
Netchan_CanPacket

Returns true if the bandwidth choke isn't active
================
*/
#define MAX_BACKUP 200
bool CNetChannel::CanPacket() const
{
	if(mpData->cleartime < mpSystem->GetRealTime() + MAX_BACKUP * mpData->rate)
		return true;
	return false;
};

/*
===============
Netchan_CanReliable

Returns true if the bandwidth choke isn't 
================
*/
bool CNetChannel::CanReliable() const
{
	if(mpData->reliable_length)
		return false; // waiting for ack
	return CanPacket();
};