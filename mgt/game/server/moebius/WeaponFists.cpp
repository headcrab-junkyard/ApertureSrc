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

void player_axe1();
void player_axeb1();
void player_axec1();
void player_axed1();

class CWeaponFists : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void OnAttack() override;
};

void CWeaponFists::Spawn()
{
};

void CWeaponFists::OnAttack()
{
	self->attack_finished = gpGlobals->time + 0.5f;
	EmitSound(CHAN_WEAPON, "weapons/fists1.wav", 1, ATTN_NORM);

	float r = random();

	if(r < 0.25f)
		player_axe1(mpOwner);
	else if(r < 0.5f)
		player_axeb1(mpOwner);
	else if(r < 0.75f)
		player_axec1(mpOwner);
	else
		player_axed1(mpOwner);
};

/*
================
W_FireAxe
================
*/
void W_FireAxe()
{
	vec3_t source;
	vec3_t org;

	makevectors(self->v_angle);
	source = self->GetOrigin() + '0 0 16';

	traceline(source, source + v_forward * 64, FALSE, self);

	if(trace_fraction == 1.0)
		return;

	org = trace_endpos - v_forward * 4;

	if(trace_ent.takedamage)
	{
		trace_ent.axhitme = 1;
		
		SpawnBlood(org, 20);
		
		if(deathmatch > 3)
			T_Damage(trace_ent, self, self, 75);
		else
			T_Damage(trace_ent, self, self, 20);
	}
	else
	{
		// hit wall
		self->EmitSound(CHAN_WEAPON, "player/fisthit2.wav", 1, ATTN_NORM);

		gpEngine->pfnWriteByte(MSG_MULTICAST, SVC_TEMPENTITY);
		gpEngine->pfnWriteByte(MSG_MULTICAST, TE_GUNSHOT);
		gpEngine->pfnWriteByte(MSG_MULTICAST, 3);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_x);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_y);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_z);
		multicast(org, MULTICAST_PVS);
	};
};