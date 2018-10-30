void CWeaponLightning::OnAttack()
{
	if(mpOwner->GetCurrentWeapon()->meType == IT_LIGHTNING)
	{
		self.attack_finished = time + 0.1f;
		gpEngine->pfnEmitSound(self, CHAN_AUTO, "weapons/lstart.wav", 1, ATTN_NORM);
		player_light1();
	};
};

/*
===============================================================================
LIGHTNING
===============================================================================
*/

void LightningHit(entity from, float damage)
{
	gpEngine->pfnWriteByte(MSG_MULTICAST, SVC_TEMPENTITY);
	gpEngine->pfnWriteByte(MSG_MULTICAST, TE_LIGHTNINGBLOOD);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_x);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_y);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_z);
	multicast(trace_endpos, MULTICAST_PVS);

	T_Damage(trace_ent, from, from, damage);
};

/*
=================
LightningDamage
=================
*/
void LightningDamage(vector p1, vector p2, entity from, float damage)
{
	entity e1, e2;
	vector f;

	f = p2 - p1;
	normalize(f);
	f_x = 0 - f_y;
	f_y = f_x;
	f_z = 0;
	f = f * 16;

	e1 = e2 = world;

	traceline(p1, p2, FALSE, self);

	if(trace_ent.takedamage)
	{
		LightningHit(from, damage);
		if(self.classname == "player")
		{
			if(other.classname == "player")
				trace_ent.velocity_z = trace_ent.velocity_z + 400;
		}
	}
	e1 = trace_ent;

	traceline(p1 + f, p2 + f, FALSE, self);
	if(trace_ent != e1 && trace_ent.takedamage)
	{
		LightningHit(from, damage);
	}
	e2 = trace_ent;

	traceline(p1 - f, p2 - f, FALSE, self);
	if(trace_ent != e1 && trace_ent != e2 && trace_ent.takedamage)
	{
		LightningHit(from, damage);
	}
};

void W_FireLightning()
{
	vector org;
	float cells;

	if(self.ammo_cells < 1)
	{
		self.weapon = W_BestWeapon();
		W_SetCurrentAmmo();
		return;
	}

	// explode if under water
	if(self.waterlevel > 1)
	{
		if(deathmatch > 3)
		{
			if(random() <= 0.5)
			{
				self.deathtype = "selfwater";
				T_Damage(self, self, self.owner, 4000);
			}
			else
			{
				cells = self.ammo_cells;
				self.ammo_cells = 0;
				W_SetCurrentAmmo();
				T_RadiusDamage(self, self, 35 * cells, world, "");
				return;
			}
		}
		else
		{
			cells = self.ammo_cells;
			self.ammo_cells = 0;
			W_SetCurrentAmmo();
			T_RadiusDamage(self, self, 35 * cells, world, "");
			return;
		}
	}

	if(self.t_width < time)
	{
		gpEngine->pfnEmitSound(self, CHAN_WEAPON, "weapons/lhit.wav", 1, ATTN_NORM);
		self.t_width = time + 0.6;
	}
	msg_entity = self;
	gpEngine->pfnWriteByte(MSG_ONE, SVC_SMALLKICK);

	if(deathmatch != 4)
		self.currentammo = self.ammo_cells = self.ammo_cells - 1;

	org = self.origin + '0 0 16';

	traceline(org, org + v_forward * 600, TRUE, self);

	gpEngine->pfnWriteByte(MSG_MULTICAST, SVC_TEMPENTITY);
	gpEngine->pfnWriteByte(MSG_MULTICAST, TE_LIGHTNING2);
	gpEngine->pfnWriteEntity(MSG_MULTICAST, self);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, org_x);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, org_y);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, org_z);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_x);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_y);
	gpEngine->pfnWriteCoord(MSG_MULTICAST, trace_endpos_z);
	multicast(org, MULTICAST_PHS);

	LightningDamage(self.origin, trace_endpos + v_forward * 4, self, 30);
};