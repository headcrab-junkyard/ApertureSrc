/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2017-2019 BlackPhrase
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

#include "quakedef.h"
#include "GameWorld.hpp"

CGameWorld::CGameWorld() = default;
CGameWorld::~CGameWorld() = default;

edict_t *CGameWorld::SpawnEntity()
{
	edict_t *ed = ED_Alloc();
	return ed;
};

void CGameWorld::RemoveEntity(edict_t *ed)
{
	ED_Free(ed);
};

/*
==============
PF_changelevel
==============
*/
void CGameWorld::ChangeLevel(const char *s1, const char *s2)
{
	// make sure we don't issue two changelevels
	if(svs.changelevel_issued)
		return;
	svs.changelevel_issued = true;

	if(s2 && *s2)
		//if ((int)gGlobalVariables.serverflags & (SFL_NEW_UNIT | SFL_NEW_EPISODE))
			//Cbuf_AddText (va("changelevel %s %s\n",s1, s2));
		//else
		Cbuf_AddText(va("changelevel2 %s %s\n", s1, s2));
	else
		Cbuf_AddText(va("changelevel %s\n", s1));
};

/*
===============
PF_lightstyle

void(float style, string value) lightstyle
===============
*/
void CGameWorld::SetLightStyle(/*float*/ int style, const char *val)
{
	client_t *client;
	int j;

	// change the string in sv
	sv.lightstyles[style] = val;

	// send message to all clients on this server
	if(sv.state != ss_active)
		return;

	for(j = 0, client = svs.clients; j < svs.maxclients; j++, client++)
		if(client->active || client->spawned)
		{
			MSG_WriteChar(&client->netchan.message, svc_lightstyle);
			MSG_WriteChar(&client->netchan.message, style);
			MSG_WriteString(&client->netchan.message, val);
		};
};

/*
=================
PF_particle

particle(origin, color, count)
=================
*/
void CGameWorld::SpawnParticle(float *org, float *dir, float color, float count)
{
	SV_StartParticle(org, dir, color, count);
};

/*
=================
PF_ambientsound

=================
*/
void CGameWorld::AmbientSound(float *pos, const char *samp, float vol, float attenuation)
{
	char **check;
	int i, soundnum;

	// check to see if samp was properly precached
	for(soundnum = 0, check = sv.sound_precache; *check; check++, soundnum++)
		if(!strcmp(*check, samp))
			break;

	if(!*check)
	{
		gpSystem->Printf("no precache: %s\n", samp);
		return;
	};

	// add an svc_spawnambient command to the level signon packet

	MSG_WriteByte(&sv.signon, svc_spawnstaticsound);
	for(i = 0; i < 3; i++)
		MSG_WriteCoord(&sv.signon, pos[i]);

	MSG_WriteByte(&sv.signon, soundnum);

	MSG_WriteByte(&sv.signon, vol * 255);
	MSG_WriteByte(&sv.signon, attenuation * 64);
};

/*
=================
PF_findradius

Returns a chain of entities that have origins within a spherical area

findradius (origin, radius)
=================
*/
edict_t *CGameWorld::FindEntitiesInRadius(float *org, float rad)
{
	edict_t *ent, *chain;
	vec3_t eorg;
	int i, j;

	chain = (edict_t *)sv.edicts;

	ent = NEXT_EDICT(sv.edicts);
	for(i = 1; i < sv.num_edicts; i++, ent = NEXT_EDICT(ent))
	{
		if(ent->free)
			continue;
		if(ent->v.solid == SOLID_NOT)
			continue;
		for(j = 0; j < 3; j++)
			eorg[j] = org[j] - (ent->v.origin[j] + (ent->v.mins[j] + ent->v.maxs[j]) * 0.5);
		if(Length(eorg) > rad)
			continue;

		ent->v.chain = EDICT_TO_PROG(chain);
		chain = ent;
	};

	return chain;
};