/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "GameClientEventListener.hpp"

extern edict_t gEntities[MAX_EDICTS];

CGameClientEventListener::CGameClientEventListener() = default;
CGameClientEventListener::~CGameClientEventListener() = default;

/*
===========
ClientConnect

called when a player connects to a server
============
*/
bool CGameClientEventListener::OnClientConnect(int clientid /*edict_t *self*/, const char *name, const char *adr, char sRejectReason[128])
{
	if(!gEntities[clientid])
		return;
	
	bprint(PRINT_HIGH, gEntities[clientid]->v.netname);
	bprint(PRINT_HIGH, " entered the game\n");

	// a client connecting during an intermission can cause problems
	//if(intermission_running)
	//GotoNextMap();

	return true;
};

/*
===========
ClientDisconnect

Called when a player disconnects/drops from a server
Will not be called between levels
============
*/
void CGameClientEventListener::OnClientDisconnect(int clientid /*edict_t *self*/)
{
	if(!gEntities[clientid])
		return;
	
	// let everyone else know
	bprint(PRINT_HIGH, gEntities[clientid]->v.netname);
	bprint(PRINT_HIGH, " left the game");
	gpEngine->pfnEmitSound(gEntities[clientid], CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE);
	set_suicide_frame(gEntities[clientid]->v);
};

/*
============
ClientKill

Player entered the suicide command
============
*/
void CGameClientEventListener::OnClientKill(int clientid /*edict_t *self*/) // TODO: -> OnClientCommand?
{
	if(!gEntities[clientid])
		return;
	
	bprint(PRINT_MEDIUM, gEntities[clientid]->v.netname);
	bprint(PRINT_MEDIUM, " suicides\n");

	set_suicide_frame(gEntities[clientid]->v);

	//gEntities[clientid]->v.modelindex = modelindex_player;
	//logfrag(gEntities[clientid], gEntities[clientid]);
	//gEntities[clientid]->v.frags -= 2; // extra penalty
	//respawn(&gEntities[clientid]->v);
};

/*
===========
ClientPutInServer

called each time a player enters a new level
============
*/
void CGameClientEventListener::OnClientPutInServer(int client /*edict_t *client*/)
{
	if(!gEntities[client])
		return;
	
	CBaseEntity spot;
	string s;

	gEntities[client]->v.classname = "player";
	gEntities[client]->v.health = 100;
	gEntities[client]->v.takedamage = DAMAGE_AIM;
	gEntities[client]->v.solid = SOLID_SLIDEBOX;
	gEntities[client]->v.movetype = MOVETYPE_WALK;
	gEntities[client]->v.show_hostile = 0;
	gEntities[client]->v.max_health = 100;
	gEntities[client]->v.flags = FL_CLIENT;
	gEntities[client]->v.air_finished = time + 12;
	gEntities[client]->v.dmg = 2; // initial water damage
	gEntities[client]->v.super_damage_finished = 0;
	gEntities[client]->v.radsuit_finished = 0;
	gEntities[client]->v.invisible_finished = 0;
	gEntities[client]->v.invincible_finished = 0;
	gEntities[client]->v.effects = 0;
	gEntities[client]->v.invincible_time = 0;

	DecodeLevelParms();

	W_SetCurrentAmmo(gEntities[client]);

	gEntities[client]->v.attack_finished = time;
	gEntities[client]->v.th_pain = player_pain;
	gEntities[client]->v.th_die = PlayerDie;

	gEntities[client]->v.deadflag = DEAD_NO;
	// paustime is set by teleporters to keep the player from moving a while
	gEntities[client]->v.pausetime = 0;

	spot = SelectSpawnPoint();

	gEntities[client]->v.origin = spot.GetOrigin() + '0 0 1';
	gEntities[client]->v.angles = spot.GetOrigin();
	gEntities[client]->v.fixangle = TRUE; // turn this way immediately

	// oh, this is a hack!
	gpEngine->SetModel(gEntities[client], "models/eyes.mdl");
	modelindex_eyes = gEntities[client]->v.modelindex;

	gpEngine->SetModel(gEntities[client], "models/player.mdl");
	modelindex_player = gEntities[client]->v.modelindex;

	gpEngine->SetSize(gEntities[client], VEC_HULL_MIN, VEC_HULL_MAX);

	gEntities[client]->v.view_ofs = '0 0 22';

	// Mod - Xian (May.20.97)
	// Bug where player would have velocity from their last kill

	gEntities[client]->v.velocity = '0 0 0';

	player_stand1();

	makevectors(gEntities[client]->v.angles);
	spawn_tfog(gEntities[client]->v.origin + v_forward * 20);

	spawn_tdeath(gEntities[client]->v.origin, gEntities[client]->v);

	// Set Rocket Jump Modifiers
	if(stof(infokey(world, "rj")) != 0)
		rj = stof(infokey(world, "rj"));

	if(deathmatch == 4)
	{
		gEntities[client]->v.ammo_shells = 0;
		if(stof(infokey(world, "axe")) == 0)
		{
			gEntities[client]->v.ammo_nails = 255;
			gEntities[client]->v.ammo_shells = 255;
			gEntities[client]->v.ammo_rockets = 255;
			gEntities[client]->v.ammo_cells = 255;

			gEntities[client]->v.items |= IT_NAILGUN;
			gEntities[client]->v.items |= IT_SUPER_NAILGUN;
			gEntities[client]->v.items |= IT_SUPER_SHOTGUN;
			gEntities[client]->v.items |= IT_ROCKET_LAUNCHER;
			//gEntities[client]->v.items |= IT_GRENADE_LAUNCHER;
			gEntities[client]->v.items |= IT_LIGHTNING;
		};
		gEntities[client]->v.items = gEntities[client]->v.items - (gEntities[client]->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		gEntities[client]->v.armorvalue = 200;
		gEntities[client]->v.armortype = 0.8;
		gEntities[client]->v.health = 250;
		gEntities[client]->v.items |= IT_INVULNERABILITY;
		gEntities[client]->v.invincible_time = 1;
		gEntities[client]->v.invincible_finished = time + 3;
	};

	if(deathmatch == 5)
	{
		gEntities[client]->v.ammo_nails = 80;
		gEntities[client]->v.ammo_shells = 30;
		gEntities[client]->v.ammo_rockets = 10;
		gEntities[client]->v.ammo_cells = 30;
		gEntities[client]->v.items |= IT_NAILGUN;
		gEntities[client]->v.items |= IT_SUPER_NAILGUN;
		gEntities[client]->v.items |= IT_SUPER_SHOTGUN;
		gEntities[client]->v.items |= IT_ROCKET_LAUNCHER;
		gEntities[client]->v.items |= IT_GRENADE_LAUNCHER;
		gEntities[client]->v.items |= IT_LIGHTNING;
		gEntities[client]->v.items = gEntities[client]->v.items - (gEntities[client]->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		gEntities[client]->v.armorvalue = 200;
		gEntities[client]->v.armortype = 0.8;
		gEntities[client]->v.health = 200;
		gEntities[client]->v.items |= IT_INVULNERABILITY;
		gEntities[client]->v.invincible_time = 1;
		gEntities[client]->v.invincible_finished = time + 3;
	};
};

/*
=================
ClientCommand
=================
*/
void CGameClientEventListener::OnClientCommand(int clientid, const ICmdArgs &apArgs)
{
	if(!gEntities[clientid])
		return;
	
	// not fully in game yet
	//if(!gEntities[clientid]->client)
		//return;

	char *cmd = apArgs.GetByIndex(0);

	if (Q_stricmp (cmd, "players") == 0)
	{
		Cmd_Players_f(gEntities[clientid]);
		return;
	};
	
	if (Q_stricmp (cmd, "say") == 0)
	{
		Cmd_Say_f (gEntities[clientid], false, false);
		return;
	};
	
	if (Q_stricmp (cmd, "say_team") == 0)
	{
		Cmd_Say_f (gEntities[clientid], true, false);
		return;
	};
	
	if (Q_stricmp (cmd, "score") == 0)
	{
		Cmd_Score_f (gEntities[clientid]);
		return;
	};
	
	if (Q_stricmp (cmd, "help") == 0)
	{
		Cmd_Help_f (gEntities[clientid]);
		return;
	};

	if (level.intermissiontime)
		return;

	if (Q_stricmp (cmd, "use") == 0)
		Cmd_Use_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "drop") == 0)
		Cmd_Drop_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "give") == 0)
		Cmd_Give_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "god") == 0)
		Cmd_God_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "notarget") == 0)
		Cmd_Notarget_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "noclip") == 0)
		Cmd_Noclip_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "inven") == 0)
		Cmd_Inven_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "invnext") == 0)
		SelectNextItem (gEntities[clientid], -1);
	else if (Q_stricmp (cmd, "invprev") == 0)
		SelectPrevItem (gEntities[clientid], -1);
	else if (Q_stricmp (cmd, "invnextw") == 0)
		SelectNextItem (gEntities[clientid], IT_WEAPON);
	else if (Q_stricmp (cmd, "invprevw") == 0)
		SelectPrevItem (gEntities[clientid], IT_WEAPON);
	else if (Q_stricmp (cmd, "invnextp") == 0)
		SelectNextItem (gEntities[clientid], IT_POWERUP);
	else if (Q_stricmp (cmd, "invprevp") == 0)
		SelectPrevItem (gEntities[clientid], IT_POWERUP);
	else if (Q_stricmp (cmd, "invuse") == 0)
		Cmd_InvUse_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "invdrop") == 0)
		Cmd_InvDrop_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "weapprev") == 0)
		Cmd_WeapPrev_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "weapnext") == 0)
		Cmd_WeapNext_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "weaplast") == 0)
		Cmd_WeapLast_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "kill") == 0)
		Cmd_Kill_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "putaway") == 0)
		Cmd_PutAway_f (gEntities[clientid]);
	else if (Q_stricmp (cmd, "wave") == 0)
		Cmd_Wave_f (gEntities[clientid]);
	else if (Q_stricmp(cmd, "playerlist") == 0)
		Cmd_PlayerList_f(gEntities[clientid]);
	else	// anything that doesn't match a command will be a chat
		Cmd_Say_f (gEntities[clientid], false, true);
};

void CGameClientEventListener::OnClientUserInfoChanged(int clientid, char *userinfo)
{
	if(!gEntities[clientid])
		return;
	
	// TODO
};