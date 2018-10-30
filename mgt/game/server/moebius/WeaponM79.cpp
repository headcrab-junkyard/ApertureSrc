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

/*QUAKED weapon_grenadelauncher (0 .5 .8) (-16 -16 0) (16 16 32)
*/
C_EXPORT void weapon_grenadelauncher(entvars_t *self)
{
	CWeaponGrenadeLauncher::Spawn();
};

/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void CWeaponGrenadeLauncher::Spawn()
{
	if(deathmatch <= 3)
	{
		precache_model("models/weapons/v_m79.mdl");
		self->SetModel("models/weapons/v_m79.mdl");
		self->weapon = 3;
		self->netname = "Grenade Launcher";
		self->SetTouchCallback(weapon_touch);
		self->SetSize('-16 -16 0', '16 16 56');
		StartItem(self);
	};
};

void CWeaponGrenadeLauncher::OnAttack()
{
	player_rocket1();
	mpOwner.attack_finished = time + 0.6f;
	W_FireGrenade();
};

void weapon_grenadelauncher_fire(edict_t *ent)
{
	vec3_t offset;
	vec3_t forward, right;
	vec3_t start;
	int damage = 120;
	float radius;

	radius = damage + 40;
	if(is_quad)
		damage *= 4;

	VectorSet(offset, 8, 8, ent->viewheight - 8);
	AngleVectors(ent->client->v_angle, forward, right, nullptr);
	
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -1;

	fire_grenade(ent, start, forward, damage, 600, 2.5, radius);

	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_GRENADE | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;

	PlayerNoise(ent, start, PNOISE_WEAPON);

	if(!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index]--;
};

void Weapon_GrenadeLauncher(edict_t *ent)
{
	static int pause_frames[] = { 34, 51, 59, 0 };
	static int fire_frames[] = { 6, 0 };

	Weapon_Generic(ent, 5, 16, 59, 64, pause_frames, fire_frames, weapon_grenadelauncher_fire);
};