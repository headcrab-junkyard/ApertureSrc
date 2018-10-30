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

class CWeaponGlock : public CBaseWeapon
{
public:
	void Fire() override;
	void SecondaryAttack() override;
private:
	void ToggleFireMode();
	
	int mnFireMode{0};
};

void CWeaponGlock::OnAttack()
{
	player_shot1();
	self.attack_finished = time + 0.2f;
	CWeaponGlock::Fire();
};

void CWeaponGlock::SecondaryAttack()
{
	ToggleFireMode();
};

void CWeaponGlock::ToggleFireMode()
{
	// BP: actually it can be implemented as bool but will leave that for now
	
	if(mnFireMode == 1)
		mnFireMode = 2;
	else
		mnFireMode = 1;
};

/*
================
W_FireShotgun
================
*/
void CWeaponGlock::Fire()
{
	vec3_t dir;

	self->EmitSound(CHAN_WEAPON, "weapons/guncock.wav", 1, ATTN_NORM);

	msg_entity = self;
	WriteByte(MSG_ONE, SVC_SMALLKICK);

	if(deathmatch != 4)
		self.currentammo = self.ammo_shells = self.ammo_shells - 1;

	dir = aim(self, 100000);
	
	int nBulletCount{1};
	
	if(mnFireMode == 2)
		nBulletCount = 3;

	FireBullets(nBulletCount, dir, '0.04 0.04 0');
};