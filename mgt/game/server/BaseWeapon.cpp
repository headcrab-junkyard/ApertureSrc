/// @file

#include "Weapon.hpp"

/*
============
W_Attack

An attack impulse can be triggered now
============
*/
void  player_axe1();
void  player_axeb1();
void  player_axec1();
void  player_axed1();
void  player_shot1();
void  player_nail1();
void  player_light1();
void  player_rocket1();

void CPlayer::Attack()
{
	if(!W_CheckNoAmmo())
		return;

	makevectors(self.v_angle); // calculate forward angle for velocity
	self.show_hostile = time + 1.0f; // wake monsters up

	mpCurrentWeapon->OnAttack();
};

void CWeaponAxe::OnAttack()
{
	if(self.weapon == IT_AXE)
	{
		self.attack_finished = time + 0.5f;
		gpEngine->pfnEmitSound (self, CHAN_WEAPON, "weapons/ax1.wav", 1, ATTN_NORM);
		
		float r = random();
		
		if (r < 0.25f)
			player_axe1 ();
		else if (r < 0.5f)
			player_axeb1 ();
		else if (r < 0.75f)
			player_axec1 ();
		else
			player_axed1 ();
	};
};

void CWeaponShotgun::OnAttack()
{
	if (self.weapon == IT_SHOTGUN)
	{
		player_shot1 ();
		self.attack_finished = time + 0.5f;
		W_FireShotgun ();
	};
};

void CWeaponSuperShotgun::OnAttack()
{
	if (self.weapon == IT_SUPER_SHOTGUN)
	{
		player_shot1 ();
		self.attack_finished = time + 0.7f;
		W_FireSuperShotgun ();
	};
};

void CWeaponNailgun::OnAttack()
{
	if(self.weapon == IT_NAILGUN)
		player_nail1 ();
};

void CWeaponSuperNailgun::OnAttack()
{
	if (self.weapon == IT_SUPER_NAILGUN)
		player_nail1 ();
};

void CWeaponGrenadeLauncher::OnAttack()
{
	if(self.weapon == IT_GRENADE_LAUNCHER)
	{
		player_rocket1();
		self.attack_finished = time + 0.6f;
		W_FireGrenade();
	};
};

void CWeaponRocketLauncher::OnAttack()
{
	if (self.weapon == IT_ROCKET_LAUNCHER)
	{
		player_rocket1();
		self.attack_finished = time + 0.8f;
		W_FireRocket();
	};
};

void CWeaponLightning::OnAttack()
{
	if(mpOwner->GetCurrentWeapon()->meType == IT_LIGHTNING)
	{
		self.attack_finished = time + 0.1f;
		gpEngine->pfnEmitSound (self, CHAN_AUTO, "weapons/lstart.wav", 1, ATTN_NORM);
		player_light1();
	};
};

//q2
/*
================
Weapon_Generic

A generic function to handle the basics of weapon thinking
================
*/
#define FRAME_FIRE_FIRST		(FRAME_ACTIVATE_LAST + 1)
#define FRAME_IDLE_FIRST		(FRAME_FIRE_LAST + 1)
#define FRAME_DEACTIVATE_FIRST	(FRAME_IDLE_LAST + 1)

void Weapon_Generic (edict_t *ent, int FRAME_ACTIVATE_LAST, int FRAME_FIRE_LAST, int FRAME_IDLE_LAST, int FRAME_DEACTIVATE_LAST, int *pause_frames, int *fire_frames, void (*fire)(edict_t *ent))
{
	int		n;

	if(ent->deadflag || ent->s.modelindex != 255) // VWep animations screw up corpses
	{
		return;
	}

	if (ent->client->weaponstate == WEAPON_DROPPING)
	{
		if (ent->client->ps.gunframe == FRAME_DEACTIVATE_LAST)
		{
			ChangeWeapon (ent);
			return;
		}
		else if ((FRAME_DEACTIVATE_LAST - ent->client->ps.gunframe) == 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4+1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304+1;
				ent->client->anim_end = FRAME_pain301;
				
			}
		}

		ent->client->ps.gunframe++;
		return;
	}

	if (ent->client->weaponstate == WEAPON_ACTIVATING)
	{
		if (ent->client->ps.gunframe == FRAME_ACTIVATE_LAST)
		{
			ent->client->weaponstate = WEAPON_READY;
			ent->client->ps.gunframe = FRAME_IDLE_FIRST;
			return;
		}

		ent->client->ps.gunframe++;
		return;
	}

	if ((ent->client->newweapon) && (ent->client->weaponstate != WEAPON_FIRING))
	{
		ent->client->weaponstate = WEAPON_DROPPING;
		ent->client->ps.gunframe = FRAME_DEACTIVATE_FIRST;

		if ((FRAME_DEACTIVATE_LAST - FRAME_DEACTIVATE_FIRST) < 4)
		{
			ent->client->anim_priority = ANIM_REVERSE;
			if(ent->client->ps.pmove.pm_flags & PMF_DUCKED)
			{
				ent->s.frame = FRAME_crpain4+1;
				ent->client->anim_end = FRAME_crpain1;
			}
			else
			{
				ent->s.frame = FRAME_pain304+1;
				ent->client->anim_end = FRAME_pain301;
				
			}
		}
		return;
	}

	if (ent->client->weaponstate == WEAPON_READY)
	{
		if ( ((ent->client->latched_buttons|ent->client->buttons) & BUTTON_ATTACK) )
		{
			ent->client->latched_buttons &= ~BUTTON_ATTACK;
			if ((!ent->client->ammo_index) || 
				( ent->client->pers.inventory[ent->client->ammo_index] >= ent->client->pers.weapon->quantity))
			{
				ent->client->ps.gunframe = FRAME_FIRE_FIRST;
				ent->client->weaponstate = WEAPON_FIRING;

				// start the animation
				ent->client->anim_priority = ANIM_ATTACK;
				if (ent->client->ps.pmove.pm_flags & PMF_DUCKED)
				{
					ent->s.frame = FRAME_crattak1-1;
					ent->client->anim_end = FRAME_crattak9;
				}
				else
				{
					ent->s.frame = FRAME_attack1-1;
					ent->client->anim_end = FRAME_attack8;
				}
			}
			else
			{
				if (level.time >= ent->pain_debounce_time)
				{
					gi.sound(ent, CHAN_VOICE, gi.soundindex("weapons/noammo.wav"), 1, ATTN_NORM, 0);
					ent->pain_debounce_time = level.time + 1;
				}
				NoAmmoWeaponChange (ent);
			}
		}
		else
		{
			if (ent->client->ps.gunframe == FRAME_IDLE_LAST)
			{
				ent->client->ps.gunframe = FRAME_IDLE_FIRST;
				return;
			}

			if (pause_frames)
			{
				for (n = 0; pause_frames[n]; n++)
				{
					if (ent->client->ps.gunframe == pause_frames[n])
					{
						if (rand()&15)
							return;
					}
				}
			}

			ent->client->ps.gunframe++;
			return;
		}
	}

	if (ent->client->weaponstate == WEAPON_FIRING)
	{
		for (n = 0; fire_frames[n]; n++)
		{
			if (ent->client->ps.gunframe == fire_frames[n])
			{
				if (ent->client->quad_framenum > level.framenum)
					gi.sound(ent, CHAN_ITEM, gi.soundindex("items/damage3.wav"), 1, ATTN_NORM, 0);

				fire (ent);
				break;
			}
		}

		if (!fire_frames[n])
			ent->client->ps.gunframe++;

		if (ent->client->ps.gunframe == FRAME_IDLE_FIRST+1)
			ent->client->weaponstate = WEAPON_READY;
	}
}