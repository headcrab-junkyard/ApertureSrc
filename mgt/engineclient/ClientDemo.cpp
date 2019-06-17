/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018-2019 BlackPhrase
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
/// @brief demo handling

#include "quakedef.h"
#include "engine/ICmdArgs.hpp"

#include "network/INetworkSystem.hpp"
extern INetworkSystem *gpNetwork;

void CL_FinishTimeDemo();

/*
==============================================================================

DEMO CODE

When a demo is playing back, all NET_SendMessages are skipped, and
NET_GetMessages are read from the demo file.

Whenever cl.time gets past the last received message, another message is
read from the demo file.
==============================================================================
*/

/*
==============
CL_StopPlayback

Called when a demo file runs out, or the user starts a game
==============
*/
void CL_StopPlayback()
{
	if(!cls.demoplayback)
		return;

	gpFileSystem->CloseFile(cls.demofile);
	cls.demoplayback = false;
	cls.demofile = nullptr;
	cls.state = ca_disconnected;

	if(cls.timedemo)
		CL_FinishTimeDemo();
};

/*
====================
CL_WriteDemoMessage

Dumps the current net message, prefixed by the length and view angles
====================
*/
void CL_WriteDemoMessage(INetMsg *net_message)
{
	int len;
	int i;
	float f;

	len = LittleLong(net_message->cursize);
	cls.demofile->Write(&len, 4, 1);
	for(i = 0; i < 3; i++)
	{
		f = LittleFloat(cl.viewangles[i]);
		cls.demofile->Write(&f, 4, 1);
	};

	cls.demofile->Write(net_message->data, net_message->cursize, 1);
	cls.demofile->Flush();
};

/*
====================
CL_GetMessage

Handles recording and playback of demos, on top of NET_ code
====================
*/
qboolean CL_GetMessage(INetMsg *net_message)
{
	int r, i;
	float f;

	if(cls.demoplayback)
	{
		// decide if it is time to grab the next message
		if(cls.signon == SIGNONS) // allways grab until fully connected
		{
			if(cls.timedemo)
			{
				if(host_framecount == cls.td_lastframe)
					return 0; // allready read this frame's message
				cls.td_lastframe = host_framecount;
				// if this is the second frame, grab the real td_starttime
				// so the bogus time on the first frame doesn't count
				if(host_framecount == cls.td_startframe + 1)
					cls.td_starttime = realtime;
			}
			else if(/* cl.time > 0 && */ cl.time <= cl.mtime[0])
				return 0; // don't need another message yet
		};

		// get the next message
		cls.demofile->Read(&net_message->cursize, 4, 1);
		VectorCopy(cl.mviewangles[0], cl.mviewangles[1]);
		for(i = 0; i < 3; i++)
		{
			r = cls.demofile->Read(&f, 4, 1);
			cl.mviewangles[0][i] = LittleFloat(f);
		};

		net_message->cursize = LittleLong(net_message->cursize);
		if(net_message->cursize > MAX_MSGLEN)
			gpSystem->Error("Demo message > MAX_MSGLEN");
		r = cls.demofile->Read(net_message->data, net_message->cursize, 1);
		if(r != 1)
		{
			CL_StopPlayback();
			return false;
		};

		return true;
	};

	// TODO: BYE NETQUAKE!
	//while(1) // TODO
	{
		/*r =*/ if(!gpNetwork->GetPacket(NS_CLIENT, &net_from, net_message)) // TODO: was NET_GetMessage
			return false;

		//if(r != 1 && r != 2)
			//return r;

		// discard nop keepalive message
		if(net_message->cursize == 1 && net_message->data[0] == svc_nop)
			gpSystem->Printf("<-- server to client keepalive\n");
		//else
			//break;
	};

	if(cls.demorecording)
		CL_WriteDemoMessage(net_message);

	return true;
};

/*
====================
CL_StopRecording

stop recording a demo
====================
*/
void CL_StopRecording()
{
	if(!cls.demorecording)
	{
		gpSystem->Printf("Not recording a demo.\n");
		return;
	};

	// write a disconnect message to the demo file
	SZ_Clear(&net_message);
	MSG_WriteByte(&net_message, svc_disconnect);
	CL_WriteDemoMessage(&net_message);

	// finish up
	gpFileSystem->CloseFile(cls.demofile);
	
	cls.demofile = nullptr;
	cls.demorecording = false;
	
	gpSystem->Printf("Completed demo\n");
};

/*
====================
CL_Stop_f

stop recording a demo
====================
*/
void CL_Stop_f(const ICmdArgs &apArgs)
{
	if(cmd_source != src_command)
		return;

	CL_StopRecording();
};

/*
====================
CL_Record_f

record <demoname> <map> [cd track]
====================
*/
void CL_Record_f(const ICmdArgs &apArgs)
{
	int c;
	char name[MAX_OSPATH];
	int track;

	if(cmd_source != src_command)
		return;

	c = apArgs.GetCount();

	if(c != 2 && c != 3 && c != 4)
	{
		gpSystem->Printf("record <demoname> [<map> [cd track]]\n");
		return;
	};

	if(strstr(apArgs.GetByIndex(1), ".."))
	{
		gpSystem->Printf("Relative pathnames are not allowed.\n");
		return;
	};

	if(c == 2 && cls.state == ca_connected)
	{
		gpSystem->Printf("Can not record - already connected to server\nClient demo recording must be started before connecting\n");
		return;
	};

	// write the forced cd track number, or -1
	if(c == 4)
	{
		track = atoi(apArgs.GetByIndex(3));
		gpSystem->Printf("Forcing CD track to %i\n", cls.forcetrack);
	}
	else
		track = -1;

	sprintf(name, "%s/%s", com_gamedir, apArgs.GetByIndex(1));

	//
	// start the map up
	//
	if(c > 2)
		Cmd_ExecuteString(va("map %s", apArgs.GetByIndex(2)), src_command);

	//
	// open the demo file
	//
	COM_DefaultExtension(name, ".dem");

	gpSystem->Printf("recording to %s.\n", name);
	cls.demofile = gpFileSystem->OpenFile(name, "wb");

	if(!cls.demofile)
	{
		gpSystem->Printf("ERROR: couldn't open.\n");
		return;
	};

	cls.forcetrack = track;
	cls.demofile->Printf("%i\n", cls.forcetrack);

	cls.demorecording = true;
};

/*
====================
CL_PlayDemo_f

play [demoname]
====================
*/
void CL_PlayDemo_f(const ICmdArgs &apArgs)
{
	char name[256];
	int c;
	bool neg = false;

	if(cmd_source != src_command)
		return;

	if(apArgs.GetCount() != 2)
	{
		gpSystem->Printf("play <demoname> : plays a demo\n");
		return;
	};

	//
	// disconnect from server
	//
	CL_Disconnect();

	//
	// open the demo file
	//
	strcpy(name, apArgs.GetByIndex(1));
	COM_DefaultExtension(name, ".dem");

	gpSystem->Printf("Playing demo from %s.\n", name);
	cls.demofile = gpFileSystem->OpenFile(name, "rb");

	if(!cls.demofile)
	{
		gpSystem->Printf("ERROR: couldn't open.\n");
		cls.demonum = -1; // stop demo loop
		return;
	};

	cls.demoplayback = true;
	cls.state = ca_connected;
	cls.forcetrack = 0;

	while((c = cls.demofile->GetChar()) != '\n')
		if(c == '-')
			neg = true;
		else
			cls.forcetrack = cls.forcetrack * 10 + (c - '0');

	if(neg)
		cls.forcetrack = -cls.forcetrack;
	// ZOID, fscanf is evil
	//	fscanf (cls.demofile, "%i\n", &cls.forcetrack);
};

/*
====================
CL_FinishTimeDemo

====================
*/
void CL_FinishTimeDemo()
{
	int frames;
	float time;

	cls.timedemo = false;

	// the first frame didn't count
	frames = (host_framecount - cls.td_startframe) - 1;
	time = realtime - cls.td_starttime;
	if(!time)
		time = 1;
	gpSystem->Printf("%i frames %5.1f seconds %5.1f fps\n", frames, time, frames / time);
};

/*
====================
CL_TimeDemo_f

timedemo [demoname]
====================
*/
void CL_TimeDemo_f(const ICmdArgs &apArgs)
{
	if(cmd_source != src_command)
		return;

	if(apArgs.GetCount() != 2)
	{
		gpSystem->Printf("timedemo <demoname> : gets demo speeds\n");
		return;
	};

	CL_PlayDemo_f(apArgs);

	// cls.td_starttime will be grabbed at the second frame of the demo, so
	// all the loading time doesn't get counted

	cls.timedemo = true;
	cls.td_startframe = host_framecount;
	cls.td_lastframe = -1; // get a new message this frame
};