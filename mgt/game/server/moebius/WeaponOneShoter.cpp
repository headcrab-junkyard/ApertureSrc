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

#include "BaseWeapon.hpp"

class CWeaponRemington : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
};

/*QUAKED weapon_remington (0 .5 .8) (-16 -16 0) (16 16 32)
*/
C_EXPORT void weapon_remington(entvars_t *self)
{
	CWeaponRemington::Spawn();
};

void CWeaponRemington::Spawn()
{
	//if(deathmatch <= 3)
	{
		gpEngine->pfnPrecacheModel("models/weapons/w_remington.mdl");
		gpEngine->pfnPrecacheModel("models/weapons/v_remington.mdl");
		
		SetModel("models/weapons/v_remington.mdl");
		
		self->mnID = IT_SUPER_SHOTGUN;
		self->netname = "Double-barrelled Shotgun";
		self->SetTouchCallback(weapon_touch);
		
		SetSize('-16 -16 0', '16 16 56');
		
		StartItem(self);
	};
};

/*
======================================================================

SUPERSHOTGUN

======================================================================
*/

void CWeaponRemington::Fire(edict_t *ent)
{
	vec3_t start;
	vec3_t forward, right;
	vec3_t offset;
	vec3_t v;
	int damage = 6;
	int kick = 12;

	AngleVectors(ent->client->v_angle, forward, right, NULL);

	VectorScale(forward, -2, ent->client->kick_origin);
	ent->client->kick_angles[0] = -2;

	VectorSet(offset, 0, 8, ent->viewheight - 8);
	P_ProjectSource(ent->client, ent->s.origin, offset, forward, right, start);

	if(is_quad)
	{
		damage *= 4;
		kick *= 4;
	}

	v[PITCH] = ent->client->v_angle[PITCH];
	v[YAW] = ent->client->v_angle[YAW] - 5;
	v[ROLL] = ent->client->v_angle[ROLL];
	AngleVectors(v, forward, NULL, NULL);
	fire_shotgun(ent, start, forward, damage, kick, DEFAULT_SHOTGUN_HSPREAD, DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, MOD_SSHOTGUN);
	v[YAW] = ent->client->v_angle[YAW] + 5;
	AngleVectors(v, forward, NULL, NULL);
	fire_shotgun(ent, start, forward, damage, kick, DEFAULT_SHOTGUN_HSPREAD, DEFAULT_SHOTGUN_VSPREAD, DEFAULT_SSHOTGUN_COUNT / 2, MOD_SSHOTGUN);

	// send muzzle flash
	gi.WriteByte(svc_muzzleflash);
	gi.WriteShort(ent - g_edicts);
	gi.WriteByte(MZ_SSHOTGUN | is_silenced);
	gi.multicast(ent->s.origin, MULTICAST_PVS);

	ent->client->ps.gunframe++;
	PlayerNoise(ent, start, PNOISE_WEAPON);

	if(!((int)dmflags->value & DF_INFINITE_AMMO))
		ent->client->pers.inventory[ent->client->ammo_index] -= 2;
}

void Weapon_SuperShotgun(edict_t *ent)
{
	static int pause_frames[] = { 29, 42, 57, 0 };
	static int fire_frames[] = { 7, 0 };

	Weapon_Generic(ent, 6, 17, 57, 61, pause_frames, fire_frames, weapon_supershotgun_fire);
}