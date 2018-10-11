/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/// @file

/*
======================================================================

BLASTER

======================================================================
*/

void Blaster_Fire(edict_t *ent, vec3_t g_offset, int damage, qboolean hyper, int effect)
{
	vec3_t forward, right;
	vec3_t start;
	vec3_t offset;

	if(is_quad)
		damage *= 4;
	AngleVectors(ent->client->v_angle, forward, right, NULL);
	VectorSet(offset, 24, 8, ent->viewheight - 8);
	VectorAdd(offset, g_offset, offset);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_blaster(ent, start, forward, damage, 1000, effect, hyper);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	if(hyper)
		gi.WriteByte(MZ_HYPERBLASTER | is_silenced);
	else
		gi.WriteByte(MZ_BLASTER | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);
}

void Weapon_Blaster_Fire(edict_t *ent)
{
	int damage;

	if(deathmatch->value)
		damage = 15;
	else
		damage = 10;
	Blaster_Fire(ent, vec3_origin, damage, false, EF_BLASTER);
	ent->client->ps.gunframe++;
}

void Weapon_Blaster(edict_t *ent)
{
	static int pause_frames[] = { 19, 32, 0 };
	static int fire_frames[] = { 5, 0 };

	Weapon_Generic(ent, 4, 8, 52, 55, pause_frames, fire_frames, Weapon_Blaster_Fire);
}