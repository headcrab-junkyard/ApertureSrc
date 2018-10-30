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

class CWeaponOneShoter : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override; // TODO: Fire?
	void SecondaryAttack() override; // TODO: AltFire?
};

/*QUAKED weapon_oneshoter (0 .5 .8) (-16 -16 0) (16 16 32)
*/
C_EXPORT void weapon_oneshoter(entvars_t *self)
{
	CWeaponOneShoter::Spawn();
};

void CWeaponOneShoter::Spawn()
{
	//if(deathmatch <= 3)
	{
		gpEngine->pfnPrecacheModel("models/weapons/w_oneshoter.mdl");
		gpEngine->pfnPrecacheModel("models/weapons/v_oneshoter.mdl");
		
		SetModel("models/weapons/v_oneshoter.mdl");

		self->mnID = WEAPON_ONESHOTER;
		self->netname = "Double-barrelled Shotgun";
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
void CWeaponOneShoter::PrimaryAttack() // TODO: OnAttack?
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

void CWeaponOneShoter::SecondaryAttack()
{
	// TODO
};