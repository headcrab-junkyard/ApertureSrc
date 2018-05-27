/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2018 BlackPhrase
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
/// @brief coordinates spawning and killing of local servers

#include "quakedef.h"
//#include "r_local.h"
#include "engineclient/IEngineClient.hpp"

/*

A server can always be started, even if the system started out as a client
to a remote system.

A client can NOT be started if the system started as a dedicated server.

Memory is cleared / released when a server or client begins, not when they end.

*/

quakeparms_t host_parms;

bool host_initialized; // true if into command execution
//bool	nomaster;

double host_frametime;
//double		host_time; // TODO

double realtime;    // without any filtering or bounding
double oldrealtime; // last frame run

int host_framecount;

int host_hunklevel;

int minimum_memory;

client_t *host_client; // current client

jmp_buf host_abortserver;

byte *host_basepal; // TODO: unsigned short
byte *host_colormap;

void *gpEngineClientLib{ nullptr };
IEngineClient *gpEngineClient{ nullptr };

cvar_t host_framerate = { "host_framerate", "0" }; // set for slow motion
cvar_t host_speeds = { "host_speeds", "0" };       // set for running times
cvar_t host_profile = { "host_profile", "0" };

cvar_t sys_ticrate = { "sys_ticrate", "0.05" }; // TODO: 100

cvar_t fraglimit = { "fraglimit", "0", false, true };
cvar_t timelimit = { "timelimit", "0", false, true };
cvar_t teamplay = { "teamplay", "0", false, true };

#if defined(OGS_DEV) || defined(OGS_DEBUG)
cvar_t developer = { "developer", "1" }; // show extra messages (should be 0 for release!)
#else
cvar_t developer = { "developer", "0" };
#endif

cvar_t skill = { "skill", "1" };           // 0 - 3
cvar_t deathmatch = { "deathmatch", "0" }; // 0, 1, or 2
cvar_t coop = { "coop", "0" };             // 0 or 1

cvar_t pausable = { "pausable", "1" };

/*
================
Host_EndGame
================
*/
void Host_EndGame(const char *message, ...)
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, message);
	vsprintf(string, message, argptr);
	va_end(argptr);

	Con_DPrintf("Host_EndGame: %s\n", string);

	if(sv.active)
		Host_ShutdownServer(false);

	if(gbDedicatedServer)
		Sys_Error("Host_EndGame: %s\n", string); // dedicated servers exit

	// TODO: host event broadcaster -> notify about the game end?
	gpEngineClient->HostEndGame();

	longjmp(host_abortserver, 1);
};

/*
================
Host_Error

This shuts down both the client and server
================
*/
void Host_Error(const char *error, ...)
{
	va_list argptr;
	char string[1024]{};
	static bool inerror{ false };

	if(inerror)
		Sys_Error("Host_Error: recursively entered");

	inerror = true;

	// TODO: revisit
	SCR_EndLoadingPlaque(); // reenable screen updates

	va_start(argptr, error);
	vsprintf(string, error, argptr);
	va_end(argptr);

	Con_Printf("Host_Error: %s\n", string);

	//qw
	//SV_FinalMessage (va("server crashed: %s\n", string));
	//SV_Shutdown();

	if(sv.active)
		Host_ShutdownServer(false);

	if(gbDedicatedServer)
		Sys_Error("Host_Error: %s\n", string); // dedicated servers exit
	
	// TODO: host event listener -> onerror
	gpEngineClient->HostError();
	
	inerror = false;

	longjmp(host_abortserver, 1);
};

/*
================
Host_FindMaxClients
================
*/
void Host_FindMaxClients()
{
	svs.maxclients = 1;

	auto i{ COM_CheckParm("-dedicated") };

	if(i)
	{
		gbDedicatedServer = true;
		if(i != (com_argc - 1))
			svs.maxclients = Q_atoi(com_argv[i + 1]);
		else
			svs.maxclients = 8;
	};

	i = COM_CheckParm("-listen");
	if(i)
	{
		if(gbDedicatedServer)
			Sys_Error("Only one of -dedicated or -listen can be specified");
		
		if(i != (com_argc - 1))
			svs.maxclients = Q_atoi(com_argv[i + 1]);
		else
			svs.maxclients = 8;
	};

	if(svs.maxclients < 1)
		svs.maxclients = 8;
	else if(svs.maxclients > MAX_SCOREBOARD)
		svs.maxclients = MAX_SCOREBOARD;

	svs.maxclientslimit = svs.maxclients;
	
	if(svs.maxclientslimit < 4)
		svs.maxclientslimit = 4;
	
	svs.clients = (client_t*)Hunk_AllocName(svs.maxclientslimit * sizeof(client_t), "clients");

	if(svs.maxclients > 1)
		Cvar_SetValue("deathmatch", 1.0);
	else
		Cvar_SetValue("deathmatch", 0.0);
};

/*
=======================
Host_InitLocal
======================
*/
void Host_InitLocal()
{
	Host_InitCommands();

	Cvar_RegisterVariable(&host_framerate);
	Cvar_RegisterVariable(&host_speeds);
	Cvar_RegisterVariable(&host_profile);

	Cvar_RegisterVariable(&sys_ticrate);

	Cvar_RegisterVariable(&fraglimit);
	Cvar_RegisterVariable(&timelimit);
	Cvar_RegisterVariable(&teamplay);

	Cvar_RegisterVariable(&developer);

	Cvar_RegisterVariable(&skill);
	Cvar_RegisterVariable(&deathmatch);
	Cvar_RegisterVariable(&coop);

	Cvar_RegisterVariable(&pausable);

	Host_FindMaxClients();

	host_time = 1.0; // so a think at time 0 won't get called
};

/*
=================
SV_ClientPrintf

Sends text across to be displayed 
FIXME: make this just a stuffed echo?
=================
*/
void SV_ClientPrintf(client_t *cl, /*int level,*/ const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};

	//if (level < cl->messagelevel)
	//return;

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&cl->netchan.message, svc_print);
	//MSG_WriteByte (&cl->netchan.message, level);
	MSG_WriteString(&cl->netchan.message, string);
};

/*
=================
SV_BroadcastPrintf

Sends text to all active clients
=================
*/
void SV_BroadcastPrintf(const char *fmt, ...)
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	for(int i = 0; i < svs.maxclients; ++i)
		if(svs.clients[i].active && svs.clients[i].spawned)
		{
			MSG_WriteByte(&svs.clients[i].netchan.message, svc_print);
			MSG_WriteString(&svs.clients[i].netchan.message, string);
		};
};

/*
=================
Host_ClientCommands

Send text over to the client to be executed
=================
*/
void SV_ClientCommands(client_t *host_client, const char *fmt, ...) // TODO: was Host_ClientCommands
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	MSG_WriteByte(&host_client->netchan.message, svc_stufftext);
	MSG_WriteString(&host_client->netchan.message, string);
};

/*
=====================
SV_DropClient

Called when the player is getting totally kicked off the host
if (crash = true), don't bother sending signofs
=====================
*/
void SV_DropClient(client_t *drop, bool crash, char *fmt, ...)
{
	// TODO: fmt support

	// add the disconnect
	MSG_WriteByte(&drop->netchan.message, svc_disconnect);

	if(!crash)
	{
		// send any final messages (don't check for errors)
		if(Netchan_CanPacket(&drop->netchan)) // TODO: was NET_CanSendMessage; Netchan_CanReliable?
		{
			MSG_WriteByte(&drop->netchan.message, svc_disconnect);
			Netchan_Transmit(&drop->netchan, 0, drop->netchan.message.data);
		};

		if(drop->edict && drop->spawned)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			//gGlobalVariables.self = EDICT_TO_PROG(drop->edict);
			gpServerEventDispatcher->ClientDisconnect(drop->edict); // was gEntityInterface.pfnClientDisconnect
		};

		Sys_Printf("Client %s removed\n", drop->name);
	};

	// free the client (the body stays around)
	drop->active = false;
	drop->name[0] = 0;
	drop->old_frags = 0; // TODO: was -999999

	memset(drop->userinfo, 0, sizeof(drop->userinfo));

	// send notification to all clients
	int i;
	client_t *client;
	for(i = 0, client = svs.clients; i < svs.maxclients; i++, client++)
	{
		if(!client->active)
			continue;
		MSG_WriteByte(&client->netchan.message, svc_updatename);
		MSG_WriteByte(&client->netchan.message, drop - svs.clients);
		MSG_WriteString(&client->netchan.message, "");
		MSG_WriteByte(&client->netchan.message, svc_updatefrags);
		MSG_WriteByte(&client->netchan.message, drop - svs.clients);
		MSG_WriteShort(&client->netchan.message, 0);
		MSG_WriteByte(&client->netchan.message, svc_updatecolors);
		MSG_WriteByte(&client->netchan.message, drop - svs.clients);
		MSG_WriteByte(&client->netchan.message, 0);
	};
};

/*
==================
Host_ShutdownServer

This only happens at the end of a game, not between levels
==================
*/
void Host_ShutdownServer(bool crash)
{
	int i;
	int count;
	sizebuf_t buf;
	char message[4];
	double start;

	if(!sv.active)
		return;

	sv.active = false;
	
	// TODO: host event listener -> onservershutdown
	gpEngineClient->HostServerShutdown();

	// flush any pending messages - like the score!!!
	start = Sys_FloatTime();
	do
	{
		count = 0;
		for(i = 0, host_client = svs.clients; i < svs.maxclients; i++, host_client++)
		{
			if(host_client->active && host_client->netchan.message.cursize)
			{
				if(Netchan_CanPacket(&host_client->netchan)) // TODO: was NET_CanSendMessage; Netchan_CanReliable?
				{
					Netchan_Transmit(&host_client->netchan, strlen(message), buf.data);
					SZ_Clear(&host_client->netchan.message);
				}
				else
				{
					NET_GetPacket(NS_SERVER, &net_from, &host_client->netchan.message); // TODO: was NET_GetMessage; REVISIT: We're reading into write buffer?????????
					count++;
				};
			};
		};

		if((Sys_FloatTime() - start) > 3.0)
			break;
	} while(count);

	// make sure all the clients know we're disconnecting
	buf.data = (byte*)message;
	buf.maxsize = 4;
	buf.cursize = 0;
	
	MSG_WriteByte(&buf, svc_disconnect);
	
	SV_BroadcastCommand("disconnect"); // TODO: was count = NET_SendToAll(&buf, 5);
	
	if(count)
		Con_Printf("Host_ShutdownServer: NET_SendToAll failed for %u clients\n", count);

	for(i = 0, host_client = svs.clients; i < svs.maxclients; i++, host_client++)
		if(host_client->active)
			SV_DropClient(host_client, crash, "???");

	//
	// clear structures
	//
	memset(&sv, 0, sizeof(sv));
	memset(svs.clients, 0, svs.maxclientslimit * sizeof(client_t));
};

/*
================
Host_ClearMemory

This clears all the memory used by both the client and server, but does
not reinitialize anything.
================
*/
void Host_ClearMemory() // TODO: bool bQuiet
{
	Con_DPrintf("Clearing memory\n");

	Mod_ClearAll();

	if(host_hunklevel) // FIXME: check this...
		Hunk_FreeToLowMark(host_hunklevel);

	memset(&sv, 0, sizeof(sv));

	if(gpEngineClient)
		gpEngineClient->ClearMemory(); // HostClearMemory
};

//============================================================================

/*
===================
Host_FilterTime

Returns false if the time is too short to run a frame
===================
*/
qboolean Host_FilterTime(float time)
{
	realtime += time;

	//auto deltatime{realtime - oldrealtime};
	//if(NOT TIMEDEMO AND DELTATIME IS LESS THAN TIMESTEP(FPX_MAX))
	//inline float GetTimeStep(float fps){return 1.0/fps;}
	if(!cls.timedemo && realtime - oldrealtime < 1.0 / 72.0) // TODO: use "fps_max" cvar value here
		return false; // framerate is too high

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	if(host_framerate.value > 0)
		host_frametime = host_framerate.value;
	else
	{
		// don't allow really long or short frames
		
		if(host_frametime > 0.1)
			host_frametime = 0.1;
		
		if(host_frametime < 0.001)
			host_frametime = 0.001;
	};

	return true;
};

/*
===================
Host_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetConsoleCommands()
{
	char *cmd;

	while(1)
	{
		cmd = Sys_ConsoleInput();
		if(!cmd)
			break;
		Cbuf_AddText(cmd);
	};
};

/*
==================
Host_Frame

Runs all active servers
==================
*/
void _Host_Frame(float time)
{
	static double time1 = 0;
	static double time2 = 0;
	static double time3 = 0;
	int pass1, pass2, pass3;

	if(setjmp(host_abortserver))
		return; // something bad happened, or the server disconnected

	// keep the random time dependent
	rand();

	// decide the simulation time
	if(!Host_FilterTime(time))
		return; // don't run too fast, or packets will flood out

	// get new key events
	Sys_SendKeyEvents();

	// process console commands
	Cbuf_Execute();

	// if running the server locally, make intentions now
	// resend a connection request if necessary
	if(sv.active)
	{
		//if(cls.state == ca_disconnected)
			//CL_CheckForResend();
		//else
			//CL_SendCmd();
	};

	//-------------------
	//
	// server operations
	//
	//-------------------

	// check for commands typed to the host
	Host_GetConsoleCommands();

	if(sv.active)
		SV_Frame();

	if(gpEngineClient)
		gpEngineClient->Frame(time1, time2);

	if(host_speeds.value)
	{
		pass1 = (time1 - time3) * 1000;
		time3 = Sys_FloatTime();
		pass2 = (time2 - time1) * 1000;
		pass3 = (time3 - time2) * 1000;
		Con_Printf("%3i tot %3i server %3i gfx %3i snd\n",
		           pass1 + pass2 + pass3, pass1, pass2, pass3);
	};

	host_framecount++;
};

void Host_Frame(float time)
{
	double time1, time2;
	static double timetotal;
	static int timecount;
	int i, c, m;

	if(!host_profile.value)
	{
		_Host_Frame(time);
		return;
	};

	time1 = Sys_FloatTime();
	_Host_Frame(time);
	time2 = Sys_FloatTime();

	timetotal += time2 - time1;
	timecount++;

	if(timecount < 1000)
		return;

	m = timetotal * 1000 / timecount;
	timecount = 0;
	timetotal = 0;
	c = 0;
	for(i = 0; i < svs.maxclients; i++)
	{
		if(svs.clients[i].active)
			c++;
	};

	Con_Printf("host_profile: %2i clients %2i msec\n", c, m);
};

//============================================================================

/*
extern int vcrFile;
#define	VCR_SIGNATURE	0x56435231
// "VCR1"

void Host_InitVCR (quakeparms_t *parms)
{
	int		i, len, n;
	char	*p;
	
	if (COM_CheckParm("-playback"))
	{
		if (com_argc != 2)
			Sys_Error("No other parameters allowed with -playback\n");

		FS_FileOpenRead("quake.vcr", &vcrFile);
		if (vcrFile == -1)
			Sys_Error("playback file not found\n");

		FS_FileRead (vcrFile, &i, sizeof(int));
		if (i != VCR_SIGNATURE)
			Sys_Error("Invalid signature in vcr file\n");

		FS_FileRead (vcrFile, &com_argc, sizeof(int));
		com_argv = malloc(com_argc * sizeof(char *));
		com_argv[0] = parms->argv[0];
		for (i = 0; i < com_argc; i++)
		{
			FS_FileRead (vcrFile, &len, sizeof(int));
			p = malloc(len);
			FS_FileRead (vcrFile, p, len);
			com_argv[i+1] = p;
		};
		com_argc++; // add one for arg[0]
		parms->argc = com_argc;
		parms->argv = com_argv;
	};

	if ( (n = COM_CheckParm("-record")) != 0)
	{
		vcrFile = FS_FileOpenWrite("quake.vcr");

		i = VCR_SIGNATURE;
		FS_FileWrite(vcrFile, &i, sizeof(int));
		i = com_argc - 1;
		FS_FileWrite(vcrFile, &i, sizeof(int));
		for (i = 1; i < com_argc; i++)
		{
			if (i == n)
			{
				len = 10;
				FS_FileWrite(vcrFile, &len, sizeof(int));
				FS_FileWrite(vcrFile, "-playback", len);
				continue;
			}
			len = Q_strlen(com_argv[i]) + 1;
			FS_FileWrite(vcrFile, &len, sizeof(int));
			FS_FileWrite(vcrFile, com_argv[i], len);
		};
	};
};
*/

bool InitEngineClient()
{
	gpEngineClientLib{ Sys_LoadModule("engineclient") };

	if(!gpEngineClientLib)
		return false;

	auto pfnEngineClientFactory{ Sys_GetFactory(gpEngineClientLib) };

	if(!pfnEngineClientFactory)
		return false;

	gpEngineClient = (IEngineClient *)pfnEngineClientFactory(MGT_ENGINECLIENT_INTERFACE_VERSION, nullptr);

	if(!gpEngineClient)
		return false;

	if(gpEngineClient)
		if(!gpEngineClient->Init(Sys_GetFactoryThis()))
			return false;

	return true;
};

void ShutdownEngineClient()
{
	if(!gpEngineClientLib)
		return;

	if(gpEngineClient)
		gpEngineClient->Shutdown();

	gpEngineClient = nullptr;

	Sys_UnloadModule(gpEngineClientLib);
	gpEngineClientLib = nullptr;
};

/*
====================
Host_Init
====================
*/
void Host_Init(quakeparms_t *parms)
{
	minimum_memory = MINIMUM_MEMORY;

	if(COM_CheckParm("-minmemory"))
		parms->memsize = minimum_memory;

	host_parms = *parms;

	if(parms->memsize < minimum_memory)
		Sys_Error("Only %4.1f megs of memory available, can't execute game", parms->memsize / (float)0x100000);

	com_argc = parms->argc;
	com_argv = parms->argv;

	Memory_Init(parms->membase, parms->memsize);
	Cbuf_Init();
	Cmd_Init();

	//Host_InitVCR (parms);
	COM_Init(parms->basedir); // TODO: no basedir?
	Host_InitLocal();

	Con_Init();

	PR_Init();
	Mod_Init();

	NET_Init();
	Netchan_Init();

	SV_Init();

	Con_Printf("Protocol version %d\n", PROTOCOL_VERSION);
	//Con_Printf ("Exe version %s/%s (%s)\n", TODO); // Exe version 1.1.2.2/Stdio (tfc)
	//Con_Printf ("Exe build: " __TIME__ " " __DATE__ "(%d)\n", buildnum()); // TODO
	Con_Printf("%4.1f Mb heap\n", parms->memsize / (1024 * 1024.0));

	R_InitTextures(); // needed even for dedicated servers (BP: not really)

	if(!gbDedicatedServer)
	{
		host_basepal = (byte *)COM_LoadHunkFile("gfx/palette.lmp");
		if(!host_basepal)
			Sys_Error("Couldn't load gfx/palette.lmp");
		host_colormap = (byte *)COM_LoadHunkFile("gfx/colormap.lmp");
		if(!host_colormap)
			Sys_Error("Couldn't load gfx/colormap.lmp");

		if(!InitEngineClient())
			return;
	};

	Cbuf_InsertText("exec valve.rc\n");

	Hunk_AllocName(0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark();

	//Cbuf_InsertText ("exec server.cfg\n"); // TODO: dedicated

	host_initialized = true;

	//Con_Printf ("\nServer Version %4.2f (Build %04d)\n\n", VERSION, build_number());
	Sys_Printf("======== MGT Initialized =========\n"); // TODO: Con_Printf?
};

/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void Host_Shutdown()
{
	static bool bDown{ false };

	if(bDown)
	{
		printf("recursive shutdown\n");
		return;
	};

	bDown = true;

	ShutdownEngineClient();

	NET_Shutdown();
};