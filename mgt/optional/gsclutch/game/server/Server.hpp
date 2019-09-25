/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
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

extern edict_t *sv_player;

struct server_t
{
	int num_edicts;
	int max_edicts;
	edict_t *edicts;      // can NOT be array indexed, because
	                      // edict_t is variable sized, but can
	                      // be used to reference the world ent
};

typedef struct client_s
{
	edict_t *edict; // EDICT_NUM(clientnum+1)
} client_t;

extern server_t sv;

//===========================================================

void SV_StartSound(edict_t *entity, int channel, const char *sample, int volume, float attenuation);

void SV_Physics(double frametime);
bool SV_RunThink(edict_t *ent);

bool SV_CheckBottom(edict_t *ent);
bool SV_movestep(edict_t *ent, vec3_t move, bool relink);

void SV_WriteClientdataToMessage(edict_t *ent, sizebuf_t *msg);

void SV_MoveToGoal(edict_t *ent, float dist);