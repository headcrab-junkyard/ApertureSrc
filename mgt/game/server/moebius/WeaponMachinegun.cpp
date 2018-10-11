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

MACHINEGUN

======================================================================
*/

void Machinegun_Fire(edict_t *ent)
{
	int i;
	vec3_t start;
	vec3_t forward, right;
	vec3_t angles;
	int damage = 8;
	int kick = 2;
	vec3_t offset;

	if(!(ent->client->buttons & BUTTON_ATTACK))
	{
		ent->client->machinegun_shots = 0;
		ent->client->ps.gunframe++;
		return;
	}

	if(ent->client->ps.gunframe == 5)
		ent->client->ps.gunframe = 4;
	else
		ent->client->ps.gunframe = 5;

	if(ent->client->pers.inventory[ent->client->ammo_index] < 1)
	{
		ent->client->ps.gunframe = 6;
		if(level.time >= ent->pain_debounce_time)
		{
			gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
			ent->pain_debounce_time = level.time + 1;
		}
		NoAmmoWeaponChange(ent);
		return;
	}

	if(is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	for(i = 1; i < 3; i++)
	{
		ent->client->kick_origin[i] = crandom() * 0.35;
		ent->client->kick_angles[i] = crandom() * 0.7;
	}
	ent->client->kick_origin[0] = crandom() * 0.35;
	ent->client->kick_angles[0] = ent->client->machinegun_shots * -1.5;

	// raise the gun as it is firing
	if(!deathmatch->value)
	{
		ent->client->machinegun_shots++;
		if(ent->client->machinegun_shots > 9)
			ent->client->machinegun_shots = 9;
	}

	// get start / end positions
	VectorAdd(ent->client->v_angle, ent->client->kick_angles, angles);
	AngleVectors(angles, forward, right, NULL);
	VectorSet(offset, 0, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);
	fire_bullet(ent, start, forward, damage, kick, DEFAULT_BULLET_HSPREAD, DEFAULT_BULLET_VSPREAD, MOD_MACHINEGUN);

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_MACHINEGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if(!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]--;

	ent->client->anim_priority = ANIM_ATTACK;
	if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crattak1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_crattak9;
	}
	else
	{
		ent->s.frame = FRAME_attack1 - (int)(random() + 0.25);
		ent->client->anim_end = FRAME_attack8;
	};
};

void Weapon_Machinegun(edict_t *ent)
{
	static int pause_frames[] = { 23, 45, 0 };
	static int fire_frames[] = { 4, 5, 0 };

	Weapon_Generic(ent, 3, 5, 45, 49, pause_frames, fire_frames, Machinegun_Fire);
};