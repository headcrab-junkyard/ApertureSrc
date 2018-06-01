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

CGameClientEventListener::CGameClientEventListener() = default;
CGameClientEventListener::~CGameClientEventListener() = default;

/*
===========
ClientConnect

called when a player connects to a server
============
*/
bool CGameClientEventListener::OnClientConnect(edict_t *self, const char *name, const char *adr, char *sRejectReason[128])
{
	bprint (PRINT_HIGH, self->v.netname);
	bprint (PRINT_HIGH, " entered the game\n");
	
	// a client connecting during an intermission can cause problems
	//if(intermission_running)
		//GotoNextMap();
	
	return true;
};

/*
===========
ClientDisconnect

called when a player disconnects from a server
============
*/
void CGameClientEventListener::OnClientDisconnect(edict_t *self)
{
	// let everyone else know
	bprint (PRINT_HIGH, self->v.netname);
		bprint (PRINT_HIGH, " left the game with ");
		bprint (PRINT_HIGH, ftos(self->v.frags));
		bprint (PRINT_HIGH, " frags\n");
	gpEngine->pfnEmitSound (self, CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE);
	set_suicide_frame (self->v);
};

/*
============
ClientKill

Player entered the suicide command
============
*/
void CGameClientEventListener::OnClientKill(edict_t *self) // TODO: -> OnClientCommand?
{
	bprint (PRINT_MEDIUM, self->v.netname);
	bprint (PRINT_MEDIUM, " suicides\n");
	
	set_suicide_frame (self->v);
	
	//self->v.modelindex = modelindex_player;
	//logfrag(self, self);
	//self->v.frags -= 2; // extra penalty
	//respawn(&self->v);
};

/*
===========
ClientPutInServer

called each time a player enters a new level
============
*/
void CGameClientEventListener::OnClientPutInServer(edict_t *client)
{
	entvars_t spot;
	string	s;

	client->v.classname = "player";
	client->v.health = 100;
	client->v.takedamage = DAMAGE_AIM;
	client->v.solid = SOLID_SLIDEBOX;
	client->v.movetype = MOVETYPE_WALK;
	client->v.show_hostile = 0;
	client->v.max_health = 100;
	client->v.flags = FL_CLIENT;
	client->v.air_finished = time + 12;
	client->v.dmg = 2;                   // initial water damage
	client->v.super_damage_finished = 0;
	client->v.radsuit_finished = 0;
	client->v.invisible_finished = 0;
	client->v.invincible_finished = 0;
	client->v.effects = 0;
	client->v.invincible_time = 0;

	DecodeLevelParms ();
	
	W_SetCurrentAmmo (client);

	client->v.attack_finished = time;
	client->v.th_pain = player_pain;
	client->v.th_die = PlayerDie;
	
	client->v.deadflag = DEAD_NO;
// paustime is set by teleporters to keep the player from moving a while
	client->v.pausetime = 0;
	
	spot = SelectSpawnPoint ();

	client->v.origin = spot.origin + '0 0 1';
	client->v.angles = spot.angles;
	client->v.fixangle = TRUE;           // turn this way immediately

// oh, this is a hack!
	gpEngine->SetModel (client, "models/eyes.mdl");
	modelindex_eyes = client->v.modelindex;

	gpEngine->SetModel (client, "models/player.mdl");
	modelindex_player = client->v.modelindex;

	gpEngine->SetSize (client, VEC_HULL_MIN, VEC_HULL_MAX);
	
	client->v.view_ofs = '0 0 22';

// Mod - Xian (May.20.97)
// Bug where player would have velocity from their last kill

	client->v.velocity = '0 0 0';

	player_stand1 ();
	
	makevectors(client->v.angles);
	spawn_tfog (client->v.origin + v_forward*20);

	spawn_tdeath (client->v.origin, client->v);

	// Set Rocket Jump Modifiers
	if (stof(infokey(world, "rj")) != 0)
		rj = stof(infokey(world, "rj"));

	if (deathmatch == 4)
	{
		client->v.ammo_shells = 0;
		if (stof(infokey(world, "axe")) == 0)
		{
			client->v.ammo_nails = 255;
			client->v.ammo_shells = 255;
			client->v.ammo_rockets = 255;
			client->v.ammo_cells = 255;
			
			client->v.items |= IT_NAILGUN;
			client->v.items |= IT_SUPER_NAILGUN;
			client->v.items |= IT_SUPER_SHOTGUN;
			client->v.items |= IT_ROCKET_LAUNCHER;
			//client->v.items |= IT_GRENADE_LAUNCHER;
			client->v.items |= IT_LIGHTNING;
		};
		client->v.items = client->v.items - (client->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		client->v.armorvalue = 200;
		client->v.armortype = 0.8;
		client->v.health = 250;
		client->v.items = client->v.items | IT_INVULNERABILITY;
		client->v.invincible_time = 1;
		client->v.invincible_finished = time + 3;
	};

	if (deathmatch == 5)
	{
		client->v.ammo_nails = 80;
		client->v.ammo_shells = 30;
		client->v.ammo_rockets = 10;
		client->v.ammo_cells = 30;
		client->v.items = client->v.items | IT_NAILGUN;
		client->v.items = client->v.items | IT_SUPER_NAILGUN;
		client->v.items = client->v.items | IT_SUPER_SHOTGUN;
		client->v.items = client->v.items | IT_ROCKET_LAUNCHER;
		client->v.items = client->v.items | IT_GRENADE_LAUNCHER;
		client->v.items = client->v.items | IT_LIGHTNING;
		client->v.items = client->v.items - (client->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		client->v.armorvalue = 200;
		client->v.armortype = 0.8;
		client->v.health = 200;
		client->v.items = client->v.items | IT_INVULNERABILITY;
		client->v.invincible_time = 1;
		client->v.invincible_finished = time + 3;
	};
};

void CGameClientEventListener::OnClientCommand(edict_t *pclent)
{
	// TODO
};

void CGameClientEventListener::OnClientUserInfoChanged(edict_t *pclent, char *userinfo)
{
	// TODO
};