/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

#include "BaseWeapon.hpp"

class CWeaponM16 : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
};

/*QUAKED weapon_m16 (0 .5 .8) (-16 -16 0) (16 16 32)
*/
C_EXPORT void weapon_m16(entvars_t *self)
{
	CWeaponM16::Spawn();
};

void CWeaponM16::Spawn()
{
	//if(deathmatch <= 3)
	{
		gpEngine->pfnPrecacheModel("models/weapons/w_m16.mdl");
		gpEngine->pfnPrecacheModel("models/weapons/v_m16.mdl");
		
		SetModel("models/weapons/v_m16.mdl");

		self->mnID = WEAPON_M16;
		self->netname = "M16";
		SetTouchCallback(weapon_touch);

		SetSize('-16 -16 0', '16 16 56');

		StartItem(self);
	};
};

/*
================
W_FireShotgun
================
*/
void CWeaponM16::PrimaryAttack() // TODO: OnAttack?
{
	player_shot1();
	self->attack_finished = gpGlobals->time + 0.2f;

	EmitSound(CHAN_WEAPON, "weapons/guncock.wav", 1, ATTN_NORM);

	msg_entity = self;
	WriteByte(MSG_ONE, SVC_SMALLKICK);

	if(deathmatch != 4)
		self.currentammo = self.ammo_shells = self.ammo_shells - 1;

	vec3_t dir = aim(self, 100000);
	mpOwner->FireBullets(1, dir, '0.04 0.04 0');
};