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
/// @brief host commands

#include "quakedef.h"

// TODO: temp
#include "engineclient/IEngineClient.hpp"
extern IEngineClient *gpEngineClient;

extern CConVar pausable;

int current_skill;

void Mod_Print_f(const ICmdArgs &apArgs);

// TODO: put commands here

//=============================================================================

/*
==================
Host_InitCommands
==================
*/
void Host_InitCommands()
{
	/*
	if (COM_CheckParm ("-cheats"))
	{
		allow_cheats = true;
		Info_SetValueForStarKey (svs.info, "*cheats", "ON", MAX_SERVERINFO_STRING);
	}
	*/

	// TODO
/*
	Cmd_AddCommand("status", Host_Status_f);
	Cmd_AddCommand("quit", Host_Quit_f);
	
#ifndef SWDS
	Cmd_AddCommand("god", Host_God_f);
	Cmd_AddCommand("notarget", Host_Notarget_f);
	Cmd_AddCommand("fly", Host_Fly_f);
	Cmd_AddCommand("noclip", Host_Noclip_f);
#endif	

	Cmd_AddCommand("map", Host_Map_f);
	Cmd_AddCommand("restart", Host_Restart_f);
	Cmd_AddCommand("changelevel", Host_Changelevel_f);
	Cmd_AddCommand("changelevel2", Host_Changelevel2_f);
	Cmd_AddCommand("reconnect", Host_Reconnect_f);
	//Cmd_AddCommand("name", Host_Name_f); // TODO

	Cmd_AddCommand("version", Host_Version_f);
#ifdef HEADCRABGODS
	Cmd_AddCommand("please", Host_Please_f);
#endif
	Cmd_AddCommand("say", Host_Say_f);
	Cmd_AddCommand("say_team", Host_Say_Team_f);
	Cmd_AddCommand("tell", Host_Tell_f);
	//Cmd_AddCommand("color", Host_Color_f); // TODO
	Cmd_AddCommand("kill", Host_Kill_f);
	Cmd_AddCommand("pause", Host_Pause_f);
	//Cmd_AddCommand("spawn", Host_Spawn_f); // TODO
	Cmd_AddCommand("begin", Host_Begin_f);
	Cmd_AddCommand("prespawn", Host_PreSpawn_f);
	Cmd_AddCommand("kick", Host_Kick_f);
	Cmd_AddCommand("ping", Host_Ping_f);
	Cmd_AddCommand("load", Host_Loadgame_f);
	Cmd_AddCommand("save", Host_Savegame_f);
	Cmd_AddCommand("give", Host_Give_f);

	Cmd_AddCommand("startdemos", Host_Startdemos_f);

#ifndef SWDS
	Cmd_AddCommand("viewmodel", Host_Viewmodel_f);
	Cmd_AddCommand("viewframe", Host_Viewframe_f);
	Cmd_AddCommand("viewnext", Host_Viewnext_f);
	Cmd_AddCommand("viewprev", Host_Viewprev_f);
#endif
*/

	Cmd_AddCommand("mcache", Mod_Print_f);
};