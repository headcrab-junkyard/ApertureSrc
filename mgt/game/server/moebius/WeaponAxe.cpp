/// @file

void player_axe1();
void player_axeb1();
void player_axec1();
void player_axed1();

class CWeaponAxe : public CBaseWeapon
{
public:
	CWeaponAxe();
	~CWeaponAxe();

	void OnAttack() override;

private:
};

void CWeaponAxe::OnAttack()
{
	if(self.weapon == IT_AXE)
	{
		self.attack_finished = time + 0.5f;
		gpEngine->pfnEmitSound(self, CHAN_WEAPON, "weapons/ax1.wav", 1, ATTN_NORM);

		float r = random();

		if(r < 0.25f)
			player_axe1();
		else if(r < 0.5f)
			player_axeb1();
		else if(r < 0.75f)
			player_axec1();
		else
			player_axed1();
	};
};

/*
================
W_FireAxe
================
*/
void W_FireAxe()
{
	vector source;
	vector org;

	makevectors(self.v_angle);
	source = self.origin + '0 0 16';

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
		gpEngine->pfnEmitSound(self, CHAN_WEAPON, "player/axhit2.wav", 1, ATTN_NORM);

		gpEngine->pfnWriteByte(MSG_MULTICAST, SVC_TEMPENTITY);
		gpEngine->pfnWriteByte(MSG_MULTICAST, TE_GUNSHOT);
		gpEngine->pfnWriteByte(MSG_MULTICAST, 3);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_x);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_y);
		gpEngine->pfnWriteCoord(MSG_MULTICAST, org_z);
		multicast(org, MULTICAST_PVS);
	};
};