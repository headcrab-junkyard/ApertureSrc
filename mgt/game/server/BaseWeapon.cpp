/// @file

#include "BaseWeapon.hpp"

/*
============
W_Attack

An attack impulse can be triggered now
============
*/

void  player_shot1();
void  player_nail1();
void  player_light1();
void  player_rocket1();

CBaseWeapon::CBaseWeapon() = default;
CBaseWeapon::~CBaseWeapon() = default;

void CBaseWeapon::Spawn()
{
};

/*
================
idWeapon::SetOwner

Only called at player spawn time, not each weapon switch
================
*/
void CBaseWeapon::SetOwner( CBasePlayer *_owner )
{
	assert( !owner );
	owner = _owner;
	SetName( va( "%s_weapon", owner->name.c_str() ) );

	if ( worldModel.GetEntity() )
		worldModel.GetEntity()->SetName( va( "%s_weapon_worldmodel", owner->name.c_str() ) );
};

/*
================
idWeapon::Raise
================
*/
void CBaseWeapon::Raise()
{
	if ( isLinked )
		WEAPON_RAISEWEAPON = true;
};

/*
================
idWeapon::PutAway
================
*/
void CBaseWeapon::PutAway()
{
	hasBloodSplat = false;
	if ( isLinked )
		WEAPON_LOWERWEAPON = true;
};

/*
================
idWeapon::Reload
NOTE: this is only for impulse-triggered reload, auto reload is scripted
================
*/
void CBaseWeapon::Reload()
{
	if ( isLinked )
		WEAPON_RELOAD = true;
};

/*
================
idWeapon::LowerWeapon
================
*/
void CBaseWeapon::LowerWeapon()
{
	if ( !hide )
	{
		hideStart	= 0.0f;
		hideEnd		= hideDistance;
		
		if ( gameLocal.time - hideStartTime < hideTime )
			hideStartTime = gameLocal.time - ( hideTime - ( gameLocal.time - hideStartTime ) );
		else
			hideStartTime = gameLocal.time;
		
		hide = true;
	};
};

/*
================
idWeapon::RaiseWeapon
================
*/
void CBaseWeapon::RaiseWeapon()
{
	Show();

	if ( hide )
	{
		hideStart	= hideDistance;
		hideEnd		= 0.0f;
		
		if ( gameLocal.time - hideStartTime < hideTime )
			hideStartTime = gameLocal.time - ( hideTime - ( gameLocal.time - hideStartTime ) );
		else
			hideStartTime = gameLocal.time;
		
		hide = false;
	};
};

/*
================
idWeapon::HideWeapon
================
*/
void CBaseWeapon::HideWeapon()
{
	Hide();
	
	if ( worldModel.GetEntity() )
		worldModel.GetEntity()->Hide();
	
	muzzleFlashEnd = 0;
};

/*
================
idWeapon::ShowWeapon
================
*/
void CBaseWeapon::ShowWeapon()
{
	Show();
	
	if ( worldModel.GetEntity() )
		worldModel.GetEntity()->Show();
	
	if ( lightOn )
		MuzzleFlashLight();
};

/*
================
idWeapon::BeginAttack
================
*/
void CBaseWeapon::BeginAttack()
{
	if ( status != WP_OUTOFAMMO )
		lastAttack = gameLocal.time;

	if ( !isLinked )
		return;

	if ( !WEAPON_ATTACK )
	{
		if ( sndHum && grabberState == -1 )	// _D3XP :: don't stop grabber hum
			StopSound( SND_CHANNEL_BODY, false );
	};
	
	WEAPON_ATTACK = true;
};

/*
================
idWeapon::EndAttack
================
*/
void CBaseWeapon::EndAttack()
{
	if ( !WEAPON_ATTACK.IsLinked() )
		return;

	if ( WEAPON_ATTACK )
	{
		WEAPON_ATTACK = false;
		if ( sndHum && grabberState == -1 )	// _D3XP :: don't stop grabber hum
			StartSoundShader( sndHum, SND_CHANNEL_BODY, 0, false, NULL );
	};
};

/*
================
idWeapon::IsReloading
================
*/
bool CBaseWeapon::IsReloading() const
{
	return status == WP_RELOAD;
};

/*
================
idWeapon::IsHolstered
================
*/
bool CBaseWeapon::IsHolstered() const
{
	return status == WP_HOLSTERED;
};

/*
================
idWeapon::GetAmmoType
================
*/
ammo_t CBaseWeapon::GetAmmoType() const
{
	return ammoType;
};

/*
================
idWeapon::ClipSize
================
*/
int	CBaseWeapon::ClipSize() const
{
	return clipSize;
};

/*
================
idWeapon::AmmoInClip
================
*/
int CBaseWeapon::AmmoInClip() const
{
	return ammoClip.Get();
};

void CBasePlayer::Attack()
{
	if(!W_CheckNoAmmo())
		return;

	makevectors(self.v_angle); // calculate forward angle for velocity
	self.show_hostile = time + 1.0f; // wake monsters up

	mpCurrentWeapon->OnAttack();
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