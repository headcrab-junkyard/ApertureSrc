/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2017-2018 BlackPhrase
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

#include "EngineClient.hpp"

CEngineClient::CEngineClient() = default;
CEngineClient::~CEngineClient() = default;

bool CEngineClient::Init(CreateInterfaceFn afnEngineFactory/*, tWinHandle ahWindow*/)
{
	V_Init ();
	Chase_Init ();

#ifndef _WIN32 // on non win32, mouse comes before video for security reasons
	IN_Init ();
#endif
	VID_Init (host_basepal);
	// TODO: GL_Init() here + no VID_Shutdown in GS (at least for hw)

	Draw_Init ();
	SCR_Init ();
	R_Init ();
#ifndef	_WIN32
// on Win32, sound initialization has to come before video initialization, so we
// can put up a popup if the sound hardware is in use
	S_Init ();
#else

#ifdef	GLQUAKE
// FIXME: doesn't use the new one-window approach yet
	S_Init ();
#endif

#endif	// _WIN32
	CDAudio_Init ();
	Sbar_Init ();
	CL_Init ();
#ifdef _WIN32 // on non win32, mouse comes before video for security reasons
	IN_Init ();
#endif

	// GUI should be initialized before the client dll
	VGui_Init();
	
	// Initialize the client dll now
	ClientDLL_Init();
};

void CEngineClient::Shutdown()
{
	// keep Con_Printf from trying to update the screen
	scr_disabled_for_loading = true; // TODO: revisit

	Host_WriteConfiguration(); 
	
	ClientDLL_Shutdown();
	VGui_Shutdown();
	
	CDAudio_Shutdown();
	
	S_Shutdown();
	IN_Shutdown();
	
	if(cls.state != ca_dedicated)
		VID_Shutdown();
};

/*
===============
Host_WriteConfiguration

Writes key bindings and archived cvars to config.cfg
===============
*/
void Host_WriteConfiguration ()
{
	FILE	*f;

	// dedicated servers initialize the host but don't parse and set the config.cfg cvars
	if (host_initialized & !isDedicated)
	{
		f = fopen (va("%s/config.cfg",com_gamedir), "w");
		if (!f)
		{
			Con_Printf ("Couldn't write config.cfg.\n");
			return;
		};
		
		Key_WriteBindings (f);
		Cvar_WriteVariables (f);

		fclose (f);
	};
};

void Host_UpdateScreen()
{
	// TODO: something else?
	
	SCR_UpdateScreen();
	
	// TODO: something else?
};

void CEngineClient::Frame()
{
	//-------------------
//
// client operations
//
//-------------------

// if running the server remotely, send intentions now after
// the incoming messages have been read
	if (!sv.active)
	{
		if (cls.state == ca_disconnected)
			CL_CheckForResend ();
		else
			CL_SendCmd ();
	};

	host_time += host_frametime;

// fetch results from server
	if (cls.state == ca_connected)
	{
		CL_ReadPackets();
	};

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(false);

	// do client side motion prediction
	CL_PredictMove ();

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(true);

	// build a refresh entity list
	CL_EmitEntities ();
	
// update video
	if (host_speeds.value)
		time1 = Sys_FloatTime ();
		
	Host_UpdateScreen (); // TODO: was SCR_UpdateScreen

	if (host_speeds.value)
		time2 = Sys_FloatTime ();
		
// update audio
	if (cls.signon == SIGNONS)
	{
		S_Update (r_origin, vpn, vright, vup);
		CL_DecayLights ();
	}
	else
		S_Update (vec3_origin, vec3_origin, vec3_origin, vec3_origin);
	
	CDAudio_Update();
};