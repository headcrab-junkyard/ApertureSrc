/*
===========================================================================

Doom 3 BFG Edition GPL Source Code
Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 BFG Edition GPL Source Code ("Doom 3 BFG Edition Source Code").  

Doom 3 BFG Edition Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 BFG Edition Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 BFG Edition Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 BFG Edition Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/

/// @file

#include "BasePlayerShared.hpp"

/*
#include "../idlib/precompiled.h"
#include "Game_local.h"
#include "../framework/Common_local.h"
#include "PredictedValue_impl.h"
*/

idCVar flashlight_batteryDrainTimeMS("flashlight_batteryDrainTimeMS", "30000", CVAR_INTEGER, "amount of time (in MS) it takes for full battery to drain (-1 == no battery drain)");
idCVar flashlight_batteryChargeTimeMS("flashlight_batteryChargeTimeMS", "3000", CVAR_INTEGER, "amount of time (in MS) it takes to fully recharge battery");
idCVar flashlight_minActivatePercent("flashlight_minActivatePercent", ".25", CVAR_FLOAT, "( 0.0 - 1.0 ) minimum amount of battery (%) needed to turn on flashlight");
idCVar flashlight_batteryFlickerPercent("flashlight_batteryFlickerPercent", ".1", CVAR_FLOAT, "chance of flickering when battery is low");

// No longer userinfo, but I don't want to rename the cvar
idCVar ui_showGun("ui_showGun", "1", CVAR_GAME | CVAR_ARCHIVE | CVAR_BOOL, "show gun");

// Client-authoritative stuff
idCVar pm_clientAuthoritative_debug("pm_clientAuthoritative_debug", "0", CVAR_BOOL, "");
idCVar pm_controllerShake_damageMaxMag("pm_controllerShake_damageMaxMag", "60.0f", CVAR_FLOAT, "");
idCVar pm_controllerShake_damageMaxDur("pm_controllerShake_damageMaxDur", "60.0f", CVAR_FLOAT, "");

idCVar pm_clientAuthoritative_warnDist("pm_clientAuthoritative_warnDist", "100.0f", CVAR_FLOAT, "");
idCVar pm_clientAuthoritative_minDistZ("pm_clientAuthoritative_minDistZ", "1.0f", CVAR_FLOAT, "");
idCVar pm_clientAuthoritative_minDist("pm_clientAuthoritative_minDist", "-1.0f", CVAR_FLOAT, "");
idCVar pm_clientAuthoritative_Lerp("pm_clientAuthoritative_Lerp", "0.9f", CVAR_FLOAT, "");

idCVar pm_clientAuthoritative_Divergence("pm_clientAuthoritative_Divergence", "200.0f", CVAR_FLOAT, "");
idCVar pm_clientInterpolation_Divergence("pm_clientInterpolation_Divergence", "5000.0f", CVAR_FLOAT, "");

idCVar pm_clientAuthoritative_minSpeedSquared("pm_clientAuthoritative_minSpeedSquared", "1000.0f", CVAR_FLOAT, "");

extern idCVar g_demoMode;

/*
===============================================================================

	Player control of the Doom Marine.
	This object handles all player movement and world interaction.

===============================================================================
*/

// distance between ladder rungs (actually is half that distance, but this sounds better)
const int LADDER_RUNG_DISTANCE = 32;

// amount of health per dose from the health station
const int HEALTH_PER_DOSE = 10;

// time before a weapon dropped to the floor disappears
const int WEAPON_DROP_TIME = 20 * 1000;

// time before a next or prev weapon switch happens
const int WEAPON_SWITCH_DELAY = 150;

// how many units to raise spectator above default view height so it's in the head of someone
const int SPECTATE_RAISE = 25;

const int HEALTHPULSE_TIME = 333;

// minimum speed to bob and play run/walk animations at
const float MIN_BOB_SPEED = 5.0f;

// Special team used for spectators that we ONLY store on lobby.  The local team property on player remains as 0 or 1.
const float LOBBY_SPECTATE_TEAM_FOR_VOICE_CHAT = 2;

const idEventDef EV_Player_GetButtons("getButtons", nullptr, 'd');
const idEventDef EV_Player_GetMove("getMove", nullptr, 'v');
const idEventDef EV_Player_GetViewAngles("getViewAngles", nullptr, 'v');
const idEventDef EV_Player_StopFxFov("stopFxFov");
const idEventDef EV_Player_EnableWeapon("enableWeapon");
const idEventDef EV_Player_DisableWeapon("disableWeapon");
const idEventDef EV_Player_GetCurrentWeapon("getCurrentWeapon", nullptr, 's');
const idEventDef EV_Player_GetPreviousWeapon("getPreviousWeapon", nullptr, 's');
const idEventDef EV_Player_SelectWeapon("selectWeapon", "s");
const idEventDef EV_Player_GetWeaponEntity("getWeaponEntity", nullptr, 'e');
const idEventDef EV_Player_OpenPDA("openPDA");
const idEventDef EV_Player_InPDA("inPDA", nullptr, 'd');
const idEventDef EV_Player_ExitTeleporter("exitTeleporter");
const idEventDef EV_Player_StopAudioLog("stopAudioLog");
const idEventDef EV_Player_HideTip("hideTip");
const idEventDef EV_Player_LevelTrigger("levelTrigger");
const idEventDef EV_SpectatorTouch("spectatorTouch", "et");
const idEventDef EV_Player_GiveInventoryItem("giveInventoryItem", "s");
const idEventDef EV_Player_RemoveInventoryItem("removeInventoryItem", "s");
const idEventDef EV_Player_GetIdealWeapon("getIdealWeapon", nullptr, 's');
const idEventDef EV_Player_SetPowerupTime("setPowerupTime", "dd");
const idEventDef EV_Player_IsPowerupActive("isPowerupActive", "d", 'd');
const idEventDef EV_Player_WeaponAvailable("weaponAvailable", "s", 'd');
const idEventDef EV_Player_StartWarp("startWarp");
const idEventDef EV_Player_ToggleBloom("toggleBloom", "d");
const idEventDef EV_Player_SetBloomParms("setBloomParms", "ff");

CLASS_DECLARATION(CBaseActor, CBasePlayer)
EVENT(EV_Player_GetButtons, CBasePlayer::Event_GetButtons)
EVENT(EV_Player_GetMove, CBasePlayer::Event_GetMove)
EVENT(EV_Player_GetViewAngles, CBasePlayer::Event_GetViewAngles)
EVENT(EV_Player_StopFxFov, CBasePlayer::Event_StopFxFov)
EVENT(EV_Player_EnableWeapon, CBasePlayer::Event_EnableWeapon)
EVENT(EV_Player_DisableWeapon, CBasePlayer::Event_DisableWeapon)
EVENT(EV_Player_GetCurrentWeapon, CBasePlayer::Event_GetCurrentWeapon)
EVENT(EV_Player_GetPreviousWeapon, CBasePlayer::Event_GetPreviousWeapon)
EVENT(EV_Player_SelectWeapon, CBasePlayer::Event_SelectWeapon)
EVENT(EV_Player_GetWeaponEntity, CBasePlayer::Event_GetWeaponEntity)
EVENT(EV_Player_OpenPDA, CBasePlayer::Event_OpenPDA)
EVENT(EV_Player_InPDA, CBasePlayer::Event_InPDA)
EVENT(EV_Player_ExitTeleporter, CBasePlayer::Event_ExitTeleporter)
EVENT(EV_Player_StopAudioLog, CBasePlayer::Event_StopAudioLog)
EVENT(EV_Player_HideTip, CBasePlayer::Event_HideTip)
EVENT(EV_Player_LevelTrigger, CBasePlayer::Event_LevelTrigger)
EVENT(EV_Gibbed, CBasePlayer::Event_Gibbed)
EVENT(EV_Player_GiveInventoryItem, CBasePlayer::Event_GiveInventoryItem)
EVENT(EV_Player_RemoveInventoryItem, CBasePlayer::Event_RemoveInventoryItem)
EVENT(EV_Player_GetIdealWeapon, CBasePlayer::Event_GetIdealWeapon)
EVENT(EV_Player_WeaponAvailable, CBasePlayer::Event_WeaponAvailable)
EVENT(EV_Player_SetPowerupTime, CBasePlayer::Event_SetPowerupTime)
EVENT(EV_Player_IsPowerupActive, CBasePlayer::Event_IsPowerupActive)
EVENT(EV_Player_StartWarp, CBasePlayer::Event_StartWarp)
EVENT(EV_Player_ToggleBloom, CBasePlayer::Event_ToggleBloom)
EVENT(EV_Player_SetBloomParms, CBasePlayer::Event_SetBloomParms)
END_CLASS

const int MAX_RESPAWN_TIME = 10000;
const int RAGDOLL_DEATH_TIME = 3000;
const int MAX_PDAS = 64;
const int MAX_PDA_ITEMS = 128;
const int STEPUP_TIME = 200;
const int MAX_INVENTORY_ITEMS = 20;

CBasePlayer::CBasePlayer() = default;

/*
==============
CBasePlayer::~CBasePlayer()

Release any resources used by the player
==============
*/
CBasePlayer::~CBasePlayer()
{
	delete weapon.GetEntity();
	weapon = nullptr;

	delete flashlight.GetEntity();
	flashlight = nullptr;

	if(enviroSuitLight.IsValid())
		enviroSuitLight.GetEntity()->ProcessEvent(&EV_Remove);
	
	// have to do this here, idMultiplayerGame::DisconnectClient() is too late
	if(mpGame->GetRules()->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased())
		ReturnFlag();

	delete hudManager;
	hudManager = nullptr;

	delete pdaMenu;
	pdaMenu = nullptr;

	delete mpMessages;
	mpMessages = nullptr;
};

/*
==============
CBasePlayer::Spawn

Prepare any resources used by the player.
==============
*/
void CBasePlayer::Spawn()
{
	CStr temp;
	CBounds	bounds;

	if ( entityNumber >= MAX_CLIENTS )
		gpSystem->Error( "entityNum > MAX_CLIENTS for player.  Player may only be spawned with a client." );

	// allow thinking during cinematics
	cinematic = true;

	if ( mpGame->GetRules()->IsMultiplayer() )
	{
		// always start in spectating state waiting to be spawned in
		// do this before SetClipModel to get the right bounding box
		spectating = true;
	};

	// set our collision model
	physicsObj.SetSelf( this );
	SetClipModel();
	physicsObj.SetMass( spawnArgs.GetFloat( "mass", "100" ) );
	physicsObj.SetContents( CONTENTS_BODY );
	physicsObj.SetClipMask( MASK_PLAYERSOLID );
	SetPhysics( &physicsObj );
	InitAASLocation();

	skin = renderEntity.customSkin;

	// only the local player needs guis
	if ( !mpGame->GetRules()->IsMultiplayer() || IsLocallyControlled() ) {

		// load HUD
		if ( hudManager != nullptr )
		{
			hudManager->Initialize( "hud", common->SW() );
			hudManager->ActivateMenu( true );
			hud = hudManager->GetHud();
		};

		// load cursor
		if ( spawnArgs.GetString( "cursor", "", temp ) )
			cursor = uiManager->FindGui( temp, true, mpGame->GetRules()->IsMultiplayer(), mpGame->GetRules()->IsMultiplayer() );
		
		if ( cursor )
			cursor->Activate( true, gameLocal.time );

		if ( pdaMenu != nullptr )
			pdaMenu->Initialize( "pda", common->SW() );

		objectiveSystemOpen = false;
	};

	if ( mpGame->GetRules()->IsMultiplayer() && mpMessages == nullptr )
	{
		mpMessages = new idSWF( "mp_messages", common->SW() );
		mpMessages->Activate( true );
	};

	SetLastHitTime( 0 );

	// load the armor sound feedback
	declManager->FindSound( "player_sounds_hitArmor" );

	// set up conditions for animation
	LinkScriptVariables();

	animator.RemoveOriginOffset( true );

	// create combat collision hull for exact collision detection
	SetCombatModel();

	// init the damage effects
	playerView.SetPlayerEntity( this );

	// supress model in non-player views, but allow it in mirrors and remote views
	renderEntity.suppressSurfaceInViewID = entityNumber+1;

	// don't project shadow on self or weapon
	renderEntity.noSelfShadow = true;

	idAFAttachment *headEnt = head.GetEntity();
	if ( headEnt )
	{
		headEnt->GetRenderEntity()->suppressSurfaceInViewID = entityNumber+1;
		headEnt->GetRenderEntity()->noSelfShadow = true;
	};

	if ( mpGame->GetRules()->IsMultiplayer() )
	{
		Init();
		Hide();	// properly hidden if starting as a spectator
		
		if constexpr(IsServer())
		{
			// set yourself ready to spawn. idMultiplayerGame will decide when/if appropriate and call SpawnFromSpawnSpot
			SetupWeaponEntity();
			SpawnFromSpawnSpot();
			forceRespawn = true;
			wantSpectate = true;
			assert( spectating );
		};
	}
	else
	{
		SetupWeaponEntity();
		SpawnFromSpawnSpot();
	};

	// trigger playtesting item gives, if we didn't get here from a previous level
	// the devmap key will be set on the first devmap, but cleared on any level
	// transitions
	if ( !mpGame->GetRules()->IsMultiplayer() && gameLocal.serverInfo.FindKey( "devmap" ) )
	{
		// fire a trigger with the name "devmap"
		CBaseEntity *ent = gameLocal.FindEntity( "devmap" );
		if ( ent )
			ent->ActivateTargets( this );
	};

	if ( hud )
	{
		if ( weapon_soulcube > 0 && ( inventory.weapons & ( 1 << weapon_soulcube ) ) )
		{
			int max_souls = inventory.MaxAmmoForAmmoClass( this, "ammo_souls" );
			if ( inventory.GetInventoryAmmoForType( idWeapon::GetAmmoNumForName( "ammo_souls" ) ) >= max_souls )
				hud->SetShowSoulCubeOnLoad( true );
		};
	};

	if ( GetPDA() )
	{
		// Add any emails from the inventory
		for ( int i = 0; i < inventory.emails.Num(); i++ )
			GetPDA()->AddEmail( inventory.emails[i] );
		
		GetPDA()->SetSecurity( idLocalization::GetString( "#str_00066" ) );
	};

	if ( mpGame->GetWorld()->spawnArgs.GetBool( "no_Weapons" ) )
	{
		hiddenWeapon = true;
		if ( weapon.GetEntity() )
			weapon.GetEntity()->LowerWeapon();
		
		idealWeapon = weapon_fists;
	}
	else
		hiddenWeapon = false;
	
	UpdateHudWeapon();

	tipUp = false;
	objectiveUp = false;

	if ( inventory.levelTriggers.Num() )
		PostEventMS( &EV_Player_LevelTrigger, 0 );

	inventory.pdaOpened = false;
	inventory.selPDA = 0;

	if ( !mpGame->GetRules()->IsMultiplayer() )
	{
		int startingHealth = gameLocal.world->spawnArgs.GetInt( "startingHealth", health );
		if ( health > startingHealth ) {
			health = startingHealth;
		}
		if ( g_skill.GetInteger() < 2 )
		{
			if ( health < 25 ) {
				health = 25;
			}
			if ( g_useDynamicProtection.GetBool() ) {
				new_g_damageScale = 1.0f;
			}
		}
		else
		{
			new_g_damageScale = 1.0f;
			g_armorProtection.SetFloat( ( g_skill.GetInteger() < 2 ) ? 0.4f : 0.2f );
			if ( g_skill.GetInteger() == 3 )
				nextHealthTake = gameLocal.time + g_healthTakeTime.GetInteger() * 1000;
		}
	}

	//Setup the weapon toggle lists
	const CKeyValue *kv;
	kv = spawnArgs.MatchPrefix( "weapontoggle", nullptr );
	while( kv )
	{
		WeaponToggle_t newToggle;
		strcpy(newToggle.name, kv->GetKey().c_str());

		CStr toggleData = kv->GetValue();

		CLexer src;
		CToken token;
		src.LoadMemory(toggleData, toggleData.Length(), "toggleData");
		while(1)
		{
			if(!src.ReadToken(&token))
				break;
			
			int index = atoi(token.c_str());
			newToggle.toggleList.Append(index);

			//Skip the ,
			src.ReadToken(&token);
		};
		
		newToggle.lastUsed = 0;
		weaponToggles.Set(newToggle.name, newToggle);

		kv = spawnArgs.MatchPrefix( "weapontoggle", kv );
	};

	bloomEnabled			= false;
	bloomSpeed				= 1;
	bloomIntensity			= -0.01f;

	if ( g_demoMode.GetBool() && weapon.GetEntity() && weapon.GetEntity()->AmmoInClip() == 0 )
		weapon.GetEntity()->ForceAmmoInClip();
};

/*
==================
CBasePlayer::Killed
==================
*/
void CBasePlayer::Killed( CBaseEntity *inflictor, CBaseEntity *attacker, int damage, const CVec3 &dir, int location )
{
	if constexpr(IsServer())
	{
		float delay;

		// stop taking knockback once dead
		fl.noknockback = true;
		
		if ( health < -999 )
			health = -999;

		if ( AI_DEAD )
		{
			AI_PAIN = true;
			return;
		};

		heartInfo.Init( 0, 0, 0, BASE_HEARTRATE );
		AdjustHeartRate( DEAD_HEARTRATE, 10.0f, 0.0f, true );

		if ( !g_testDeath.GetBool() && !mpGame->GetRules()->IsMultiplayer() )
			playerView.Fade( colorBlack, 3000 );

		AI_DEAD = true;
		SetAnimState( ANIMCHANNEL_LEGS, "Legs_Death", 4 );
		SetAnimState( ANIMCHANNEL_TORSO, "Torso_Death", 4 );
		SetWaitState( "" );

		animator.ClearAllJoints();

		if ( StartRagdoll() )
		{
			pm_modelView.SetInteger( 0 );
			minRespawnTime = gameLocal.time + RAGDOLL_DEATH_TIME;
			maxRespawnTime = minRespawnTime + MAX_RESPAWN_TIME;
		}
		else
		{
			// don't allow respawn until the death anim is done
			// g_forcerespawn may force spawning at some later time
			delay = spawnArgs.GetFloat( "respawn_delay" );
			minRespawnTime = gameLocal.time + SEC2MS( delay );
			maxRespawnTime = minRespawnTime + MAX_RESPAWN_TIME;
		};

		physicsObj.SetMovementType( PM_DEAD );
		
		StartSound( "snd_death", SND_CHANNEL_VOICE, 0, false, nullptr );
		StopSound( SND_CHANNEL_BODY2, false );

		fl.takedamage = true;		// can still be gibbed

		// get rid of weapon
		weapon.GetEntity()->OwnerDied();
		
		// In multiplayer, get rid of the flashlight, or other players
		// will see it floating after the player is dead.
		if ( mpGame->GetRules()->IsMultiplayer() )
		{
			FlashlightOff();
			if ( flashlight.GetEntity() )
				flashlight.GetEntity()->OwnerDied();
		};

		// drop the weapon as an item
		DropWeapon( true );

		// drop the flag if player was carrying it
		if ( mpGame->GetRules()->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased() && carryingFlag )
			DropFlag();

		if ( !g_testDeath.GetBool() )
			LookAtKiller( inflictor, attacker );

		if ( mpGame->GetRules()->IsMultiplayer() || g_testDeath.GetBool() )
		{
			CBasePlayer *killer = nullptr;
			// no gibbing in MP. Event_Gib will early out in MP
			if ( attacker->IsType( CBasePlayer::Type ) )
			{
				killer = static_cast<CBasePlayer*>(attacker);
				if ( health < -20 || killer->PowerUpActive( BERSERK ) )
				{
					gibDeath = true;
					gibsDir = dir;
					gibsLaunched = false;
				};
			};
			gameLocal.mpGame.PlayerDeath( this, killer, isTelefragged );
		}
		else
			physicsObj.SetContents( CONTENTS_CORPSE | CONTENTS_MONSTERCLIP );

		ClearPowerUps();

		UpdateVisuals();
	};
};

/*
===============
CBasePlayer::Restart
================
*/
void CBasePlayer::Restart()
{
	CBaseActor::Restart();
	
	// client needs to setup the animation script object again
	if constexpr(IsClient())
	{
		// Make sure the weapon spawnId gets re-linked on the next snapshot.
		// Otherwise, its owner might not be set after the map restart, which causes asserts and crashes.
		weapon = nullptr;
		flashlight = nullptr;
		enviroSuitLight = nullptr;
		Init();
	}
	else
	{
		// choose a random spot and prepare the point of view in case player is left spectating
		assert( spectating );
		SpawnFromSpawnSpot();
	};

	useInitialSpawns = true;
	UpdateSkinSetup();
};

/*
===============
CBasePlayer::Respawn_Shared
Called on server and client players when they respawn (including on initial spawn)
===============
*/
void CBasePlayer::Respawn_Shared()
{
	respawn_netEvent.Set();

	serverOverridePositionTime = gameLocal.GetServerGameTimeMs();

	// Remove the hud respawn message.
	HideRespawnHudMessage();

	FlashlightOff();
};

/*
=====================
CBasePlayer::UpdateConditions
=====================
*/
void CBasePlayer::UpdateConditions()
{
	CVec3 velocity;
	float fallspeed;
	float forwardspeed;
	float sidespeed;

	// minus the push velocity to avoid playing the walking animation and sounds when riding a mover
	velocity = physicsObj.GetLinearVelocity() - physicsObj.GetPushedLinearVelocity();
	fallspeed = velocity * physicsObj.GetGravityNormal();

	if(influenceActive)
	{
		AI_FORWARD = false;
		AI_BACKWARD = false;
		AI_STRAFE_LEFT = false;
		AI_STRAFE_RIGHT = false;
	}
	else if(gameLocal.time - lastDmgTime < 500)
	{
		forwardspeed = velocity * viewAxis[0];
		sidespeed = velocity * viewAxis[1];
		AI_FORWARD = AI_ONGROUND && (forwardspeed > 20.01f);
		AI_BACKWARD = AI_ONGROUND && (forwardspeed < -20.01f);
		AI_STRAFE_LEFT = AI_ONGROUND && (sidespeed > 20.01f);
		AI_STRAFE_RIGHT = AI_ONGROUND && (sidespeed < -20.01f);
	}
	else if(xyspeed > MIN_BOB_SPEED)
	{
		AI_FORWARD = AI_ONGROUND && (usercmd.forwardmove > 0);
		AI_BACKWARD = AI_ONGROUND && (usercmd.forwardmove < 0);
		AI_STRAFE_LEFT = AI_ONGROUND && (usercmd.rightmove < 0);
		AI_STRAFE_RIGHT = AI_ONGROUND && (usercmd.rightmove > 0);
	}
	else
	{
		AI_FORWARD = false;
		AI_BACKWARD = false;
		AI_STRAFE_LEFT = false;
		AI_STRAFE_RIGHT = false;
	};

	AI_RUN = (usercmd.buttons & BUTTON_RUN) && ((!pm_stamina.GetFloat()) || (stamina > pm_staminathreshold.GetFloat()));
	AI_DEAD = (health <= 0);
};

/*
===============
CBasePlayer::PowerUpActive
===============
*/
bool CBasePlayer::PowerUpActive(int powerup) const
{
	return (inventory.powerups & (1 << powerup)) != 0;
}

/*
===============
CBasePlayer::GivePowerUp
===============
*/
bool CBasePlayer::GivePowerUp(int powerup, int time, unsigned int giveFlags)
{
	const char *sound;

	if(powerup >= 0 && powerup < MAX_POWERUPS)
	{
		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			if constexpr(IsServer())
			{
				CBitMsg msg;
				byte msgBuf[MAX_EVENT_PARAM_SIZE];

				msg.InitWrite(msgBuf, sizeof(msgBuf));
				msg.WriteShort(powerup);
				msg.WriteShort(time);
				ServerSendEvent(EVENT_POWERUP, &msg, false);
			};

			if(powerup != MEGAHEALTH)
				inventory.GivePowerUp(this, powerup, time);
		};

		switch(powerup)
		{
		case BERSERK:
		{
			if(giveFlags & ITEM_GIVE_FROM_WEAPON)
			{
				// Berserk is granted by the bloodstone in ROE, but we don't want any of the
				// standard behavior (sound fx, switch to fists) when you get it this way.
			}
			else
			{
				if(giveFlags & ITEM_GIVE_FEEDBACK)
				{
					inventory.AddPickupName("#str_00100627", this);

					if(spawnArgs.GetString("snd_berserk_third", "", &sound) && sound[0] != '\0')
						StartSoundShader(declManager->FindSound(sound), SND_CHANNEL_DEMONIC, 0, false, nullptr);
				};

				if(giveFlags & ITEM_GIVE_UPDATE_STATE)
				{
					if constexpr(!IsClient())
						idealWeapon = weapon_fists;
				};
			};
			break;
		}
		case INVISIBILITY:
		{
			if(mpGame->GetRules()->IsMultiplayer() && (giveFlags & ITEM_GIVE_FEEDBACK))
				inventory.AddPickupName("#str_00100628", this);
			
			if(giveFlags & ITEM_GIVE_UPDATE_STATE)
			{
				// remove any decals from the model
				if(modelDefHandle != -1)
					gameRenderWorld->RemoveDecals(modelDefHandle);
				
				if(weapon.GetEntity())
					weapon.GetEntity()->UpdateSkin();
				
				if(flashlight.GetEntity())
					flashlight.GetEntity()->UpdateSkin();
			};

			/*if ( spawnArgs.GetString( "snd_invisibility", "", &sound ) ) {
				StartSoundShader( declManager->FindSound( sound ), SND_CHANNEL_ANY, 0, false, nullptr );
				} */
			break;
		}
		case ADRENALINE:
		{
			if(giveFlags & ITEM_GIVE_FEEDBACK)
				inventory.AddPickupName("#str_00100799", this);

			if(giveFlags & ITEM_GIVE_UPDATE_STATE)
				stamina = 100.0f;

			break;
		}
		case MEGAHEALTH:
		{
			if(giveFlags & ITEM_GIVE_FEEDBACK)
			{
				if(mpGame->GetRules()->IsMultiplayer())
					inventory.AddPickupName("#str_00100629", this);

				if(spawnArgs.GetString("snd_megahealth", "", &sound))
					StartSoundShader(declManager->FindSound(sound), SND_CHANNEL_ANY, 0, false, nullptr);
			};
			
			if(giveFlags & ITEM_GIVE_UPDATE_STATE)
				health = 200;

			break;
		}
		case ENVIROSUIT:
		{
			if(giveFlags & ITEM_GIVE_FEEDBACK)
			{
				// Turn on the envirosuit sound
				if(gameSoundWorld)
				{
					gameSoundWorld->SetEnviroSuit(true);
				}
			}

			if(giveFlags & ITEM_GIVE_UPDATE_STATE)
			{
				// Put the helmet and lights on the player
				CDict args;

				// Light
				const CDict *lightDef = gameLocal.FindEntityDefDict("envirosuit_light", false);
				if(lightDef)
				{
					CBaseEntity *temp;
					gameLocal.SpawnEntityDef(*lightDef, &temp, false);

					CLight *eLight = static_cast<CLight *>(temp);
					eLight->GetPhysics()->SetOrigin(firstPersonViewOrigin);
					eLight->UpdateVisuals();
					eLight->Present();

					enviroSuitLight = eLight;
				}
			}
			break;
		}
		case ENVIROTIME:
		{
			if(giveFlags & ITEM_GIVE_UPDATE_STATE)
			{
				hudPowerup = ENVIROTIME;
				// The HUD display bar is fixed at 60 seconds
				hudPowerupDuration = 60000;
			}
			break;
		}
		case INVULNERABILITY:
		{
			if(common->IsMultiplayer() && (giveFlags & ITEM_GIVE_FEEDBACK))
			{
				inventory.AddPickupName("#str_00100630", this);
			}
			break;
		}
		}

		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			UpdateSkinSetup();
		}

		return true;
	}
	else
		gameLocal.Warning("Player given power up %i\n which is out of range", powerup);

	return false;
};

/*
==============
CBasePlayer::ClearPowerup
==============
*/
void CBasePlayer::ClearPowerup(int i)
{
	if constexpr(IsServer())
	{
		CBitMsg msg;
		byte msgBuf[MAX_EVENT_PARAM_SIZE];

		msg.InitWrite(msgBuf, sizeof(msgBuf));
		msg.WriteShort(i);
		msg.WriteShort(0);
		ServerSendEvent(EVENT_POWERUP, &msg, false);
	};

	powerUpSkin = nullptr;
	
	inventory.powerups &= ~(1 << i);
	inventory.powerupEndTime[i] = 0;
	
	switch(i)
	{
	case BERSERK:
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			StopSound(SND_CHANNEL_DEMONIC, false);
		}
		if(!mpGame->GetRules()->IsMultiplayer())
		{
			StopHealthRecharge();
		}
		break;
	}
	case INVISIBILITY:
	{
		if(weapon.GetEntity())
		{
			weapon.GetEntity()->UpdateSkin();
		}
		if(flashlight.GetEntity())
		{
			flashlight.GetEntity()->UpdateSkin();
		}
		break;
	}
	case ENVIROSUIT:
	{
		hudPowerup = -1;

		// Turn off the envirosuit sound
		if(gameSoundWorld)
		{
			gameSoundWorld->SetEnviroSuit(false);
		}

		// Take off the helmet and lights
		if(enviroSuitLight.IsValid())
		{
			enviroSuitLight.GetEntity()->PostEventMS(&EV_Remove, 0);
		}
		enviroSuitLight = nullptr;
		break;
	}
	case INVULNERABILITY:
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			StopSound(SND_CHANNEL_DEMONIC, false);
		}
	}
		/*case HASTE: {
			if(mpGame->GetRules()->IsMultiplayer()) {
				StopSound( SND_CHANNEL_DEMONIC, false );
			}
		}*/
	}
}

/*
==============
CBasePlayer::UpdatePowerUps
==============
*/
void CBasePlayer::UpdatePowerUps()
{
	int i;

	if constexpr(!IsClient())
	{
		for(i = 0; i < MAX_POWERUPS; i++)
		{
			if((inventory.powerups & (1 << i)) && inventory.powerupEndTime[i] > gameLocal.time)
			{
				switch(i)
				{
				case ENVIROSUIT:
				{
					if(enviroSuitLight.IsValid())
					{
						idAngles lightAng = firstPersonViewAxis.ToAngles();
						CVec3 lightOrg = firstPersonViewOrigin;
						const idDict *lightDef = gameLocal.FindEntityDefDict("envirosuit_light", false);

						CVec3 enviroOffset = lightDef->GetVector("enviro_offset");
						CVec3 enviroAngleOffset = lightDef->GetVector("enviro_angle_offset");

						lightOrg += (enviroOffset.x * firstPersonViewAxis[0]);
						lightOrg += (enviroOffset.y * firstPersonViewAxis[1]);
						lightOrg += (enviroOffset.z * firstPersonViewAxis[2]);
						lightAng.pitch += enviroAngleOffset.x;
						lightAng.yaw += enviroAngleOffset.y;
						lightAng.roll += enviroAngleOffset.z;

						enviroSuitLight.GetEntity()->GetPhysics()->SetOrigin(lightOrg);
						enviroSuitLight.GetEntity()->GetPhysics()->SetAxis(lightAng.ToMat3());
						enviroSuitLight.GetEntity()->UpdateVisuals();
						enviroSuitLight.GetEntity()->Present();
					}
					break;
				}
				default:
				{
					break;
				}
				}
			}
			if(PowerUpActive(i) && inventory.powerupEndTime[i] <= gameLocal.time)
			{
				ClearPowerup(i);
			}
		}
	}

	if(health > 0)
	{
		if(powerUpSkin)
		{
			renderEntity.customSkin = powerUpSkin;
		}
		else
		{
			renderEntity.customSkin = skin;
		}
	}

	if(healthPool && gameLocal.time > nextHealthPulse && !AI_DEAD && health > 0)
	{
		assert(!common->IsClient()); // healthPool never be set on client
		int amt = (healthPool > 5.0f) ? 5 : healthPool;
		health += amt;
		if(health > inventory.maxHealth)
		{
			health = inventory.maxHealth;
			healthPool = 0;
		}
		else
		{
			healthPool -= amt;
		}
		if(healthPool < 1.0f)
		{
			healthPool = 0.0f;
		}
		else
		{
			nextHealthPulse = gameLocal.time + HEALTHPULSE_TIME;
			healthPulse = true;
		}
	}
	if(!gameLocal.inCinematic && influenceActive == 0 && g_skill.GetInteger() == 3 && gameLocal.time > nextHealthTake && !AI_DEAD && health > g_healthTakeLimit.GetInteger())
	{
		assert(!common->IsClient()); // healthPool never be set on client

		if(!PowerUpActive(INVULNERABILITY))
		{
			health -= g_healthTakeAmt.GetInteger();
			if(health < g_healthTakeLimit.GetInteger())
			{
				health = g_healthTakeLimit.GetInteger();
			}
		}
		nextHealthTake = gameLocal.time + g_healthTakeTime.GetInteger() * 1000;
		healthTake = true;
	}
}

/*
===============
CBasePlayer::PrepareForRestart
================
*/
void CBasePlayer::PrepareForRestart()
{
	ClearPowerUps();

	if constexpr(!IsClient())
		ServerSpectate(true);

	forceRespawn = true;

	// Confirm reset hud states
	DropFlag();

	if(hud)
	{
		hud->SetFlagState(0, 0);
		hud->SetFlagState(1, 0);
	};

	// we will be restarting program, clear the client entities from program-related things first
	ShutdownThreads();

	// the sound world is going to be cleared, don't keep references to emitters
	FreeSoundEmitter(false);
};

///////////////////////////////////////////////////////////

/*
==============
CBasePlayer::CBasePlayer
==============
*/
CBasePlayer::CBasePlayer()
    : previousViewQuat(0.0f, 0.0f, 0.0f, 1.0f), nextViewQuat(0.0f, 0.0f, 0.0f, 1.0f),
	  idealWeapon(-1), serverOverridePositionTime(0), clientFireCount(0)
{
	noclip = false;
	godmode = false;

	spawnAnglesSet = false;
	spawnAngles = ang_zero;
	viewAngles = ang_zero;
	cmdAngles = ang_zero;
	independentWeaponPitchAngle = 0.0f;

	oldButtons = 0;
	buttonMask = 0;
	oldImpulseSequence = 0;

	lastHitTime = 0;
	lastSndHitTime = 0;
	lastSavingThrowTime = 0;

	laserSightHandle = -1;
	memset(&laserSightRenderEntity, 0, sizeof(laserSightRenderEntity));

	weapon = nullptr;
	primaryObjective = nullptr;

	if constexpr(IsClient())
		hudManager = new idMenuHandler_HUD();
	else
		hudManager = nullptr;
	
	hud = nullptr;
	objectiveSystemOpen = false;
	memset(quickSlot, -1, sizeof(quickSlot));

	if constexpr(IsClient())
		pdaMenu = new(TAG_SWF) idMenuHandler_PDA();
	else
		pdaMenu = nullptr;

	pdaVideoMat = nullptr;
	mpMessages = nullptr;

	mountedObject = nullptr;
	enviroSuitLight = nullptr;

	heartRate = BASE_HEARTRATE;
	heartInfo.Init(0, 0, 0, 0);
	lastHeartAdjust = 0;
	lastHeartBeat = 0;
	lastDmgTime = 0;
	deathClearContentsTime = 0;
	lastArmorPulse = -10000;
	stamina = 0.0f;
	healthPool = 0.0f;
	nextHealthPulse = 0;
	healthPulse = false;
	nextHealthTake = 0;
	healthTake = false;

	forceScoreBoard = false;
	forceRespawn = false;
	spectating = false;
	spectator = 0;
	wantSpectate = true;

	carryingFlag = false;

	lastHitToggle = false;

	minRespawnTime = 0;
	maxRespawnTime = 0;

	firstPersonViewOrigin = vec3_zero;
	firstPersonViewAxis = mat3_identity;

	hipJoint = INVALID_JOINT;
	chestJoint = INVALID_JOINT;
	headJoint = INVALID_JOINT;

	bobFoot = 0;
	bobFrac = 0.0f;
	bobfracsin = 0.0f;
	bobCycle = 0;
	xyspeed = 0.0f;
	stepUpTime = 0;
	stepUpDelta = 0.0f;
	idealLegsYaw = 0.0f;
	legsYaw = 0.0f;
	legsForward = true;
	oldViewYaw = 0.0f;
	viewBobAngles = ang_zero;
	viewBob = vec3_zero;
	landChange = 0;
	landTime = 0;

	currentWeapon = -1;
	previousWeapon = -1;
	weaponSwitchTime = 0;
	weaponEnabled = true;
	weapon_pda = -1;
	weapon_fists = -1;
	weapon_chainsaw = -1;

	hudPowerup = -1;
	lastHudPowerup = -1;
	hudPowerupDuration = 0;

	skinIndex = 0;
	skin = nullptr;
	powerUpSkin = nullptr;

	numProjectileKills = 0;
	numProjectilesFired = 0;
	numProjectileHits = 0;

	airless = false;
	airMsec = 0;
	lastAirDamage = 0;

	gibDeath = false;
	gibsLaunched = false;
	gibsDir = vec3_zero;

	zoomFov.Init(0, 0, 0, 0);
	centerView.Init(0, 0, 0, 0);
	fxFov = false;

	influenceFov = 0;
	influenceActive = 0;
	influenceRadius = 0.0f;
	influenceEntity = nullptr;
	influenceMaterial = nullptr;
	influenceSkin = nullptr;

	privateCameraView = nullptr;

	memset(loggedViewAngles, 0, sizeof(loggedViewAngles));
	memset(loggedAccel, 0, sizeof(loggedAccel));
	currentLoggedAccel = 0;

	focusTime = 0;
	focusGUIent = nullptr;
	focusUI = nullptr;
	focusCharacter = nullptr;
	talkCursor = 0;
	focusVehicle = nullptr;
	cursor = nullptr;

	oldMouseX = 0;
	oldMouseY = 0;

	lastDamageDef = 0;
	lastDamageDir = vec3_zero;
	lastDamageLocation = 0;
	smoothedFrame = 0;
	smoothedOriginUpdated = false;
	smoothedOrigin = vec3_zero;
	smoothedAngles = ang_zero;

	fl.networkSync = true;

	doingDeathSkin = false;
	weaponGone = false;
	useInitialSpawns = false;
	tourneyRank = 0;
	lastSpectateTeleport = 0;
	tourneyLine = 0;
	hiddenWeapon = false;
	tipUp = false;
	objectiveUp = false;
	teleportEntity = nullptr;
	teleportKiller = -1;
	respawning = false;
	leader = false;
	lastSpectateChange = 0;
	lastTeleFX = -9999;
	weaponCatchup = false;
	clientFireCount = 0;

	MPAim = -1;
	lastMPAim = -1;
	lastMPAimTime = 0;
	MPAimFadeTime = 0;
	MPAimHighlight = false;

	spawnedTime = 0;
	lastManOver = false;
	lastManPlayAgain = false;
	lastManPresent = false;

	isTelefragged = false;

	isLagged = false;
	isChatting = 0;

	selfSmooth = false;

	playedTimeSecs = 0;
	playedTimeResidual = 0;

	ResetControllerShake();

	memset(pdaHasBeenRead, 0, sizeof(pdaHasBeenRead));
	memset(videoHasBeenViewed, 0, sizeof(videoHasBeenViewed));
	memset(audioHasBeenHeard, 0, sizeof(audioHasBeenHeard));
};

/*
==============
CBasePlayer::LinkScriptVariables

set up conditions for animation
==============
*/
void CBasePlayer::LinkScriptVariables()
{
	AI_FORWARD.LinkTo(scriptObject, "AI_FORWARD");
	AI_BACKWARD.LinkTo(scriptObject, "AI_BACKWARD");
	AI_STRAFE_LEFT.LinkTo(scriptObject, "AI_STRAFE_LEFT");
	AI_STRAFE_RIGHT.LinkTo(scriptObject, "AI_STRAFE_RIGHT");
	AI_ATTACK_HELD.LinkTo(scriptObject, "AI_ATTACK_HELD");
	AI_WEAPON_FIRED.LinkTo(scriptObject, "AI_WEAPON_FIRED");
	AI_JUMP.LinkTo(scriptObject, "AI_JUMP");
	AI_DEAD.LinkTo(scriptObject, "AI_DEAD");
	AI_CROUCH.LinkTo(scriptObject, "AI_CROUCH");
	AI_ONGROUND.LinkTo(scriptObject, "AI_ONGROUND");
	AI_ONLADDER.LinkTo(scriptObject, "AI_ONLADDER");
	AI_HARDLANDING.LinkTo(scriptObject, "AI_HARDLANDING");
	AI_SOFTLANDING.LinkTo(scriptObject, "AI_SOFTLANDING");
	AI_RUN.LinkTo(scriptObject, "AI_RUN");
	AI_PAIN.LinkTo(scriptObject, "AI_PAIN");
	AI_RELOAD.LinkTo(scriptObject, "AI_RELOAD");
	AI_TELEPORT.LinkTo(scriptObject, "AI_TELEPORT");
	AI_TURN_LEFT.LinkTo(scriptObject, "AI_TURN_LEFT");
	AI_TURN_RIGHT.LinkTo(scriptObject, "AI_TURN_RIGHT");
}

/*
==============
CBasePlayer::SetupWeaponEntity
==============
*/
void CBasePlayer::SetupWeaponEntity()
{
	const char *weap{""};

	if(weapon.GetEntity())
	{
		// get rid of old weapon
		weapon.GetEntity()->Clear();
		currentWeapon = -1;
	}
//#ifdef GAME_DLL
	else if constexpr(!IsClient())
	{
		weapon = static_cast<CBaseWeapon *>(gameLocal.SpawnEntityType(CBaseWeapon::Type, nullptr));
		weapon.GetEntity()->SetOwner(this);
		currentWeapon = -1;

		// flashlight
		flashlight = static_cast<CBaseWeapon *>(gameLocal.SpawnEntityType(CBaseWeapon::Type, nullptr));
		flashlight.GetEntity()->SetFlashlightOwner(this);
		//FlashlightOff();
	};
//#endif

	for(int w = 0; w < MAX_WEAPONS; ++w)
	{
		weap = spawnArgs.GetString(va("def_weapon%d", w));
		if(weap != nullptr && *weap != nullptr)
			CBaseWeapon::CacheWeapon(weap);
	};
};

/*
==============
CBasePlayer::Init
==============
*/
void CBasePlayer::Init()
{
	const char *value{""};
	const CKeyValue *kv;

	noclip = false;
	godmode = false;

	oldButtons = 0;
	oldImpulseSequence = 0;

	currentWeapon = -1;
	idealWeapon = -1;
	previousWeapon = -1;
	weaponSwitchTime = 0;
	weaponEnabled = true;
	weapon_pda = SlotForWeapon("weapon_pda");
	weapon_fists = SlotForWeapon("weapon_fists");
	weapon_flashlight = SlotForWeapon("weapon_flashlight");
	weapon_chainsaw = SlotForWeapon("weapon_chainsaw");

	lastDmgTime = 0;
	lastArmorPulse = -10000;
	lastHeartAdjust = 0;
	lastHeartBeat = 0;
	heartInfo.Init(0, 0, 0, 0);

	bobCycle = 0;
	bobFrac = 0.0f;
	landChange = 0;
	landTime = 0;
	zoomFov.Init(0, 0, 0, 0);
	centerView.Init(0, 0, 0, 0);
	fxFov = false;

	influenceFov = 0;
	influenceActive = 0;
	influenceRadius = 0.0f;
	influenceEntity = nullptr;
	influenceMaterial = nullptr;
	influenceSkin = nullptr;

	mountedObject = nullptr;
	if(enviroSuitLight.IsValid())
		enviroSuitLight.GetEntity()->PostEventMS(&EV_Remove, 0);
	enviroSuitLight = nullptr;
	healthRecharge = false;
	lastHealthRechargeTime = 0;
	rechargeSpeed = 500;
	new_g_damageScale = 1.f;
	bloomEnabled = false;
	bloomSpeed = 1.f;
	bloomIntensity = -0.01f;
	inventory.InitRechargeAmmo(this);
	hudPowerup = -1;
	lastHudPowerup = -1;
	hudPowerupDuration = 0;

	currentLoggedAccel = 0;

	focusTime = 0;
	focusGUIent = nullptr;
	focusUI = nullptr;
	focusCharacter = nullptr;
	talkCursor = 0;
	focusVehicle = nullptr;

	// remove any damage effects
	playerView.ClearEffects();

	// damage values
	fl.takedamage = true;
	ClearPain();

	// restore persistent data
	RestorePersistantInfo();

	bobCycle = 0;
	stamina = 0.0f;
	healthPool = 0.0f;
	nextHealthPulse = 0;
	healthPulse = false;
	nextHealthTake = 0;
	healthTake = false;

	SetupWeaponEntity();
	currentWeapon = -1;
	previousWeapon = -1;

	heartRate = BASE_HEARTRATE;
	AdjustHeartRate(BASE_HEARTRATE, 0.0f, 0.0f, true);

	idealLegsYaw = 0.0f;
	legsYaw = 0.0f;
	legsForward = true;
	oldViewYaw = 0.0f;

	// set the pm_ cvars
	if(!mpGame->GetRules()->IsMultiplayer() || constexpr(IsServer()))
	{
		kv = spawnArgs.MatchPrefix("pm_", nullptr);
		while(kv)
		{
			cvarSystem->SetCVarString(kv->GetKey(), kv->GetValue());
			kv = spawnArgs.MatchPrefix("pm_", kv);
		};
	};

	// disable stamina on hell levels
	if(gameLocal.world && gameLocal.world->spawnArgs.GetBool("no_stamina"))
		pm_stamina.SetFloat(0.0f);

	// stamina always initialized to maximum
	stamina = pm_stamina.GetFloat();

	// air always initialized to maximum too
	airMsec = pm_airMsec.GetFloat();
	airless = false;

	gibDeath = false;
	gibsLaunched = false;
	gibsDir.Zero();

	// set the gravity
	physicsObj.SetGravity(gameLocal.GetGravity());

	// start out standing
	SetEyeHeight(pm_normalviewheight.GetFloat());

	stepUpTime = 0;
	stepUpDelta = 0.0f;
	viewBobAngles.Zero();
	viewBob.Zero();

	value = spawnArgs.GetString("model");
	if(value != nullptr && (*value != 0))
		SetModel(value);

	if(hud)
	{
		hud->SetCursorState(this, CURSOR_TALK, 0);
		hud->SetCursorState(this, CURSOR_IN_COMBAT, 1);
		hud->SetCursorState(this, CURSOR_ITEM, 0);
		hud->SetCursorState(this, CURSOR_GRABBER, 0);
		hud->SetCursorState(this, CURSOR_NONE, 0);
		hud->UpdateCursorState();
	};

	if((mpGame->GetRules()->IsMultiplayer() || g_testDeath.GetBool()) && skin)
	{
		SetSkin(skin);
		renderEntity.shaderParms[6] = 0.0f;
	}
	else if(spawnArgs.GetString("spawn_skin", nullptr, &value))
	{
		skin = declManager->FindSkin(value);
		SetSkin(skin);
		renderEntity.shaderParms[6] = 0.0f;
	};

	value = spawnArgs.GetString("bone_hips", "");
	hipJoint = animator.GetJointHandle(value);
	if(hipJoint == INVALID_JOINT)
		gpSystem->Error("Joint '%s' not found for 'bone_hips' on '%s'", value, name.c_str()); // TODO: mpGame->GetSystem()->Error

	value = spawnArgs.GetString("bone_chest", "");
	chestJoint = animator.GetJointHandle(value);
	if(chestJoint == INVALID_JOINT)
		gpSystem->Error("Joint '%s' not found for 'bone_chest' on '%s'", value, name.c_str()); // TODO: mpGame->GetSystem()->Error

	value = spawnArgs.GetString("bone_head", "");
	headJoint = animator.GetJointHandle(value);
	if(headJoint == INVALID_JOINT)
		gpSystem->Error("Joint '%s' not found for 'bone_head' on '%s'", value, name.c_str()); // TODO: mpGame->GetSystem()->Error

	// initialize the script variables
	AI_FORWARD = false;
	AI_BACKWARD = false;
	AI_STRAFE_LEFT = false;
	AI_STRAFE_RIGHT = false;
	AI_ATTACK_HELD = false;
	AI_WEAPON_FIRED = false;
	AI_JUMP = false;
	AI_DEAD = false;
	AI_CROUCH = false;
	AI_ONGROUND = true;
	AI_ONLADDER = false;
	AI_HARDLANDING = false;
	AI_SOFTLANDING = false;
	AI_RUN = false;
	AI_PAIN = false;
	AI_RELOAD = false;
	AI_TELEPORT = false;
	AI_TURN_LEFT = false;
	AI_TURN_RIGHT = false;

	// reset the script object
	ConstructScriptObject();

	// execute the script so the script object's constructor takes effect immediately
	scriptThread->Execute();

	forceScoreBoard = false;

	privateCameraView = nullptr;

	lastSpectateChange = 0;
	lastTeleFX = -9999;

	hiddenWeapon = false;
	tipUp = false;
	objectiveUp = false;
	teleportEntity = nullptr;
	teleportKiller = -1;
	leader = false;

	SetPrivateCameraView(nullptr);

	MPAim = -1;
	lastMPAim = -1;
	lastMPAimTime = 0;
	MPAimFadeTime = 0;
	MPAimHighlight = false;

	//isChatting = false;

	achievementManager.Init(this);

	flashlightBattery = flashlight_batteryDrainTimeMS.GetInteger(); // fully charged

	aimAssist.Init(this);

	// laser sight for 3DTV
	memset(&laserSightRenderEntity, 0, sizeof(laserSightRenderEntity));
	laserSightRenderEntity.hModel = renderModelManager->FindModel("_BEAM");
	laserSightRenderEntity.customShader = declManager->FindMaterial("stereoRenderLaserSight");
};

/*
===============
CBasePlayer::GiveVideo
===============
*/
void CBasePlayer::GiveVideo(const idDeclVideo *video, const char *itemName)
{
	if(video == nullptr)
		return;

	int oldNumVideos = inventory.videos.Num();
	inventory.videos.AddUnique(video);

	if(oldNumVideos < inventory.videos.Num())
		GetAchievementManager().EventCompletesAchievement(ACHIEVEMENT_WATCH_ALL_VIDEOS);

	if(itemName != nullptr && itemName[0] != 0)
		inventory.pickupItemNames.Append(itemName);

	if(hud)
		hud->DownloadVideo();
}

/*
===============
CBasePlayer::GiveSecurity
===============
*/
void CBasePlayer::GiveSecurity(const char *security)
{
	GetPDA()->SetSecurity(security);

	if(hud)
		hud->UpdatedSecurity();
}

/*
===============
CBasePlayer::GiveEmail
===============
*/
void CBasePlayer::GiveEmail(const idDeclEmail *email)
{
	if(email == nullptr)
		return;

	inventory.emails.AddUnique(email);
	GetPDA()->AddEmail(email);

	// TODO_SPARTY: hook up new email notification in new hud
	//if ( hud ) {
	//	hud->HandleNamedEvent( "emailPickup" );
	//}
}

/*
===============
CBasePlayer::GivePDA
===============
*/
void CBasePlayer::GivePDA(const idDeclPDA *pda, const char *securityItem)
{
	if(mpGame->GetRules()->IsMultiplayer() && spectating)
		return;

	if(securityItem != nullptr && securityItem[0] != 0)
		inventory.pdaSecurity.AddUnique(securityItem);

	// Just to make sure they want the default player spawn defined pda.
	// Some what of a hack, so i dont have to change any map scripts that initially give
	// the player "personal" pda.
	if(pda == nullptr || idStr::Icmp(pda->GetName(), "personal") == 0)
	{
		pda = static_cast<const idDeclPDA *>(declManager->FindType(DECL_PDA, spawnArgs.GetString("pda_name", "personal")));
	}
	if(pda == nullptr)
	{
		return;
	}

	int oldNumPDAs = inventory.pdas.Num();
	inventory.pdas.AddUnique(pda);
	int newNumPDAs = inventory.pdas.Num();

	// Set the stat for # of PDAs...
	// Only increment the PDA stat if we've added a new one....
	if(oldNumPDAs < newNumPDAs)
	{
		switch(GetExpansionType())
		{
		case GAME_BASE:
			GetAchievementManager().EventCompletesAchievement(ACHIEVEMENT_PDAS_BASE);
			break;
		case GAME_D3XP:
			GetAchievementManager().EventCompletesAchievement(ACHIEVEMENT_PDAS_ROE);
			break;
		case GAME_D3LE:
			GetAchievementManager().EventCompletesAchievement(ACHIEVEMENT_PDAS_LE);
			break;
		}
	}

	// Copy any videos over
	for(int i = 0; i < pda->GetNumVideos(); i++)
	{
		const idDeclVideo *video = pda->GetVideoByIndex(i);
		if(video != nullptr)
		{
			inventory.videos.AddUnique(video);
		}
	}

	// This is kind of a hack, but it works nicely
	// We don't want to display the 'you got a new pda' message during a map load
	if(gameLocal.GetFrameNum() > 10)
	{
		const char *sec = pda->GetSecurity();

		if(hud)
			hud->DownloadPDA(pda, (sec != nullptr && sec[0] != 0) ? true : false);

		if(inventory.pdas.Num() == 1)
		{
			GetPDA()->RemoveAddedEmailsAndVideos();
			if(!objectiveSystemOpen)
			{
				TogglePDA();
			}
			//ShowTip( spawnArgs.GetString( "text_infoTitle" ), spawnArgs.GetString( "text_firstPDA" ), true );
		}
	}
}


/*
===============
CBasePlayer::GiveItem
===============
*/
void CBasePlayer::GiveItem(const char *itemname)
{
	idDict args;

	args.Set("classname", itemname);
	args.Set("owner", name.c_str());
	gameLocal.SpawnEntityDef(args);
}

/*
==================
CBasePlayer::SlotForWeapon
==================
*/
int CBasePlayer::SlotForWeapon(const char *weaponName)
{
	int i;

	for(i = 0; i < MAX_WEAPONS; i++)
	{
		const char *weap = spawnArgs.GetString(va("def_weapon%d", i));
		if(!idStr::Cmp(weap, weaponName))
		{
			return i;
		}
	}

	// not found
	return -1;
}

/*
===============
CBasePlayer::NextBestWeapon
===============
*/
void CBasePlayer::NextBestWeapon()
{
	const char *weap;
	int w = MAX_WEAPONS;

	if(!weaponEnabled)
	{
		return;
	}

	while(w > 0)
	{
		w--;
		if(w == weapon_flashlight)
		{
			continue;
		}
		weap = spawnArgs.GetString(va("def_weapon%d", w));
		if(!weap[0] || ((inventory.weapons & (1 << w)) == 0) || (!inventory.HasAmmo(weap, true, this)))
		{
			continue;
		}
		if(!spawnArgs.GetBool(va("weapon%d_best", w)))
		{
			continue;
		}

		//Some weapons will report having ammo but the clip is empty and
		//will not have enough to fill the clip (i.e. Double Barrel Shotgun with 1 round left)
		//We need to skip these weapons because they cannot be used
		if(inventory.HasEmptyClipCannotRefill(weap, this))
		{
			continue;
		}

		break;
	}
	idealWeapon = w;
	weaponSwitchTime = gameLocal.time + WEAPON_SWITCH_DELAY;
	UpdateHudWeapon();
}

/*
=================
CBasePlayer::StealWeapon
steal the target player's current weapon
=================
*/
void CBasePlayer::StealWeapon(CBasePlayer *player)
{
	if constexpr(IsServer())
	{
		// make sure there's something to steal
		CBaseWeapon *player_weapon = static_cast<CBaseWeapon *>(player->weapon.GetEntity());
		if(!player_weapon || !player_weapon->CanDrop() || weaponGone)
			return;

		// steal - we need to effectively force the other player to abandon his weapon
		int newweap = player->currentWeapon;
		if(newweap == -1)
			return;

		// might be just dropped - check inventory
		if(!(player->inventory.weapons & (1 << newweap)))
			return;

		const char *weapon_classname = spawnArgs.GetString(va("def_weapon%d", newweap));
		assert(weapon_classname);
		int ammoavailable = player->weapon.GetEntity()->AmmoAvailable();
		int inclip = player->weapon.GetEntity()->AmmoInClip();

		ammoavailable += inclip;

		if((ammoavailable != -1) && (ammoavailable < 0))
		{
			// see DropWeapon
			common->DPrintf("CBasePlayer::StealWeapon: bad ammo setup\n");
			// we still steal the weapon, so let's use the default ammo levels
			inclip = -1;
			const CDeclEntityDef *decl = gameLocal.FindEntityDef(weapon_classname);
			assert(decl);
			const CKeyValue *keypair = decl->dict.MatchPrefix("inv_ammo_");
			assert(keypair);
			ammoavailable = atoi(keypair->GetValue());
		}

		player->weapon.GetEntity()->WeaponStolen();
		player->inventory.Drop(player->spawnArgs, nullptr, newweap);
		player->SelectWeapon(weapon_fists, false);
		// in case the robbed player is firing rounds with a continuous fire weapon like the chaingun/plasma etc.
		// this will ensure the firing actually stops
		player->weaponGone = true;

		// give weapon, setup the ammo count
		Give("weapon", weapon_classname, ITEM_GIVE_FEEDBACK | ITEM_GIVE_UPDATE_STATE);
		ammo_t ammo_i = player->inventory.AmmoIndexForWeaponClass(weapon_classname, nullptr);
		idealWeapon = newweap;
		const int currentAmmo = inventory.GetInventoryAmmoForType(ammo_i);
		inventory.SetInventoryAmmoForType(ammo_i, currentAmmo + ammoavailable);
	};
};

/*
===============
CBasePlayer::ActiveGui
===============
*/
idUserInterface *CBasePlayer::ActiveGui()
{
	if(objectiveSystemOpen)
		return nullptr;

	return focusUI;
};

/*
===============
CBasePlayer::Weapon_NPC
===============
*/
void CBasePlayer::Weapon_NPC()
{
	if(idealWeapon != currentWeapon)
		Weapon_Combat();

	StopFiring();
	weapon.GetEntity()->LowerWeapon();

	bool wasDown = (oldButtons & (BUTTON_ATTACK | BUTTON_USE)) != 0;
	bool isDown = (usercmd.buttons & (BUTTON_ATTACK | BUTTON_USE)) != 0;
	if(isDown && !wasDown)
	{
		buttonMask |= BUTTON_ATTACK;
		focusCharacter->TalkTo(this);
	}
}

/*
===============
CBasePlayer::Weapon_GUI
===============
*/
void CBasePlayer::Weapon_GUI()
{
	if(!objectiveSystemOpen)
	{
		if(idealWeapon != currentWeapon)
		{
			Weapon_Combat();
		}
		StopFiring();
		weapon.GetEntity()->LowerWeapon();
	}

	// disable click prediction for the GUIs. handy to check the state sync does the right thing
	if(common->IsClient() && !net_clientPredictGUI.GetBool())
	{
		return;
	}

	bool wasDown = (oldButtons & (BUTTON_ATTACK | BUTTON_USE)) != 0;
	bool isDown = (usercmd.buttons & (BUTTON_ATTACK | BUTTON_USE)) != 0;
	if(isDown != wasDown)
	{
		const char *command = nullptr;
		idUserInterface *ui = ActiveGui();
		if(ui)
		{
			bool updateVisuals = false;
			sysEvent_t ev = sys->GenerateMouseButtonEvent(1, isDown);
			command = ui->HandleEvent(&ev, gameLocal.time, &updateVisuals);
			if(updateVisuals && focusGUIent && ui == focusUI)
			{
				focusGUIent->UpdateVisuals();
			}
		}
		if(common->IsClient())
		{
			// we predict enough, but don't want to execute commands
			return;
		}

		// HACK - Check to see who is activating the frag chamber. Im sorry.
		if(mpGame->GetRules()->IsMultiplayer() && focusGUIent)
		{
			if(strcmp(focusGUIent->GetName(), "chamber_gui_console") == 0 && strcmp(command, " ; runScript chamber_trigger") == 0)
			{
				gameLocal.playerActivateFragChamber = this;
			}
		}

		if(focusGUIent)
		{
			HandleGuiCommands(focusGUIent, command);
		}
		else
		{
			HandleGuiCommands(this, command);
		}
	}
}

/*
===============
CBasePlayer::SpectateFreeFly
===============
*/
void CBasePlayer::SpectateFreeFly(bool force)
{
	CBasePlayer *player;
	CVec3 newOrig;
	CVec3 spawn_origin;
	idAngles spawn_angles;

	player = gameLocal.GetClientByNum(spectator);
	if(force || gameLocal.time > lastSpectateChange)
	{
		spectator = entityNumber;
		if(player != nullptr && player != this && !player->spectating && !player->IsInTeleport())
		{
			newOrig = player->GetPhysics()->GetOrigin();
			if(player->physicsObj.IsCrouching())
			{
				newOrig[2] += pm_crouchviewheight.GetFloat();
			}
			else
			{
				newOrig[2] += pm_normalviewheight.GetFloat();
			}
			newOrig[2] += SPECTATE_RAISE;
			idBounds b = idBounds(vec3_origin).Expand(pm_spectatebbox.GetFloat() * 0.5f);
			CVec3 start = player->GetPhysics()->GetOrigin();
			start[2] += pm_spectatebbox.GetFloat() * 0.5f;
			trace_t t;
			// assuming spectate bbox is inside stand or crouch box
			gameLocal.clip.TraceBounds(t, start, newOrig, b, MASK_PLAYERSOLID, player);
			newOrig.Lerp(start, newOrig, t.fraction);
			SetOrigin(newOrig);
			idAngles angle = player->viewAngles;
			angle[2] = 0;
			SetViewAngles(angle);
		}
		else
		{
			SelectInitialSpawnPoint(spawn_origin, spawn_angles);
			spawn_origin[2] += pm_normalviewheight.GetFloat();
			spawn_origin[2] += SPECTATE_RAISE;
			SetOrigin(spawn_origin);
			SetViewAngles(spawn_angles);
			// This may happen during GAMESTATE_STARTUP in mp, so we must set the spawnAngles too.
			spawnAngles = spawn_angles;

			if(force == false)
			{
				// only do this if they hit the cycle button.
				if(common->IsServer())
				{
					if(player != nullptr)
					{
						idBitMsg msg;
						byte msgBuf[MAX_EVENT_PARAM_SIZE];

						msg.InitWrite(msgBuf, sizeof(msgBuf));
						msg.WriteFloat(GetPhysics()->GetOrigin()[0]);
						msg.WriteFloat(GetPhysics()->GetOrigin()[1]);
						msg.WriteFloat(GetPhysics()->GetOrigin()[2]);
						msg.WriteFloat(viewAngles[0]);
						msg.WriteFloat(viewAngles[1]);
						msg.WriteFloat(viewAngles[2]);

						ServerSendEvent(CBasePlayer::EVENT_FORCE_ORIGIN, &msg, false);
					}
				}
			}
		}
		lastSpectateChange = gameLocal.time + 500;
	}
}

/*
===============
CBasePlayer::SpectateCycle
===============
*/
void CBasePlayer::SpectateCycle()
{
	CBasePlayer *player;

	if(gameLocal.time > lastSpectateChange)
	{
		int latchedSpectator = spectator;
		spectator = gameLocal.GetNextClientNum(spectator);
		player = gameLocal.GetClientByNum(spectator);
		assert(player); // never call here when the current spectator is wrong
		// ignore other spectators
		while(latchedSpectator != spectator && player->spectating)
		{
			spectator = gameLocal.GetNextClientNum(spectator);
			player = gameLocal.GetClientByNum(spectator);
		}
		lastSpectateChange = gameLocal.time + 500;
	}
}



/*
===============
CBasePlayer::HandleSingleGuiCommand
===============
*/
bool CBasePlayer::HandleSingleGuiCommand(CBaseEntity *entityGui, idLexer *src)
{
	idToken token;

	if(!src->ReadToken(&token))
	{
		return false;
	}

	if(token == ";")
	{
		return false;
	}

	if(token.Icmp("addhealth") == 0)
	{
		if(entityGui && health < 100)
		{
			int _health = entityGui->spawnArgs.GetInt("gui_parm1");
			int amt = (_health >= HEALTH_PER_DOSE) ? HEALTH_PER_DOSE : _health;
			_health -= amt;
			entityGui->spawnArgs.SetInt("gui_parm1", _health);
			if(entityGui->GetRenderEntity() && entityGui->GetRenderEntity()->gui[0])
			{
				entityGui->GetRenderEntity()->gui[0]->SetStateInt("gui_parm1", _health);
			}
			health += amt;
			if(health > 100)
			{
				health = 100;
			}
		}
		return true;
	}
	src->UnreadToken(&token);
	return false;
}

/*
==============
CBasePlayer::PlayVideoDisk
==============
*/
void CBasePlayer::PlayVideoDisk(const idDeclVideo *decl)
{
	EndAudioLog();
	pdaVideoMat = decl->GetRoq();
	if(pdaVideoMat)
	{
		int c = pdaVideoMat->GetNumStages();
		for(int i = 0; i < c; i++)
		{
			const shaderStage_t *stage = pdaVideoMat->GetStage(i);
			if(stage != nullptr && stage->texture.cinematic)
			{
				stage->texture.cinematic->ResetTime(Sys_Milliseconds());
			}
		}
		if(decl->GetWave() != nullptr)
		{
			StartSoundShader(decl->GetWave(), SND_CHANNEL_PDA_VIDEO, 0, false, nullptr);
		}
	}
}

/*
==============
CBasePlayer::EndVideoDisk
==============
*/
void CBasePlayer::EndVideoDisk()
{
	pdaVideoMat = nullptr;
	StopSound(SND_CHANNEL_PDA_VIDEO, false);
}

/*
==============
CBasePlayer::Collide
==============
*/
bool CBasePlayer::Collide(const trace_t &collision, const CVec3 &velocity)
{
	CBaseEntity *other;

	if(common->IsClient() && spectating == false)
	{
		return false;
	}

	other = gameLocal.entities[collision.c.entityNum];
	if(other)
	{
		other->Signal(SIG_TOUCH);
		if(!spectating)
		{
			if(other->RespondsTo(EV_Touch))
			{
				other->ProcessEvent(&EV_Touch, this, &collision);
			}
		}
		else
		{
			if(other->RespondsTo(EV_SpectatorTouch))
			{
				other->ProcessEvent(&EV_SpectatorTouch, this, &collision);
			}
		}
	}
	return false;
}

/*
================
CBasePlayer::UpdateLocation

Searches nearby locations 
================
*/
void CBasePlayer::UpdateLocation()
{
	if(hud)
	{
		hud->UpdateLocation(this);
	}
}

/*
================
CBasePlayer::ClearFocus

Clears the focus cursor
================
*/
void CBasePlayer::ClearFocus()
{
	focusCharacter = nullptr;
	focusGUIent = nullptr;
	focusUI = nullptr;
	focusVehicle = nullptr;
	talkCursor = 0;
}

/*
================
CBasePlayer::UpdateFocus

Searches nearby entities for interactive guis, possibly making one of them
the focus and sending it a mouse move event
================
*/
void CBasePlayer::UpdateFocus()
{
	idClipModel *clipModelList[MAX_GENTITIES];
	idClipModel *clip;
	int listedClipModels;
	CBaseEntity *oldFocus;
	CBaseEntity *ent;
	idUserInterface *oldUI;
	idAI *oldChar;
	int oldTalkCursor;
	idAFEntity_Vehicle *oldVehicle;
	int i, j;
	CVec3 start, end;
	bool allowFocus;
	const char *command;
	trace_t trace;
	guiPoint_t pt;
	const idKeyValue *kv;
	sysEvent_t ev;
	idUserInterface *ui;

	if(gameLocal.inCinematic)
	{
		return;
	}

	// only update the focus character when attack button isn't pressed so players
	// can still chainsaw NPC's
	if(mpGame->GetRules()->IsMultiplayer() || (!focusCharacter && (usercmd.buttons & BUTTON_ATTACK)))
	{
		allowFocus = false;
	}
	else
	{
		allowFocus = true;
	}

	oldFocus = focusGUIent;
	oldUI = focusUI;
	oldChar = focusCharacter;
	oldTalkCursor = talkCursor;
	oldVehicle = focusVehicle;

	if(focusTime <= gameLocal.time)
	{
		ClearFocus();
	}

	// don't let spectators interact with GUIs
	if(spectating)
	{
		return;
	}

	start = GetEyePosition();
	end = start + firstPersonViewAxis[0] * 80.0f;

	// player identification -> names to the hud
	if(mpGame->GetRules()->IsMultiplayer() && IsLocallyControlled())
	{
		CVec3 end = start + viewAngles.ToForward() * 768.0f;
		gameLocal.clip.TracePoint(trace, start, end, MASK_SHOT_BOUNDINGBOX, this);
		int iclient = -1;
		if((trace.fraction < 1.0f) && (trace.c.entityNum < MAX_CLIENTS))
		{
			iclient = trace.c.entityNum;
		}
		if(MPAim != iclient)
		{
			lastMPAim = MPAim;
			MPAim = iclient;
			lastMPAimTime = gameLocal.realClientTime;
		}
	}

	idBounds bounds(start);
	bounds.AddPoint(end);

	listedClipModels = gameLocal.clip.ClipModelsTouchingBounds(bounds, -1, clipModelList, MAX_GENTITIES);

	// no pretense at sorting here, just assume that there will only be one active
	// gui within range along the trace
	for(i = 0; i < listedClipModels; i++)
	{
		clip = clipModelList[i];
		ent = clip->GetEntity();

		if(ent->IsHidden())
		{
			continue;
		}

		if(allowFocus)
		{
			if(ent->IsType(idAFAttachment::Type))
			{
				CBaseEntity *body = static_cast<idAFAttachment *>(ent)->GetBody();
				if(body != nullptr && body->IsType(idAI::Type) && (static_cast<idAI *>(body)->GetTalkState() >= TALK_OK))
				{
					gameLocal.clip.TracePoint(trace, start, end, MASK_SHOT_RENDERMODEL, this);
					if((trace.fraction < 1.0f) && (trace.c.entityNum == ent->entityNumber))
					{
						ClearFocus();
						focusCharacter = static_cast<idAI *>(body);
						talkCursor = 1;
						focusTime = gameLocal.time + FOCUS_TIME;
						break;
					}
				}
				continue;
			}

			if(ent->IsType(idAI::Type))
			{
				if(static_cast<idAI *>(ent)->GetTalkState() >= TALK_OK)
				{
					gameLocal.clip.TracePoint(trace, start, end, MASK_SHOT_RENDERMODEL, this);
					if((trace.fraction < 1.0f) && (trace.c.entityNum == ent->entityNumber))
					{
						ClearFocus();
						focusCharacter = static_cast<idAI *>(ent);
						talkCursor = 1;
						focusTime = gameLocal.time + FOCUS_TIME;
						break;
					}
				}
				continue;
			}

			if(ent->IsType(idAFEntity_Vehicle::Type))
			{
				gameLocal.clip.TracePoint(trace, start, end, MASK_SHOT_RENDERMODEL, this);
				if((trace.fraction < 1.0f) && (trace.c.entityNum == ent->entityNumber))
				{
					ClearFocus();
					focusVehicle = static_cast<idAFEntity_Vehicle *>(ent);
					focusTime = gameLocal.time + FOCUS_TIME;
					break;
				}
				continue;
			}
		}

		if(!ent->GetRenderEntity() || !ent->GetRenderEntity()->gui[0] || !ent->GetRenderEntity()->gui[0]->IsInteractive())
		{
			continue;
		}

		if(ent->spawnArgs.GetBool("inv_item"))
		{
			// don't allow guis on pickup items focus
			continue;
		}

		pt = gameRenderWorld->GuiTrace(ent->GetModelDefHandle(), start, end);
		if(pt.x != -1)
		{
			// we have a hit
			renderEntity_t *focusGUIrenderEntity = ent->GetRenderEntity();
			if(!focusGUIrenderEntity)
			{
				continue;
			}

			if(pt.guiId == 1)
			{
				ui = focusGUIrenderEntity->gui[0];
			}
			else if(pt.guiId == 2)
			{
				ui = focusGUIrenderEntity->gui[1];
			}
			else
			{
				ui = focusGUIrenderEntity->gui[2];
			}

			if(ui == nullptr)
			{
				continue;
			}

			ClearFocus();
			focusGUIent = ent;
			focusUI = ui;

			if(oldFocus != ent)
			{
				// new activation
				// going to see if we have anything in inventory a gui might be interested in
				// need to enumerate inventory items
				focusUI->SetStateInt("inv_count", inventory.items.Num());
				for(j = 0; j < inventory.items.Num(); j++)
				{
					idDict *item = inventory.items[j];
					const char *iname = item->GetString("inv_name");
					const char *iicon = item->GetString("inv_icon");
					const char *itext = item->GetString("inv_text");

					focusUI->SetStateString(va("inv_name_%i", j), iname);
					focusUI->SetStateString(va("inv_icon_%i", j), iicon);
					focusUI->SetStateString(va("inv_text_%i", j), itext);
					kv = item->MatchPrefix("inv_id", nullptr);
					if(kv)
					{
						focusUI->SetStateString(va("inv_id_%i", j), kv->GetValue());
					}
					focusUI->SetStateInt(iname, 1);
				}

				for(j = 0; j < inventory.pdaSecurity.Num(); j++)
				{
					const char *p = inventory.pdaSecurity[j];
					if(p && *p)
					{
						focusUI->SetStateInt(p, 1);
					}
				}

				int powerCellCount = 0;
				for(j = 0; j < inventory.items.Num(); j++)
				{
					idDict *item = inventory.items[j];
					if(item->GetInt("inv_powercell"))
					{
						powerCellCount++;
					}
				}
				focusUI->SetStateInt("powercell_count", powerCellCount);

				int staminapercentage = (int)(100.0f * stamina / pm_stamina.GetFloat());
				focusUI->SetStateString("player_health", va("%i", health));
				focusUI->SetStateString("player_stamina", va("%i%%", staminapercentage));
				focusUI->SetStateString("player_armor", va("%i%%", inventory.armor));

				kv = focusGUIent->spawnArgs.MatchPrefix("gui_parm", nullptr);
				while(kv)
				{
					focusUI->SetStateString(kv->GetKey(), kv->GetValue());
					kv = focusGUIent->spawnArgs.MatchPrefix("gui_parm", kv);
				}
			}

			// clamp the mouse to the corner
			ev = sys->GenerateMouseMoveEvent(-2000, -2000);
			command = focusUI->HandleEvent(&ev, gameLocal.time);
			HandleGuiCommands(focusGUIent, command);

			// move to an absolute position
			ev = sys->GenerateMouseMoveEvent(pt.x * SCREEN_WIDTH, pt.y * SCREEN_HEIGHT);
			command = focusUI->HandleEvent(&ev, gameLocal.time);
			HandleGuiCommands(focusGUIent, command);
			focusTime = gameLocal.time + FOCUS_GUI_TIME;
			break;
		}
	}

	if(focusGUIent && focusUI)
	{
		if(!oldFocus || oldFocus != focusGUIent)
		{
			command = focusUI->Activate(true, gameLocal.time);
			HandleGuiCommands(focusGUIent, command);
			StartSound("snd_guienter", SND_CHANNEL_ANY, 0, false, nullptr);
			// HideTip();
			// HideObjective();
		}
	}
	else if(oldFocus && oldUI)
	{
		command = oldUI->Activate(false, gameLocal.time);
		HandleGuiCommands(oldFocus, command);
		StartSound("snd_guiexit", SND_CHANNEL_ANY, 0, false, nullptr);
	};

	if(hud)
		hud->SetCursorState(this, CURSOR_TALK, talkCursor);

	if(oldChar != focusCharacter && hud)
	{
		if(focusCharacter)
		{
			hud->SetCursorText("#str_02036", focusCharacter->spawnArgs.GetString("npc_name", "Joe"));
			hud->UpdateCursorState();
		}
		else
		{
			hud->SetCursorText("", "");
			hud->UpdateCursorState();
		}
	}
}

/*
=================
CBasePlayer::CrashLand

Check for hard landings that generate sound events
=================
*/
void CBasePlayer::CrashLand(const CVec3 &oldOrigin, const CVec3 &oldVelocity)
{
	CVec3 origin, velocity;
	CVec3 gravityVector, gravityNormal;
	float delta;
	float hardDelta, fatalDelta, softDelta;
	float dist;
	float vel, acc;
	float t;
	float a, b, c, den;
	waterLevel_t waterLevel;
	bool noDamage;

	AI_SOFTLANDING = false;
	AI_HARDLANDING = false;

	// if the player is not on the ground
	if(!physicsObj.HasGroundContacts())
	{
		return;
	}

	gravityNormal = physicsObj.GetGravityNormal();

	// if the player wasn't going down
	if((oldVelocity * -gravityNormal) >= 0.0f)
	{
		return;
	}

	waterLevel = physicsObj.GetWaterLevel();

	// never take falling damage if completely underwater
	if(waterLevel == WATERLEVEL_HEAD)
	{
		return;
	}

	// no falling damage if touching a nodamage surface
	noDamage = false;
	for(int i = 0; i < physicsObj.GetNumContacts(); i++)
	{
		const contactInfo_t &contact = physicsObj.GetContact(i);
		if(contact.material->GetSurfaceFlags() & SURF_NODAMAGE)
		{
			noDamage = true;
			StartSound("snd_land_hard", SND_CHANNEL_ANY, 0, false, nullptr);
			break;
		}
	}

	origin = GetPhysics()->GetOrigin();
	gravityVector = physicsObj.GetGravity();

	// calculate the exact velocity on landing
	dist = (origin - oldOrigin) * -gravityNormal;
	vel = oldVelocity * -gravityNormal;
	acc = -gravityVector.Length();

	a = acc / 2.0f;
	b = vel;
	c = -dist;

	den = b * b - 4.0f * a * c;
	if(den < 0)
	{
		return;
	}
	t = (-b - CMath::Sqrt(den)) / (2.0f * a);

	delta = vel + t * acc;
	delta = delta * delta * 0.0001;

	// reduce falling damage if there is standing water
	if(waterLevel == WATERLEVEL_WAIST)
	{
		delta *= 0.25f;
	}
	if(waterLevel == WATERLEVEL_FEET)
	{
		delta *= 0.5f;
	}

	if(delta < 1.0f)
	{
		return;
	}

	// allow falling a bit further for multiplayer
	if(mpGame->GetRules()->IsMultiplayer())
	{
		fatalDelta = 75.0f;
		hardDelta = 50.0f;
		softDelta = 45.0f;
	}
	else
	{
		fatalDelta = 65.0f;
		hardDelta = 45.0f;
		softDelta = 30.0f;
	}

	if(delta > fatalDelta)
	{
		AI_HARDLANDING = true;
		landChange = -32;
		landTime = gameLocal.time;
		if(!noDamage)
		{
			pain_debounce_time = gameLocal.time + pain_delay + 1; // ignore pain since we'll play our landing anim
			Damage(nullptr, nullptr, CVec3(0, 0, -1), "damage_fatalfall", 1.0f, 0);
		}
	}
	else if(delta > hardDelta)
	{
		AI_HARDLANDING = true;
		landChange = -24;
		landTime = gameLocal.time;
		if(!noDamage)
		{
			pain_debounce_time = gameLocal.time + pain_delay + 1; // ignore pain since we'll play our landing anim
			Damage(nullptr, nullptr, CVec3(0, 0, -1), "damage_hardfall", 1.0f, 0);
		}
	}
	else if(delta > softDelta)
	{
		AI_HARDLANDING = true;
		landChange = -16;
		landTime = gameLocal.time;
		if(!noDamage)
		{
			pain_debounce_time = gameLocal.time + pain_delay + 1; // ignore pain since we'll play our landing anim
			Damage(nullptr, nullptr, CVec3(0, 0, -1), "damage_softfall", 1.0f, 0);
		}
	}
	else if(delta > 7)
	{
		AI_SOFTLANDING = true;
		landChange = -8;
		landTime = gameLocal.time;
	}
	else if(delta > 3)
	{
		// just walk on
	}
}

/*
===============
CBasePlayer::BobCycle
===============
*/
void CBasePlayer::BobCycle(const CVec3 &pushVelocity)
{
	float bobmove;
	int old, deltaTime;
	CVec3 vel, gravityDir, velocity;
	idMat3 viewaxis;
	float bob;
	float delta;
	float speed;
	float f;

	//
	// calculate speed and cycle to be used for
	// all cyclic walking effects
	//
	velocity = physicsObj.GetLinearVelocity() - pushVelocity;

	gravityDir = physicsObj.GetGravityNormal();
	vel = velocity - (velocity * gravityDir) * gravityDir;
	xyspeed = vel.LengthFast();

	// do not evaluate the bob for other clients
	// when doing a spectate follow, don't do any weapon bobbing
	if(common->IsClient() && !IsLocallyControlled())
	{
		viewBobAngles.Zero();
		viewBob.Zero();
		return;
	}

	if(!physicsObj.HasGroundContacts() || influenceActive == INFLUENCE_LEVEL2 || (mpGame->GetRules()->IsMultiplayer() && spectating))
	{
		// airborne
		bobCycle = 0;
		bobFoot = 0;
		bobfracsin = 0;
	}
	else if((!usercmd.forwardmove && !usercmd.rightmove) || (xyspeed <= MIN_BOB_SPEED))
	{
		// start at beginning of cycle again
		bobCycle = 0;
		bobFoot = 0;
		bobfracsin = 0;
	}
	else
	{
		if(physicsObj.IsCrouching())
		{
			bobmove = pm_crouchbob.GetFloat();
			// ducked characters never play footsteps
		}
		else
		{
			// vary the bobbing based on the speed of the player
			bobmove = pm_walkbob.GetFloat() * (1.0f - bobFrac) + pm_runbob.GetFloat() * bobFrac;
		}

		// check for footstep / splash sounds
		old = bobCycle;
		bobCycle = (int)(old + bobmove * (gameLocal.time - gameLocal.previousTime)) & 255;
		bobFoot = (bobCycle & 128) >> 7;
		bobfracsin = CMath::Fabs(sin((bobCycle & 127) / 127.0 * CMath::PI));
	}

	// calculate angles for view bobbing
	viewBobAngles.Zero();

	viewaxis = viewAngles.ToMat3() * physicsObj.GetGravityAxis();

	// add angles based on velocity
	delta = velocity * viewaxis[0];
	viewBobAngles.pitch += delta * pm_runpitch.GetFloat();

	delta = velocity * viewaxis[1];
	viewBobAngles.roll -= delta * pm_runroll.GetFloat();

	// add angles based on bob
	// make sure the bob is visible even at low speeds
	speed = xyspeed > 200 ? xyspeed : 200;

	delta = bobfracsin * pm_bobpitch.GetFloat() * speed;
	if(physicsObj.IsCrouching())
	{
		delta *= 3; // crouching
	}
	viewBobAngles.pitch += delta;
	delta = bobfracsin * pm_bobroll.GetFloat() * speed;
	if(physicsObj.IsCrouching())
	{
		delta *= 3; // crouching accentuates roll
	}
	if(bobFoot & 1)
	{
		delta = -delta;
	}
	viewBobAngles.roll += delta;

	// calculate position for view bobbing
	viewBob.Zero();

	if(physicsObj.HasSteppedUp())
	{
		// check for stepping up before a previous step is completed
		deltaTime = gameLocal.time - stepUpTime;
		if(deltaTime < STEPUP_TIME)
		{
			stepUpDelta = stepUpDelta * (STEPUP_TIME - deltaTime) / STEPUP_TIME + physicsObj.GetStepUp();
		}
		else
		{
			stepUpDelta = physicsObj.GetStepUp();
		}
		if(stepUpDelta > 2.0f * pm_stepsize.GetFloat())
		{
			stepUpDelta = 2.0f * pm_stepsize.GetFloat();
		}
		stepUpTime = gameLocal.time;
	}

	CVec3 gravity = physicsObj.GetGravityNormal();

	// if the player stepped up recently
	deltaTime = gameLocal.time - stepUpTime;
	if(deltaTime < STEPUP_TIME)
	{
		viewBob += gravity * (stepUpDelta * (STEPUP_TIME - deltaTime) / STEPUP_TIME);
	}

	// add bob height after any movement smoothing
	bob = bobfracsin * xyspeed * pm_bobup.GetFloat();
	if(bob > 6)
	{
		bob = 6;
	}
	viewBob[2] += bob;

	// add fall height
	delta = gameLocal.time - landTime;
	if(delta < LAND_DEFLECT_TIME)
	{
		f = delta / LAND_DEFLECT_TIME;
		viewBob -= gravity * (landChange * f);
	}
	else if(delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME)
	{
		delta -= LAND_DEFLECT_TIME;
		f = 1.0 - (delta / LAND_RETURN_TIME);
		viewBob -= gravity * (landChange * f);
	}
}

/*
================
CBasePlayer::UpdateDeltaViewAngles
================
*/
void CBasePlayer::UpdateDeltaViewAngles(const idAngles &angles)
{
	// set the delta angle
	idAngles delta;
	for(int i = 0; i < 3; i++)
	{
		delta[i] = angles[i] - SHORT2ANGLE(usercmd.angles[i]);
	}
	SetDeltaViewAngles(delta);
}

/*
================
CBasePlayer::SetViewAngles
================
*/
void CBasePlayer::SetViewAngles(const idAngles &angles)
{
	UpdateDeltaViewAngles(angles);
	viewAngles = angles;
}

/*
================
CBasePlayer::UpdateViewAngles
================
*/
void CBasePlayer::UpdateViewAngles()
{
	int i;
	idAngles delta;

	if(!noclip && (gameLocal.inCinematic || privateCameraView || gameLocal.GetCamera() || influenceActive == INFLUENCE_LEVEL2 || objectiveSystemOpen))
	{
		// no view changes at all, but we still want to update the deltas or else when
		// we get out of this mode, our view will snap to a kind of random angle
		UpdateDeltaViewAngles(viewAngles);
		return;
	}

	// if dead
	if(health <= 0)
	{
		if(pm_thirdPersonDeath.GetBool())
		{
			viewAngles.roll = 0.0f;
			viewAngles.pitch = 30.0f;
		}
		else
		{
			viewAngles.roll = 40.0f;
			viewAngles.pitch = -15.0f;
		}
		return;
	}

	//

	// circularly clamp the angles with deltas
	for(i = 0; i < 3; i++)
	{
		cmdAngles[i] = SHORT2ANGLE(usercmd.angles[i]);
		if(influenceActive == INFLUENCE_LEVEL3)
		{
			viewAngles[i] += CMath::ClampFloat(-1.0f, 1.0f, CMath::AngleDelta(CMath::AngleNormalize180(SHORT2ANGLE(usercmd.angles[i]) + deltaViewAngles[i]), viewAngles[i]));
		}
		else
		{
			viewAngles[i] = CMath::AngleNormalize180(SHORT2ANGLE(usercmd.angles[i]) + deltaViewAngles[i]);
		}
	}
	if(!centerView.IsDone(gameLocal.time))
	{
		viewAngles.pitch = centerView.GetCurrentValue(gameLocal.time);
	}

	// clamp the pitch
	if(noclip)
	{
		if(viewAngles.pitch > 89.0f)
		{
			// don't let the player look down more than 89 degrees while noclipping
			viewAngles.pitch = 89.0f;
		}
		else if(viewAngles.pitch < -89.0f)
		{
			// don't let the player look up more than 89 degrees while noclipping
			viewAngles.pitch = -89.0f;
		}
	}
	else if(mountedObject)
	{
		int yaw_min, yaw_max, varc;

		mountedObject->GetAngleRestrictions(yaw_min, yaw_max, varc);

		if(yaw_min < yaw_max)
		{
			viewAngles.yaw = CMath::ClampFloat(yaw_min, yaw_max, viewAngles.yaw);
		}
		else
		{
			if(viewAngles.yaw < 0)
			{
				viewAngles.yaw = CMath::ClampFloat(-180.f, yaw_max, viewAngles.yaw);
			}
			else
			{
				viewAngles.yaw = CMath::ClampFloat(yaw_min, 180.f, viewAngles.yaw);
			}
		}
		viewAngles.pitch = CMath::ClampFloat(-varc, varc, viewAngles.pitch);
	}
	else
	{
		// don't let the player look up or down more than 90 degrees normally
		const float restrict = 1.0f;

		viewAngles.pitch = std::min(viewAngles.pitch, pm_maxviewpitch.GetFloat() * restrict);
		viewAngles.pitch = std::max(viewAngles.pitch, pm_minviewpitch.GetFloat() * restrict);
	}

	UpdateDeltaViewAngles(viewAngles);

	// orient the model towards the direction we're looking
	SetAngles(idAngles(0, viewAngles.yaw, 0));

	// save in the log for analyzing weapon angle offsets
	loggedViewAngles[gameLocal.framenum & (NUM_LOGGED_VIEW_ANGLES - 1)] = viewAngles;
}

/*
==============
CBasePlayer::AdjustHeartRate

Player heartrate works as follows

DEF_HEARTRATE is resting heartrate

Taking damage when health is above 75 adjusts heart rate by 1 beat per second
Taking damage when health is below 75 adjusts heart rate by 5 beats per second
Maximum heartrate from damage is MAX_HEARTRATE

Firing a weapon adds 1 beat per second up to a maximum of COMBAT_HEARTRATE

Being at less than 25% stamina adds 5 beats per second up to ZEROSTAMINA_HEARTRATE

All heartrates are target rates.. the heart rate will start falling as soon as there have been no adjustments for 5 seconds
Once it starts falling it always tries to get to DEF_HEARTRATE

The exception to the above rule is upon death at which point the rate is set to DYING_HEARTRATE and starts falling 
immediately to zero

Heart rate volumes go from zero ( -40 db for DEF_HEARTRATE to 5 db for MAX_HEARTRATE ) the volume is 
scaled linearly based on the actual rate

Exception to the above rule is once the player is dead, the dying heart rate starts at either the current volume if
it is audible or -10db and scales to 8db on the last few beats
==============
*/
void CBasePlayer::AdjustHeartRate(int target, float timeInSecs, float delay, bool force)
{
	if(heartInfo.GetEndValue() == target)
	{
		return;
	}

	if(AI_DEAD && !force)
	{
		return;
	}

	lastHeartAdjust = gameLocal.time;

	heartInfo.Init(gameLocal.time + delay * 1000, timeInSecs * 1000, heartRate, target);
}

/*
==============
CBasePlayer::GetBaseHeartRate
==============
*/
int CBasePlayer::GetBaseHeartRate()
{
	int base = CMath::Ftoi((BASE_HEARTRATE + LOWHEALTH_HEARTRATE_ADJ) - ((float)health / 100.0f) * LOWHEALTH_HEARTRATE_ADJ);
	int rate = CMath::Ftoi(base + (ZEROSTAMINA_HEARTRATE - base) * (1.0f - stamina / pm_stamina.GetFloat()));
	int diff = (lastDmgTime) ? gameLocal.time - lastDmgTime : 99999;
	rate += (diff < 5000) ? (diff < 2500) ? (diff < 1000) ? 15 : 10 : 5 : 0;
	return rate;
}

/*
==============
CBasePlayer::SetCurrentHeartRate
==============
*/
void CBasePlayer::SetCurrentHeartRate()
{
	int base = CMath::Ftoi((BASE_HEARTRATE + LOWHEALTH_HEARTRATE_ADJ) - ((float)health / 100.0f) * LOWHEALTH_HEARTRATE_ADJ);

	if(PowerUpActive(ADRENALINE))
	{
		heartRate = 135;
	}
	else
	{
		heartRate = CMath::Ftoi(heartInfo.GetCurrentValue(gameLocal.time));
		int currentRate = GetBaseHeartRate();
		if(health >= 0 && gameLocal.time > lastHeartAdjust + 2500)
		{
			AdjustHeartRate(currentRate, 2.5f, 0.0f, false);
		}
	}

	int bps = CMath::Ftoi(60.0f / heartRate * 1000.0f);
	if(gameLocal.time - lastHeartBeat > bps)
	{
		int dmgVol = DMG_VOLUME;
		int deathVol = DEATH_VOLUME;
		int zeroVol = ZERO_VOLUME;
		float pct = 0.0;
		if(heartRate > BASE_HEARTRATE && health > 0)
		{
			pct = (float)(heartRate - base) / (MAX_HEARTRATE - base);
			pct *= ((float)dmgVol - (float)zeroVol);
		}
		else if(health <= 0)
		{
			pct = (float)(heartRate - DYING_HEARTRATE) / (BASE_HEARTRATE - DYING_HEARTRATE);
			if(pct > 1.0f)
			{
				pct = 1.0f;
			}
			else if(pct < 0.0f)
			{
				pct = 0.0f;
			}
			pct *= ((float)deathVol - (float)zeroVol);
		}

		pct += (float)zeroVol;

		if(pct != zeroVol)
		{
			StartSound("snd_heartbeat", SND_CHANNEL_HEART, SSF_PRIVATE_SOUND, false, nullptr);
			// modify just this channel to a custom volume
			soundShaderParms_t parms;
			memset(&parms, 0, sizeof(parms));
			parms.volume = pct;
			refSound.referenceSound->ModifySound(SND_CHANNEL_HEART, &parms);
		}

		lastHeartBeat = gameLocal.time;
	}
}

/*
==============
CBasePlayer::UpdateAir
==============
*/
void CBasePlayer::UpdateAir()
{
	if(health <= 0)
	{
		return;
	}

	// see if the player is connected to the info_vacuum
	bool newAirless = false;

	if(gameLocal.vacuumAreaNum != -1)
	{
		int num = GetNumPVSAreas();
		if(num > 0)
		{
			int areaNum;

			// if the player box spans multiple areas, get the area from the origin point instead,
			// otherwise a rotating player box may poke into an outside area
			if(num == 1)
			{
				const int *pvsAreas = GetPVSAreas();
				areaNum = pvsAreas[0];
			}
			else
			{
				areaNum = gameRenderWorld->PointInArea(this->GetPhysics()->GetOrigin());
			}
			newAirless = gameRenderWorld->AreasAreConnected(gameLocal.vacuumAreaNum, areaNum, PS_BLOCK_AIR);
		}
	}

	if(PowerUpActive(ENVIROTIME))
	{
		newAirless = false;
	}

	if(newAirless)
	{
		if(!airless)
		{
			StartSound("snd_decompress", SND_CHANNEL_ANY, SSF_GLOBAL, false, nullptr);
			StartSound("snd_noAir", SND_CHANNEL_BODY2, 0, false, nullptr);
		}
		airMsec -= (gameLocal.time - gameLocal.previousTime);
		if(airMsec < 0)
		{
			airMsec = 0;
			// check for damage
			const idDict *damageDef = gameLocal.FindEntityDefDict("damage_noair", false);
			int dmgTiming = 1000 * ((damageDef) ? damageDef->GetFloat("delay", "3.0") : 3.0f);
			if(gameLocal.time > lastAirDamage + dmgTiming)
			{
				Damage(nullptr, nullptr, vec3_origin, "damage_noair", 1.0f, 0);
				lastAirDamage = gameLocal.time;
			}
		}
	}
	else
	{
		if(airless)
		{
			StartSound("snd_recompress", SND_CHANNEL_ANY, SSF_GLOBAL, false, nullptr);
			StopSound(SND_CHANNEL_BODY2, false);
		}
		airMsec += (gameLocal.time - gameLocal.previousTime); // regain twice as fast as lose
		if(airMsec > pm_airMsec.GetInteger())
		{
			airMsec = pm_airMsec.GetInteger();
		}
	}

	airless = newAirless;

	if(hud)
		hud->UpdateOxygen(airless, 100 * airMsec / pm_airMsec.GetInteger());
}

void CBasePlayer::UpdatePowerupHud()
{
	if(health <= 0)
	{
		return;
	}

	if(lastHudPowerup != hudPowerup)
	{
		if(hudPowerup == -1)
		{
			//The powerup hud should be turned off
			// TODO_SPARTY: powerups??
			//if ( hud ) {
			//	hud->HandleNamedEvent( "noPowerup" );
			//}
		}
		else
		{
			//Turn the pwoerup hud on
			// TODO_SPARTY: powerups??
			//if ( hud ) {
			//	hud->HandleNamedEvent( "Powerup" );
			//}
		}

		lastHudPowerup = hudPowerup;
	}

	if(hudPowerup != -1 && hudPowerup < MAX_POWERUPS)
	{
		if(PowerUpActive(hudPowerup))
		{
			//int remaining = inventory.powerupEndTime[ hudPowerup ] - gameLocal.time;
			//int filledbar = CMath::ClampInt( 0, hudPowerupDuration, remaining );

			// TODO_SPARTY: powerups??
			//if ( hud ) {
			//	hud->SetStateInt( "player_powerup", 100 * filledbar / hudPowerupDuration );
			//	hud->SetStateInt( "player_poweruptime", remaining / 1000 );
			//}
		}
	}
}

/*
==============
CBasePlayer::GetPDA
==============
 */
const idDeclPDA *CBasePlayer::GetPDA() const
{
	if(inventory.pdas.Num() > 0)
	{
		return inventory.pdas[0];
	}
	else
	{
		return nullptr;
	}
}

/*
==============
CBasePlayer::GetVideo
==============
*/
const idDeclVideo *CBasePlayer::GetVideo(int index)
{
	if(index >= 0 && index < inventory.videos.Num())
	{
		return inventory.videos[index];
	}
	return nullptr;
}

/*
==============
CBasePlayer::Spectate
==============
*/
void CBasePlayer::Spectate(bool spectate, bool force)
{
	spectating = spectate;

	if(spectating)
	{
		// join the spectators
		ClearPowerUps();
		spectator = this->entityNumber;
		Init();
		StopRagdoll();
		SetPhysics(&physicsObj);
		physicsObj.DisableClip();
		FlashlightOff();
		Hide();
		Event_DisableWeapon();

		// Raise me up by a little bit. if i'm the local client.
		if(IsLocallyControlled())
		{
			SetSpectateOrigin();
		}

		HideRespawnHudMessage();

		idLib::Printf("DMP _ GENERAL :> Player %d Spectating \n", entityNumber);
	}
	else
	{
		// put everything back together again
		currentWeapon = -1; // to make sure the def will be loaded if necessary
		Show();
		Event_EnableWeapon();
		idLib::Printf("DMP _ GENERAL :> Player %d Not Spectating \n", entityNumber);
		SetEyeHeight(pm_normalviewheight.GetFloat());
	}
	SetClipModel();
}

/*
==============
CBasePlayer::SetClipModel
==============
*/
void CBasePlayer::SetClipModel()
{
	idBounds bounds;

	if(spectating)
	{
		bounds = idBounds(vec3_origin).Expand(pm_spectatebbox.GetFloat() * 0.5f);
	}
	else
	{
		bounds[0].Set(-pm_bboxwidth.GetFloat() * 0.5f, -pm_bboxwidth.GetFloat() * 0.5f, 0);
		bounds[1].Set(pm_bboxwidth.GetFloat() * 0.5f, pm_bboxwidth.GetFloat() * 0.5f, pm_normalheight.GetFloat());
	}
	// the origin of the clip model needs to be set before calling SetClipModel
	// otherwise our physics object's current origin value gets reset to 0
	idClipModel *newClip;
	if(pm_usecylinder.GetBool())
	{
		newClip = new(TAG_PHYSICS_CLIP_ENTITY) idClipModel(idTraceModel(bounds, 8));
		newClip->Translate(physicsObj.PlayerGetOrigin());
		physicsObj.SetClipModel(newClip, 1.0f);
	}
	else
	{
		newClip = new(TAG_PHYSICS_CLIP_ENTITY) idClipModel(idTraceModel(bounds));
		newClip->Translate(physicsObj.PlayerGetOrigin());
		physicsObj.SetClipModel(newClip, 1.0f);
	}
}

/*
==============
CBasePlayer::UseVehicle
==============
*/
void CBasePlayer::UseVehicle()
{
	trace_t trace;
	CVec3 start, end;
	CBaseEntity *ent;

	if(GetBindMaster() && GetBindMaster()->IsType(idAFEntity_Vehicle::Type))
	{
		Show();
		static_cast<idAFEntity_Vehicle *>(GetBindMaster())->Use(this);
	}
	else
	{
		start = GetEyePosition();
		end = start + viewAngles.ToForward() * 80.0f;
		gameLocal.clip.TracePoint(trace, start, end, MASK_SHOT_RENDERMODEL, this);
		if(trace.fraction < 1.0f)
		{
			ent = gameLocal.entities[trace.c.entityNum];
			if(ent && ent->IsType(idAFEntity_Vehicle::Type))
			{
				Hide();
				static_cast<idAFEntity_Vehicle *>(ent)->Use(this);
			}
		}
	}
}

/*
==============
CBasePlayer::PerformImpulse
==============
*/
void CBasePlayer::PerformImpulse(int impulse)
{
	bool isIntroMap = (idStr::FindText(gameLocal.GetMapFileName(), "mars_city1") >= 0);

	// Normal 1 - 0 Keys.
	if(impulse >= IMPULSE_0 && impulse <= IMPULSE_12 && !isIntroMap)
	{
		SelectWeapon(impulse, false);
		return;
	}

	// DPAD Weapon Quick Select
	if(impulse >= IMPULSE_28 && impulse <= IMPULSE_31 && !isIntroMap)
	{
		SelectWeapon(impulse, false);
		return;
	}

	switch(impulse)
	{
	case IMPULSE_13:
	{
		Reload();
		break;
	}
	case IMPULSE_14:
	{
		if(!isIntroMap)
		{
			NextWeapon();
		}
		break;
	}
	case IMPULSE_15:
	{
		if(!isIntroMap)
		{
			PrevWeapon();
		}
		break;
	}
	case IMPULSE_16:
	{
		if(flashlight.IsValid())
		{
			if(flashlight.GetEntity()->lightOn)
			{
				FlashlightOff();
			}
			else if(!spectating && weaponEnabled && !hiddenWeapon && !gameLocal.world->spawnArgs.GetBool("no_Weapons"))
			{
				FlashlightOn();
			}
		}
		break;
	}
	case IMPULSE_19:
	{
		// when we're not in single player, IMPULSE_19 is used for showScores
		// otherwise it opens the pda
		if(!mpGame->GetRules()->IsMultiplayer())
		{
#if !defined(ID_RETAIL) && !defined(ID_RETAIL_INTERNAL)
			if(!common->KeyState(56))
			{ // don't toggle PDA when LEFT ALT is down
#endif
				if(objectiveSystemOpen)
				{
					TogglePDA();
				}
				else if(weapon_pda >= 0)
				{
					SelectWeapon(weapon_pda, true);
				}
#if !defined(ID_RETAIL) && !defined(ID_RETAIL_INTERNAL)
			}
#endif
		}
		else
		{
			if(IsLocallyControlled())
			{
				gameLocal.mpGame.SetScoreboardActive(true);
			}
		}
		break;
	}
	case IMPULSE_22:
	{
		if(common->IsClient() || IsLocallyControlled())
		{
			gameLocal.mpGame.ToggleSpectate();
		}
		break;
	}
	case IMPULSE_25:
	{
		if(common->IsServer() && gameLocal.mpGame.IsGametypeFlagBased() && (gameLocal.serverInfo.GetInt("si_midnight") == 2))
		{
			if(enviroSuitLight.IsValid())
			{
				enviroSuitLight.GetEntity()->PostEventMS(&EV_Remove, 0);
				enviroSuitLight = nullptr;
			}
			else
			{
				const idDict *lightDef = gameLocal.FindEntityDefDict("envirosuit_light", false);
				if(lightDef)
				{
					CBaseEntity *temp = static_cast<CBaseEntity *>(enviroSuitLight.GetEntity());
					idAngles lightAng = firstPersonViewAxis.ToAngles();
					CVec3 lightOrg = firstPersonViewOrigin;

					CVec3 enviroOffset = lightDef->GetVector("enviro_offset");
					CVec3 enviroAngleOffset = lightDef->GetVector("enviro_angle_offset");

					gameLocal.SpawnEntityDef(*lightDef, &temp, false);
					enviroSuitLight = static_cast<idLight *>(temp);

					enviroSuitLight.GetEntity()->fl.networkSync = true;

					lightOrg += (enviroOffset.x * firstPersonViewAxis[0]);
					lightOrg += (enviroOffset.y * firstPersonViewAxis[1]);
					lightOrg += (enviroOffset.z * firstPersonViewAxis[2]);
					lightAng.pitch += enviroAngleOffset.x;
					lightAng.yaw += enviroAngleOffset.y;
					lightAng.roll += enviroAngleOffset.z;

					enviroSuitLight.GetEntity()->GetPhysics()->SetOrigin(lightOrg);
					enviroSuitLight.GetEntity()->GetPhysics()->SetAxis(lightAng.ToMat3());

					enviroSuitLight.GetEntity()->UpdateVisuals();
					enviroSuitLight.GetEntity()->Present();
				}
			}
		}
		break;
	}
	//Hack so the chainsaw will work in MP
	case IMPULSE_27:
	{
		SelectWeapon(18, false);
		break;
	}
	}
}

/*
==============
CBasePlayer::EvaluateControls
==============
*/
void CBasePlayer::EvaluateControls()
{
	// check for respawning
	if(health <= 0 && !g_testDeath.GetBool())
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			// in MP, idMultiplayerGame decides spawns
			if((gameLocal.time > minRespawnTime) && (usercmd.buttons & BUTTON_ATTACK))
			{
				forceRespawn = true;
			}
			else if(gameLocal.time > maxRespawnTime)
			{
				forceRespawn = true;
			}
		}
		else
		{
			// in single player, we let the session handle restarting the level or loading a game
			if(gameLocal.time > minRespawnTime)
			{
				gameLocal.sessionCommand = "died";
			}
		}
	}

	if(usercmd.impulseSequence != oldImpulseSequence)
	{
		PerformImpulse(usercmd.impulse);
	}

	if(forceScoreBoard)
	{
		gameLocal.mpGame.SetScoreboardActive(true);
	}

	oldImpulseSequence = usercmd.impulseSequence;

	AdjustSpeed();

	// update the viewangles
	UpdateViewAngles();
}

/*
==============
CBasePlayer::AdjustSpeed
==============
*/
void CBasePlayer::AdjustSpeed()
{
	float speed;
	float rate;

	if(spectating)
	{
		speed = pm_spectatespeed.GetFloat();
		bobFrac = 0.0f;
	}
	else if(noclip)
	{
		speed = pm_noclipspeed.GetFloat();
		bobFrac = 0.0f;
	}
	else if(!physicsObj.OnLadder() && (usercmd.buttons & BUTTON_RUN) && (usercmd.forwardmove || usercmd.rightmove) && !(usercmd.buttons & BUTTON_CROUCH))
	{
		if(!mpGame->GetRules()->IsMultiplayer() && !physicsObj.IsCrouching() && !PowerUpActive(ADRENALINE))
		{
			stamina -= MS2SEC(gameLocal.time - gameLocal.previousTime);
		}
		if(stamina < 0)
		{
			stamina = 0;
		}
		if((!pm_stamina.GetFloat()) || (stamina > pm_staminathreshold.GetFloat()))
		{
			bobFrac = 1.0f;
		}
		else if(pm_staminathreshold.GetFloat() <= 0.0001f)
		{
			bobFrac = 0.0f;
		}
		else
		{
			bobFrac = stamina / pm_staminathreshold.GetFloat();
		}
		speed = pm_walkspeed.GetFloat() * (1.0f - bobFrac) + pm_runspeed.GetFloat() * bobFrac;
	}
	else
	{
		rate = pm_staminarate.GetFloat();

		// increase 25% faster when not moving
		if((usercmd.forwardmove == 0) && (usercmd.rightmove == 0) && (!physicsObj.OnLadder() || ((usercmd.buttons & (BUTTON_CROUCH | BUTTON_JUMP)) == 0)))
		{
			rate *= 1.25f;
		}

		stamina += rate * MS2SEC(gameLocal.time - gameLocal.previousTime);
		if(stamina > pm_stamina.GetFloat())
		{
			stamina = pm_stamina.GetFloat();
		}
		speed = pm_walkspeed.GetFloat();
		bobFrac = 0.0f;
	}

	speed *= PowerUpModifier(SPEED);

	if(influenceActive == INFLUENCE_LEVEL3)
	{
		speed *= 0.33f;
	}

	physicsObj.SetSpeed(speed, pm_crouchspeed.GetFloat());
}

/*
==============
CBasePlayer::AdjustBodyAngles
==============
*/
void CBasePlayer::AdjustBodyAngles()
{
	idMat3 lookAxis;
	idMat3 legsAxis;
	bool blend;
	float diff;
	float frac;
	float upBlend;
	float forwardBlend;
	float downBlend;

	if(health < 0)
	{
		return;
	}

	blend = true;

	if(!physicsObj.HasGroundContacts())
	{
		idealLegsYaw = 0.0f;
		legsForward = true;
	}
	else if(usercmd.forwardmove < 0)
	{
		idealLegsYaw = CMath::AngleNormalize180(CVec3(-usercmd.forwardmove, usercmd.rightmove, 0.0f).ToYaw());
		legsForward = false;
	}
	else if(usercmd.forwardmove > 0)
	{
		idealLegsYaw = CMath::AngleNormalize180(CVec3(usercmd.forwardmove, -usercmd.rightmove, 0.0f).ToYaw());
		legsForward = true;
	}
	else if((usercmd.rightmove != 0) && physicsObj.IsCrouching())
	{
		if(!legsForward)
		{
			idealLegsYaw = CMath::AngleNormalize180(CVec3(CMath::Abs(usercmd.rightmove), usercmd.rightmove, 0.0f).ToYaw());
		}
		else
		{
			idealLegsYaw = CMath::AngleNormalize180(CVec3(CMath::Abs(usercmd.rightmove), -usercmd.rightmove, 0.0f).ToYaw());
		}
	}
	else if(usercmd.rightmove != 0)
	{
		idealLegsYaw = 0.0f;
		legsForward = true;
	}
	else
	{
		legsForward = true;
		diff = CMath::Fabs(idealLegsYaw - legsYaw);
		idealLegsYaw = idealLegsYaw - CMath::AngleNormalize180(viewAngles.yaw - oldViewYaw);
		if(diff < 0.1f)
		{
			legsYaw = idealLegsYaw;
			blend = false;
		}
	}

	if(!physicsObj.IsCrouching())
	{
		legsForward = true;
	}

	oldViewYaw = viewAngles.yaw;

	AI_TURN_LEFT = false;
	AI_TURN_RIGHT = false;
	if(idealLegsYaw < -45.0f)
	{
		idealLegsYaw = 0;
		AI_TURN_RIGHT = true;
		blend = true;
	}
	else if(idealLegsYaw > 45.0f)
	{
		idealLegsYaw = 0;
		AI_TURN_LEFT = true;
		blend = true;
	}

	if(blend)
	{
		legsYaw = legsYaw * 0.9f + idealLegsYaw * 0.1f;
	}
	legsAxis = idAngles(0.0f, legsYaw, 0.0f).ToMat3();
	animator.SetJointAxis(hipJoint, JOINTMOD_WORLD, legsAxis);

	// calculate the blending between down, straight, and up
	frac = viewAngles.pitch / 90.0f;
	if(frac > 0.0f)
	{
		downBlend = frac;
		forwardBlend = 1.0f - frac;
		upBlend = 0.0f;
	}
	else
	{
		downBlend = 0.0f;
		forwardBlend = 1.0f + frac;
		upBlend = -frac;
	}

	animator.CurrentAnim(ANIMCHANNEL_TORSO)->SetSyncedAnimWeight(0, downBlend);
	animator.CurrentAnim(ANIMCHANNEL_TORSO)->SetSyncedAnimWeight(1, forwardBlend);
	animator.CurrentAnim(ANIMCHANNEL_TORSO)->SetSyncedAnimWeight(2, upBlend);

	animator.CurrentAnim(ANIMCHANNEL_LEGS)->SetSyncedAnimWeight(0, downBlend);
	animator.CurrentAnim(ANIMCHANNEL_LEGS)->SetSyncedAnimWeight(1, forwardBlend);
	animator.CurrentAnim(ANIMCHANNEL_LEGS)->SetSyncedAnimWeight(2, upBlend);
}

/*
==============
CBasePlayer::InitAASLocation
==============
*/
void CBasePlayer::InitAASLocation()
{
	int i;
	int num;
	CVec3 size;
	idBounds bounds;
	idAAS *aas;
	CVec3 origin;

	GetFloorPos(64.0f, origin);

	num = gameLocal.NumAAS();
	aasLocation.SetGranularity(1);
	aasLocation.SetNum(num);
	for(i = 0; i < aasLocation.Num(); i++)
	{
		aasLocation[i].areaNum = 0;
		aasLocation[i].pos = origin;
		aas = gameLocal.GetAAS(i);
		if(aas != nullptr && aas->GetSettings())
		{
			size = aas->GetSettings()->boundingBoxes[0][1];
			bounds[0] = -size;
			size.z = 32.0f;
			bounds[1] = size;

			aasLocation[i].areaNum = aas->PointReachableAreaNum(origin, bounds, AREA_REACHABLE_WALK);
		}
	}
}

/*
==============
CBasePlayer::SetAASLocation
==============
*/
void CBasePlayer::SetAASLocation()
{
	int i;
	int areaNum;
	CVec3 size;
	idBounds bounds;
	idAAS *aas;
	CVec3 origin;

	if(!GetFloorPos(64.0f, origin))
	{
		return;
	}

	for(i = 0; i < aasLocation.Num(); i++)
	{
		aas = gameLocal.GetAAS(i);
		if(!aas)
		{
			continue;
		}

		size = aas->GetSettings()->boundingBoxes[0][1];
		bounds[0] = -size;
		size.z = 32.0f;
		bounds[1] = size;

		areaNum = aas->PointReachableAreaNum(origin, bounds, AREA_REACHABLE_WALK);
		if(areaNum)
		{
			aasLocation[i].pos = origin;
			aasLocation[i].areaNum = areaNum;
		}
	}
}

/*
==============
CBasePlayer::GetAASLocation
==============
*/
void CBasePlayer::GetAASLocation(idAAS *aas, CVec3 &pos, int &areaNum) const
{
	int i;

	if(aas != nullptr)
	{
		for(i = 0; i < aasLocation.Num(); i++)
		{
			if(aas == gameLocal.GetAAS(i))
			{
				areaNum = aasLocation[i].areaNum;
				pos = aasLocation[i].pos;
				return;
			}
		}
	}

	areaNum = 0;
	pos = physicsObj.GetOrigin();
}

/*
==============
CBasePlayer::Move_Interpolated
==============
*/
void CBasePlayer::Move_Interpolated(float fraction)
{
	float newEyeOffset;
	CVec3 oldOrigin;
	CVec3 oldVelocity;
	CVec3 pushVelocity;

	// save old origin and velocity for crashlanding
	oldOrigin = physicsObj.GetOrigin();
	oldVelocity = physicsObj.GetLinearVelocity();
	pushVelocity = physicsObj.GetPushedLinearVelocity();

	// set physics variables
	physicsObj.SetMaxStepHeight(pm_stepsize.GetFloat());
	physicsObj.SetMaxJumpHeight(pm_jumpheight.GetFloat());

	if(noclip)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_NOCLIP);
	}
	else if(spectating)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_SPECTATOR);
	}
	else if(health <= 0)
	{
		physicsObj.SetContents(CONTENTS_CORPSE | CONTENTS_MONSTERCLIP);
		physicsObj.SetMovementType(PM_DEAD);
	}
	else if(gameLocal.inCinematic || gameLocal.GetCamera() || privateCameraView || (influenceActive == INFLUENCE_LEVEL2))
	{
		physicsObj.SetContents(CONTENTS_BODY);
		physicsObj.SetMovementType(PM_FREEZE);
	}
	else if(mountedObject)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_FREEZE);
	}
	else
	{
		physicsObj.SetContents(CONTENTS_BODY);
		physicsObj.SetMovementType(PM_NORMAL);
	}

	if(spectating)
	{
		physicsObj.SetClipMask(MASK_DEADSOLID);
	}
	else if(health <= 0)
	{
		physicsObj.SetClipMask(MASK_DEADSOLID);
	}
	else
	{
		physicsObj.SetClipMask(MASK_PLAYERSOLID);
	}

	physicsObj.SetDebugLevel(g_debugMove.GetBool());

	{
		CVec3 org;
		idMat3 axis;
		GetViewPos(org, axis);

		physicsObj.SetPlayerInput(usercmd, axis[0]);
	}

	// FIXME: physics gets disabled somehow
	BecomeActive(TH_PHYSICS);
	InterpolatePhysics(fraction);

	// update our last valid AAS location for the AI
	SetAASLocation();

	if(spectating)
	{
		newEyeOffset = 0.0f;
	}
	else if(health <= 0)
	{
		newEyeOffset = pm_deadviewheight.GetFloat();
	}
	else if(physicsObj.IsCrouching())
	{
		newEyeOffset = pm_crouchviewheight.GetFloat();
	}
	else if(GetBindMaster() && GetBindMaster()->IsType(idAFEntity_Vehicle::Type))
	{
		newEyeOffset = 0.0f;
	}
	else
	{
		newEyeOffset = pm_normalviewheight.GetFloat();
	}

	if(EyeHeight() != newEyeOffset)
	{
		if(spectating)
		{
			SetEyeHeight(newEyeOffset);
		}
		else
		{
			// smooth out duck height changes
			SetEyeHeight(EyeHeight() * pm_crouchrate.GetFloat() + newEyeOffset * (1.0f - pm_crouchrate.GetFloat()));
		}
	}

	if(AI_JUMP)
	{
		// bounce the view weapon
		loggedAccel_t *acc = &loggedAccel[currentLoggedAccel & (NUM_LOGGED_ACCELS - 1)];
		currentLoggedAccel++;
		acc->time = gameLocal.time;
		acc->dir[2] = 200;
		acc->dir[0] = acc->dir[1] = 0;
	}

	if(AI_ONLADDER)
	{
		int old_rung = oldOrigin.z / LADDER_RUNG_DISTANCE;
		int new_rung = physicsObj.GetOrigin().z / LADDER_RUNG_DISTANCE;

		if(old_rung != new_rung)
		{
			StartSound("snd_stepladder", SND_CHANNEL_ANY, 0, false, nullptr);
		}
	}

	BobCycle(pushVelocity);
	CrashLand(oldOrigin, oldVelocity);
}

/*
==============
CBasePlayer::Move
==============
*/
void CBasePlayer::Move()
{
	float newEyeOffset;
	CVec3 oldOrigin;
	CVec3 oldVelocity;
	CVec3 pushVelocity;

	// save old origin and velocity for crashlanding
	oldOrigin = physicsObj.GetOrigin();
	oldVelocity = physicsObj.GetLinearVelocity();
	pushVelocity = physicsObj.GetPushedLinearVelocity();

	// set physics variables
	physicsObj.SetMaxStepHeight(pm_stepsize.GetFloat());
	physicsObj.SetMaxJumpHeight(pm_jumpheight.GetFloat());

	if(noclip)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_NOCLIP);
	}
	else if(spectating)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_SPECTATOR);
	}
	else if(health <= 0)
	{
		physicsObj.SetContents(CONTENTS_CORPSE | CONTENTS_MONSTERCLIP);
		physicsObj.SetMovementType(PM_DEAD);
	}
	else if(gameLocal.inCinematic || gameLocal.GetCamera() || privateCameraView || (influenceActive == INFLUENCE_LEVEL2))
	{
		physicsObj.SetContents(CONTENTS_BODY);
		physicsObj.SetMovementType(PM_FREEZE);
	}
	else if(mountedObject)
	{
		physicsObj.SetContents(0);
		physicsObj.SetMovementType(PM_FREEZE);
	}
	else
	{
		physicsObj.SetContents(CONTENTS_BODY);
		physicsObj.SetMovementType(PM_NORMAL);
	}

	if(spectating)
	{
		physicsObj.SetClipMask(MASK_DEADSOLID);
	}
	else if(health <= 0)
	{
		physicsObj.SetClipMask(MASK_DEADSOLID);
	}
	else
	{
		physicsObj.SetClipMask(MASK_PLAYERSOLID);
	}

	physicsObj.SetDebugLevel(g_debugMove.GetBool());

	{
		CVec3 org;
		idMat3 axis;
		GetViewPos(org, axis);

		physicsObj.SetPlayerInput(usercmd, axis[0]);
	}

	// FIXME: physics gets disabled somehow
	BecomeActive(TH_PHYSICS);
	RunPhysics();

	// update our last valid AAS location for the AI
	SetAASLocation();

	if(spectating)
	{
		newEyeOffset = 0.0f;
	}
	else if(health <= 0)
	{
		newEyeOffset = pm_deadviewheight.GetFloat();
	}
	else if(physicsObj.IsCrouching())
	{
		newEyeOffset = pm_crouchviewheight.GetFloat();
	}
	else if(GetBindMaster() && GetBindMaster()->IsType(idAFEntity_Vehicle::Type))
	{
		newEyeOffset = 0.0f;
	}
	else
	{
		newEyeOffset = pm_normalviewheight.GetFloat();
	}

	if(EyeHeight() != newEyeOffset)
	{
		if(spectating)
		{
			SetEyeHeight(newEyeOffset);
		}
		else
		{
			// smooth out duck height changes
			SetEyeHeight(EyeHeight() * pm_crouchrate.GetFloat() + newEyeOffset * (1.0f - pm_crouchrate.GetFloat()));
		}
	}

	if(noclip || gameLocal.inCinematic || (influenceActive == INFLUENCE_LEVEL2))
	{
		AI_CROUCH = false;
		AI_ONGROUND = (influenceActive == INFLUENCE_LEVEL2);
		AI_ONLADDER = false;
		AI_JUMP = false;
	}
	else
	{
		AI_CROUCH = physicsObj.IsCrouching();
		AI_ONGROUND = physicsObj.HasGroundContacts();
		AI_ONLADDER = physicsObj.OnLadder();
		AI_JUMP = physicsObj.HasJumped();

		// check if we're standing on top of a monster and give a push if we are
		CBaseEntity *groundEnt = physicsObj.GetGroundEntity();
		if(groundEnt != nullptr && groundEnt->IsType(idAI::Type))
		{
			CVec3 vel = physicsObj.GetLinearVelocity();
			if(vel.ToVec2().LengthSqr() < 0.1f)
			{
				vel.ToVec2() = physicsObj.GetOrigin().ToVec2() - groundEnt->GetPhysics()->GetAbsBounds().GetCenter().ToVec2();
				vel.ToVec2().NormalizeFast();
				vel.ToVec2() *= pm_walkspeed.GetFloat();
			}
			else
			{
				// give em a push in the direction they're going
				vel *= 1.1f;
			}
			physicsObj.SetLinearVelocity(vel);
		}
	}

	if(AI_JUMP)
	{
		// bounce the view weapon
		loggedAccel_t *acc = &loggedAccel[currentLoggedAccel & (NUM_LOGGED_ACCELS - 1)];
		currentLoggedAccel++;
		acc->time = gameLocal.time;
		acc->dir[2] = 200;
		acc->dir[0] = acc->dir[1] = 0;
	}

	if(AI_ONLADDER)
	{
		int old_rung = oldOrigin.z / LADDER_RUNG_DISTANCE;
		int new_rung = physicsObj.GetOrigin().z / LADDER_RUNG_DISTANCE;

		if(old_rung != new_rung)
		{
			StartSound("snd_stepladder", SND_CHANNEL_ANY, 0, false, nullptr);
		}
	}

	BobCycle(pushVelocity);
	CrashLand(oldOrigin, oldVelocity);
}

/*
========================
CBasePlayer::AllowClientAuthPhysics
========================
*/
bool CBasePlayer::AllowClientAuthPhysics()
{
	// note respawn count > 1: respawn should be called twice - once for initial spawn and once for actual respawn by game mode
	// TODO: I don't think doom 3 will need to care about the respawn count.
	return (usercmd.serverGameMilliseconds > serverOverridePositionTime && commonLocal.GetUCmdMgr().HasUserCmdForPlayer(entityNumber));
}

/*
========================
CBasePlayer::RunPhysics_RemoteClientCorrection
========================
*/
void CBasePlayer::RunPhysics_RemoteClientCorrection()
{
	if(!AllowClientAuthPhysics())
	{
		// We are still overriding client's position
		if(pm_clientAuthoritative_debug.GetBool())
		{
			//clientGame->renderWorld->DebugPoint( idColor::colorRed, GetOrigin() );
			idLib::Printf("[%d]Ignoring client auth:  cmd.serverTime: %d  overrideTime: %d \n", entityNumber, usercmd.serverGameMilliseconds, serverOverridePositionTime);
		}
		return;
	}

	// Client is on a pusher... ignore him so he doesn't lag behind
	bool becameUnlocked = false;
	if(physicsObj.ClientPusherLocked(becameUnlocked))
	{
		// Check and see how far we've diverged.
		CVec3 cmdPos(usercmd.pos[0], usercmd.pos[1], usercmd.pos[2]);
		CVec3 newOrigin = physicsObj.GetOrigin();

		CVec3 divergeVec = cmdPos - newOrigin;
		idLib::Printf("Client Divergence: %s Length: %2f\n", divergeVec.ToString(3), divergeVec.Length());

		// if the client Diverges over a certain amount, snap him back
		if(divergeVec.Length() < pm_clientAuthoritative_Divergence.GetFloat())
		{
			return;
		}
	}
	if(becameUnlocked)
	{
		// Client just got off of a mover, wait before listening to him
		serverOverridePositionTime = gameLocal.GetServerGameTimeMs();
		return;
	}

	// Correction
	{
		CVec3 newOrigin = physicsObj.GetOrigin();
		CVec3 cmdPos(usercmd.pos[0], usercmd.pos[1], usercmd.pos[2]);
		CVec3 desiredPos = cmdPos;

		float delta = (desiredPos - newOrigin).Length();
		// ignore small differences in Z: this can cause player to not have proper ground contacts which messes up
		// velocity/acceleration calculation. If this hack doesn't work out, will may need more precision for at least
		// the Z component of the client's origin.
		if(CMath::Fabs(desiredPos.z - newOrigin.z) < pm_clientAuthoritative_minDistZ.GetFloat())
		{
			if(pm_clientAuthoritative_debug.GetBool())
			{
				//idLib::Printf("[%d]Remote client physics: ignore small z delta: %f\n", usercmd.clientGameFrame, ( desiredPos.z - newOrigin.z ) );
			}
			desiredPos.z = newOrigin.z;
		}

		// Origin
		if(delta > pm_clientAuthoritative_minDist.GetFloat())
		{
			if(pm_clientAuthoritative_Lerp.GetFloat() > 0.0f)
			{
				desiredPos.x = CMath::LerpToWithScale(newOrigin.x, desiredPos.x, pm_clientAuthoritative_Lerp.GetFloat());
				desiredPos.y = CMath::LerpToWithScale(newOrigin.y, desiredPos.y, pm_clientAuthoritative_Lerp.GetFloat());
			}

			// Set corrected position immediately if non deferred
			physicsObj.SetOrigin(desiredPos);

			if(pm_clientAuthoritative_debug.GetBool() && delta > pm_clientAuthoritative_warnDist.GetFloat())
			{
				idLib::Warning("Remote client player physics: delta movement for frame was %f units", delta);
				gameRenderWorld->DebugLine(colorRed, newOrigin, desiredPos);
			}
		}
		if(pm_clientAuthoritative_debug.GetBool())
		{
			//idLib::Printf( "[%d]Remote client player physics delta: %.2f. forward: %d pos <%.2f, %.2f, %.2f> \n", usercmd.clientGameFrame, delta, (int)usercmd.forwardmove, desiredPos.x, desiredPos.y, desiredPos.z );
			gameRenderWorld->DebugLine(colorRed, newOrigin, desiredPos);
			//gameRenderWorld->DebugPoint( colorBlue, cmdPos );
		}

		// Set velocity if significantly different than client.
		const float serverSpeedSquared = physicsObj.GetLinearVelocity().LengthSqr();
		const float clientSpeedSquared = usercmd.speedSquared;

		if(std::abs(serverSpeedSquared - clientSpeedSquared) > pm_clientAuthoritative_minSpeedSquared.GetFloat())
		{
			CVec3 normalizedVelocity = physicsObj.GetLinearVelocity();

			const float VELOCITY_EPSILON = 0.001f;
			if(normalizedVelocity.LengthSqr() > VELOCITY_EPSILON)
			{
				normalizedVelocity.Normalize();
			}

			physicsObj.SetLinearVelocity(normalizedVelocity * CMath::Sqrt(clientSpeedSquared));
		}
	}
}

/*
========================
CBasePlayer::GetPhysicsTimeStep

Uses the time from the usercmd in case the server is running at a slower engineHz
than the client.
========================
*/
int CBasePlayer::GetPhysicsTimeStep() const
{
	// if the ucDeltaMillisecond value looks wrong, use the game delta milliseconds
	// This can happen if the user brings up the pause menu in SP
	const int ucDeltaMilliseconds = usercmd.clientGameMilliseconds - oldCmd.clientGameMilliseconds;
	if(ucDeltaMilliseconds < 1 || ucDeltaMilliseconds > 20)
	{
		return gameLocal.time - gameLocal.previousTime;
	}
	else
	{
		return ucDeltaMilliseconds;
	}
}

/*
==============
CBasePlayer::ShowRespawnHudMessage

Called once when the minimum respawn time has passed after a player has died
so that we can display a message to the user.
==============
*/
void CBasePlayer::ShowRespawnHudMessage()
{
	if(IsLocallyControlled())
	{
		hud->ShowRespawnMessage(true);
	}
	else
	{
		// Clients show the hud message through a reliable message.
		idBitMsg outMsg;
		byte dummyData[1];
		outMsg.InitWrite(dummyData, sizeof(dummyData));
		outMsg.BeginWriting();
		outMsg.WriteByte(0);
		session->GetActingGameStateLobbyBase().SendReliableToLobbyUser(gameLocal.lobbyUserIDs[entityNumber], GAME_RELIABLE_MESSAGE_RESPAWN_AVAILABLE, outMsg);
	}
}

/*
==============
CBasePlayer::HideRespawnHudMessage

Called once when we should remove the respawn message from the hud,
for example, when a player does respawn.
==============
*/
void CBasePlayer::HideRespawnHudMessage()
{
	if(IsLocallyControlled())
	{
		hud->ShowRespawnMessage(false);
	}
}

/*
==============
CBasePlayer::UpdateHud
==============
*/
void CBasePlayer::UpdateHud()
{
	CBasePlayer *aimed;

	if(!hud)
	{
		return;
	}

	if(!IsLocallyControlled())
	{
		return;
	}

	int c = inventory.pickupItemNames.Num();
	if(c > 0)
	{
		if(hud != nullptr && hud->IsPickupListReady())
		{
			if(inventory.nextItemPickup && gameLocal.time - inventory.nextItemPickup > 2000)
			{
				inventory.nextItemNum = 1;
			}
			int i;

			int count = 5;
			bool showNewPickups = false;
			for(i = 0; i < count; i++)
			{ //_D3XP
				if(i < c)
				{
					hud->UpdatePickupInfo(i, inventory.pickupItemNames[0]);
					inventory.nextItemNum++;
					showNewPickups = true;
				}
				else
				{
					hud->UpdatePickupInfo(i, "");
					continue;
				}

				inventory.nextItemPickup = gameLocal.time + 2500;
				inventory.pickupItemNames.RemoveIndex(0);
			}

			if(showNewPickups)
			{
				hud->ShowPickups();
			}
		}
	}

	if(gameLocal.realClientTime == lastMPAimTime)
	{
		if(MPAim != -1 && gameLocal.mpGame.IsGametypeTeamBased() /* CTF */
		   && gameLocal.entities[MPAim] && gameLocal.entities[MPAim]->IsType(CBasePlayer::Type) && static_cast<CBasePlayer *>(gameLocal.entities[MPAim])->team == team)
		{
			aimed = static_cast<CBasePlayer *>(gameLocal.entities[MPAim]);

			hud->TriggerHitTarget(true, session->GetActingGameStateLobbyBase().GetLobbyUserName(gameLocal.lobbyUserIDs[MPAim]), aimed->team + 1);
			MPAimHighlight = true;
			MPAimFadeTime = 0; // no fade till loosing focus
		}
		else if(MPAimHighlight)
		{
			hud->TriggerHitTarget(false, "");
			MPAimFadeTime = gameLocal.realClientTime;
			MPAimHighlight = false;
		}
	}
	if(MPAimFadeTime)
	{
		assert(!MPAimHighlight);
		if(gameLocal.realClientTime - MPAimFadeTime > 2000)
		{
			MPAimFadeTime = 0;
		}
	}

	if(mpGame->GetRules()->IsMultiplayer() && IsLocallyControlled())
	{
		hud->ToggleLagged(isLagged);

		// TODO_SPARTY: what is this projectile stuff for
		//hud->SetStateInt( "g_showProjectilePct", g_showProjectilePct.GetInteger() );
		//if ( numProjectilesFired ) {
		//	hud->SetStateString( "projectilepct", va( "Hit %% %.1f", ( (float) numProjectileHits / numProjectilesFired ) * 100 ) );
		//} else {
		//	hud->SetStateString( "projectilepct", "Hit % 0.0" );
		//}
	}
}

/*
==============
CBasePlayer::UpdateDeathSkin
==============
*/
void CBasePlayer::UpdateDeathSkin(bool state_hitch)
{
	if(!(mpGame->GetRules()->IsMultiplayer() || g_testDeath.GetBool()))
	{
		return;
	}
	if(health <= 0)
	{
		if(!doingDeathSkin)
		{
			deathClearContentsTime = spawnArgs.GetInt("deathSkinTime");
			doingDeathSkin = true;
			renderEntity.noShadow = true;
			if(state_hitch)
			{
				renderEntity.shaderParms[SHADERPARM_TIME_OF_DEATH] = gameLocal.time * 0.001f - 2.0f;
			}
			else
			{
				renderEntity.shaderParms[SHADERPARM_TIME_OF_DEATH] = gameLocal.time * 0.001f;
			}
			UpdateVisuals();
		}

		// wait a bit before switching off the content
		if(deathClearContentsTime && gameLocal.time > deathClearContentsTime)
		{
			SetCombatContents(false);
			deathClearContentsTime = 0;
		}
	}
	else
	{
		renderEntity.noShadow = false;
		renderEntity.shaderParms[SHADERPARM_TIME_OF_DEATH] = 0.0f;
		UpdateVisuals();
		doingDeathSkin = false;
	}
}

/*
==============
CBasePlayer::StartFxOnBone
==============
*/
void CBasePlayer::StartFxOnBone(const char *fx, const char *bone)
{
	CVec3 offset;
	idMat3 axis;
	jointHandle_t jointHandle = GetAnimator()->GetJointHandle(bone);

	if(jointHandle == INVALID_JOINT)
	{
		gameLocal.Printf("Cannot find bone %s\n", bone);
		return;
	}

	if(GetAnimator()->GetJointTransform(jointHandle, gameLocal.time, offset, axis))
	{
		offset = GetPhysics()->GetOrigin() + offset * GetPhysics()->GetAxis();
		axis = axis * GetPhysics()->GetAxis();
	}

	idEntityFx::StartFx(fx, &offset, &axis, this, true);
}

/*
==============
CBasePlayer::HandleGuiEvents
==============
*/
bool CBasePlayer::HandleGuiEvents(const sysEvent_t *ev)
{
	bool handled = false;

	if(hudManager != nullptr && hudManager->IsActive())
	{
		handled = hudManager->HandleGuiEvent(ev);
	}

	if(pdaMenu != nullptr && pdaMenu->IsActive())
	{
		handled = pdaMenu->HandleGuiEvent(ev);
	}

	return handled;
}

/*
==============
CBasePlayer::UpdateLaserSight
==============
*/
idCVar g_laserSightWidth("g_laserSightWidth", "2.0", CVAR_FLOAT | CVAR_ARCHIVE, "laser sight beam width");
idCVar g_laserSightLength("g_laserSightLength", "250", CVAR_FLOAT | CVAR_ARCHIVE, "laser sight beam length");

void CBasePlayer::UpdateLaserSight()
{
	CVec3 muzzleOrigin;
	idMat3 muzzleAxis;

	// In Multiplayer, weapon might not have been spawned yet.
	if(weapon.GetEntity() == nullptr)
	{
		return;
	}

	if(!IsGameStereoRendered() ||
	   !weapon.GetEntity()->ShowCrosshair() ||
	   AI_DEAD ||
	   weapon->IsHidden() ||
	   !weapon->GetMuzzlePositionWithHacks(muzzleOrigin, muzzleAxis))
	{
		// hide it
		laserSightRenderEntity.allowSurfaceInViewID = -1;
		if(laserSightHandle == -1)
		{
			laserSightHandle = gameRenderWorld->AddEntityDef(&laserSightRenderEntity);
		}
		else
		{
			gameRenderWorld->UpdateEntityDef(laserSightHandle, &laserSightRenderEntity);
		}
		return;
	}

	// program the beam model

	// only show in the player's view
	laserSightRenderEntity.allowSurfaceInViewID = entityNumber + 1;
	laserSightRenderEntity.axis.Identity();

	laserSightRenderEntity.origin = muzzleOrigin - muzzleAxis[0] * 2.0f;
	CVec3 &target = *reinterpret_cast<CVec3 *>(&laserSightRenderEntity.shaderParms[SHADERPARM_BEAM_END_X]);
	target = muzzleOrigin + muzzleAxis[0] * g_laserSightLength.GetFloat();

	laserSightRenderEntity.shaderParms[SHADERPARM_BEAM_WIDTH] = g_laserSightWidth.GetFloat();

	if(IsGameStereoRendered() && laserSightHandle == -1)
	{
		laserSightHandle = gameRenderWorld->AddEntityDef(&laserSightRenderEntity);
	}
	else
	{
		gameRenderWorld->UpdateEntityDef(laserSightHandle, &laserSightRenderEntity);
	}
}



/*
=================
CBasePlayer::GetCurrentWeapon
=================
*/
idStr CBasePlayer::GetCurrentWeapon()
{
	const char *weapon;

	if(currentWeapon >= 0)
	{
		weapon = spawnArgs.GetString(va("def_weapon%d", currentWeapon));
		return weapon;
	}
	else
	{
		return "";
	}
}

/*
=================
CBasePlayer::CanGive
=================
*/
bool CBasePlayer::CanGive(const char *statname, const char *value)
{
	if(AI_DEAD)
	{
		return false;
	}

	if(!idStr::Icmp(statname, "health"))
	{
		if(health >= inventory.maxHealth)
		{
			return false;
		}
		return true;
	}
	else if(!idStr::Icmp(statname, "stamina"))
	{
		if(stamina >= 100)
		{
			return false;
		}
		return true;
	}
	else if(!idStr::Icmp(statname, "heartRate"))
	{
		return true;
	}
	else if(!idStr::Icmp(statname, "air"))
	{
		if(airMsec >= pm_airMsec.GetInteger())
		{
			return false;
		}
		return true;
	}
	else
	{
		return inventory.CanGive(this, spawnArgs, statname, value);
	}
}

/*
=================
CBasePlayer::StopHelltime

provides a quick non-ramping way of stopping helltime
=================
*/
void CBasePlayer::StopHelltime(bool quick)
{
	if(!PowerUpActive(HELLTIME))
	{
		return;
	}

	// take away the powerups
	if(PowerUpActive(INVULNERABILITY))
	{
		ClearPowerup(INVULNERABILITY);
	}

	if(PowerUpActive(BERSERK))
	{
		ClearPowerup(BERSERK);
	}

	// stop the looping sound
	StopSound(SND_CHANNEL_DEMONIC, false);

	// reset the game vars
	if(quick)
	{
		gameLocal.QuickSlowmoReset();
	}
}

/*
=================
CBasePlayer::Event_ToggleBloom
=================
*/
void CBasePlayer::Event_ToggleBloom(int on)
{
	if(on)
	{
		bloomEnabled = true;
	}
	else
	{
		bloomEnabled = false;
	}
}

/*
=================
CBasePlayer::Event_SetBloomParms
=================
*/
void CBasePlayer::Event_SetBloomParms(float speed, float intensity)
{
	bloomSpeed = speed;
	bloomIntensity = intensity;
}

/*
=================
CBasePlayer::RouteGuiMouse
=================
*/
void CBasePlayer::RouteGuiMouse(idUserInterface *gui)
{
	sysEvent_t ev;
	const char *command;

	if(usercmd.mx != oldMouseX || usercmd.my != oldMouseY)
	{
		ev = sys->GenerateMouseMoveEvent(usercmd.mx - oldMouseX, usercmd.my - oldMouseY);
		command = gui->HandleEvent(&ev, gameLocal.time);
		oldMouseX = usercmd.mx;
		oldMouseY = usercmd.my;
	}
}

/*
==============
CBasePlayer::Kill
==============
*/
void CBasePlayer::Kill(bool delayRespawn, bool nodamage)
{
	if(spectating)
	{
		SpectateFreeFly(false);
	}
	else if(health > 0)
	{
		godmode = false;
		if(nodamage)
		{
			ServerSpectate(true);
			idLib::Printf("TOURNEY Kill :> Player %d On Deck \n", entityNumber);
			forceRespawn = true;
		}
		else
		{
			Damage(this, this, vec3_origin, "damage_suicide", 1.0f, INVALID_JOINT);
			if(delayRespawn)
			{
				forceRespawn = false;
				int delay = spawnArgs.GetFloat("respawn_delay");
				minRespawnTime = gameLocal.time + SEC2MS(delay);
				maxRespawnTime = minRespawnTime + MAX_RESPAWN_TIME;
			}
		}
	}
}

/*
=====================
CBasePlayer::GetAIAimTargets

Returns positions for the AI to aim at.
=====================
*/
void CBasePlayer::GetAIAimTargets(const CVec3 &lastSightPos, CVec3 &headPos, CVec3 &chestPos)
{
	CVec3 offset;
	idMat3 axis;
	CVec3 origin;

	origin = lastSightPos - physicsObj.GetOrigin();

	GetJointWorldTransform(chestJoint, gameLocal.time, offset, axis);
	headPos = offset + origin;

	GetJointWorldTransform(headJoint, gameLocal.time, offset, axis);
	chestPos = offset + origin;
}

/*
================
CBasePlayer::DamageFeedback

callback function for when another entity received damage from this entity.  damage can be adjusted and returned to the caller.
================
*/
void CBasePlayer::DamageFeedback(CBaseEntity *victim, CBaseEntity *inflictor, int &damage)
{
	// Since we're predicting projectiles on the client now, we might actually get here
	// (used be an assert for clients).
	if constexpr (IsClient())
		return;

	damage *= PowerUpModifier(BERSERK);
	if(damage && (victim != this) && (victim->IsType(CBaseActor::Type) || victim->IsType(idDamagable::Type)))
	{
		CBasePlayer *victimPlayer = nullptr;

		/* No damage feedback sound for hitting friendlies in CTF */
		if(victim->IsType(CBasePlayer::Type))
		{
			victimPlayer = static_cast<CBasePlayer *>(victim);
		}

		if(gameLocal.mpGame.IsGametypeFlagBased() && victimPlayer && this->team == victimPlayer->team)
		{
			/* Do nothing ... */
		}
		else
		{
			SetLastHitTime(gameLocal.time);
		}
	}
}

/*
=================
CBasePlayer::CalcDamagePoints

Calculates how many health and armor points will be inflicted, but
doesn't actually do anything with them.  This is used to tell when an attack
would have killed the player, possibly allowing a "saving throw"
=================
*/
void CBasePlayer::CalcDamagePoints(CBaseEntity *inflictor, CBaseEntity *attacker, const idDict *damageDef,
                                const float damageScale, const int location, int *health, int *armor)
{
	int damage;
	int armorSave;

	damageDef->GetInt("damage", "20", damage);
	damage = GetDamageForLocation(damage, location);

	CBasePlayer *player = attacker->IsType(CBasePlayer::Type) ? static_cast<CBasePlayer *>(attacker) : nullptr;
	if(!mpGame->GetRules()->IsMultiplayer())
	{
		if(inflictor != gameLocal.world)
		{
			switch(g_skill.GetInteger())
			{
			case 0:
				damage *= 0.50f;
				if(damage < 1)
				{
					damage = 1;
				}
				break;
			case 2:
				damage *= 1.70f;
				break;
			case 3:
				damage *= 3.5f;
				break;
			default:
				break;
			}
		}
	}

	damage *= damageScale;

	// always give half damage if hurting self
	if(attacker == this)
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			// only do this in mp so single player plasma and rocket splash is very dangerous in close quarters
			damage *= damageDef->GetFloat("selfDamageScale", "0.5");
		}
		else
		{
			damage *= damageDef->GetFloat("selfDamageScale", "1");
		}
	}

	// check for completely getting out of the damage
	if(!damageDef->GetBool("noGod"))
	{
		// check for godmode
		if(godmode)
		{
			damage = 0;
		}
		//Invulnerability is just like god mode
		if(PowerUpActive(INVULNERABILITY))
		{
			damage = 0;
		}
	}

	// inform the attacker that they hit someone
	attacker->DamageFeedback(this, inflictor, damage);

	// save some from armor
	if(!damageDef->GetBool("noArmor"))
	{
		float armor_protection;

		armor_protection = (mpGame->GetRules()->IsMultiplayer()) ? g_armorProtectionMP.GetFloat() : g_armorProtection.GetFloat();

		armorSave = ceil(damage * armor_protection);
		if(armorSave >= inventory.armor)
		{
			armorSave = inventory.armor;
		}

		if(!damage)
		{
			armorSave = 0;
		}
		else if(armorSave >= damage)
		{
			armorSave = damage - 1;
			damage = 1;
		}
		else
		{
			damage -= armorSave;
		}
	}
	else
	{
		armorSave = 0;
	}

	// check for team damage
	if(gameLocal.mpGame.IsGametypeTeamBased()                                                                         /* CTF */
	   && !gameLocal.serverInfo.GetBool("si_teamDamage") && !damageDef->GetBool("noTeam") && player && player != this // you get self damage no matter what
	   && player->team == team)
	{
		damage = 0;
	}

	*health = damage;
	*armor = armorSave;
}

/*
============
CBasePlayer::ControllerShakeFromDamage
============
*/
void CBasePlayer::ControllerShakeFromDamage(int damage)
{
	// If the player is local. SHAkkkkkkeeee!
	if(mpGame->GetRules()->IsMultiplayer() && IsLocallyControlled())
	{
		int maxMagScale = pm_controllerShake_damageMaxMag.GetFloat();
		int maxDurScale = pm_controllerShake_damageMaxDur.GetFloat();

		// determine rumble
		// >= 100 damage - will be 300 Mag
		float highMag = (Max(damage, 100) / 100.0f) * maxMagScale;
		int highDuration = CMath::Ftoi((Max(damage, 100) / 100.0f) * maxDurScale);
		float lowMag = highMag * 0.75f;
		int lowDuration = CMath::Ftoi(highDuration);

		SetControllerShake(highMag, highDuration, lowMag, lowDuration);
	}
}

/*
============
AdjustDamageAmount

Modifies the previously calculated damage to adjust for more factors.
============
*/
int CBasePlayer::AdjustDamageAmount(const int inputDamage)
{
	int outputDamage = inputDamage;

	if(inputDamage > 0)
	{
		if(!mpGame->GetRules()->IsMultiplayer())
		{
			float scale = new_g_damageScale;
			if(g_useDynamicProtection.GetBool() && g_skill.GetInteger() < 2)
			{
				if(gameLocal.time > lastDmgTime + 500 && scale > 0.25f)
				{
					scale -= 0.05f;
					new_g_damageScale = scale;
				}
			}

			if(scale > 0.0f)
			{
				outputDamage *= scale;
			}
		}

		if(g_demoMode.GetBool())
		{
			outputDamage /= 2;
		}

		if(outputDamage < 1)
		{
			outputDamage = 1;
		}
	}

	return outputDamage;
}

/*
===========
CBasePlayer::Teleport
============
*/
void CBasePlayer::Teleport(const CVec3 &origin, const idAngles &angles, CBaseEntity *destination)
{
	CVec3 org;

	if(weapon.GetEntity())
		weapon.GetEntity()->LowerWeapon();

	SetOrigin(origin + CVec3(0, 0, CM_CLIP_EPSILON));
	
	if(!mpGame->GetRules()->IsMultiplayer() && GetFloorPos(16.0f, org))
		SetOrigin(org);

	// clear the ik heights so model doesn't appear in the wrong place
	walkIK.EnableAll();

	GetPhysics()->SetLinearVelocity(vec3_origin);

	SetViewAngles(angles);

	legsYaw = 0.0f;
	idealLegsYaw = 0.0f;
	oldViewYaw = viewAngles.yaw;

	if(mpGame->GetRules()->IsMultiplayer())
		playerView.Flash(colorWhite, 140);

	UpdateVisuals();

	teleportEntity = destination;

	if(!common->IsClient() && !noclip)
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			// kill anything at the new position or mark for kill depending on immediate or delayed teleport
			gameLocal.KillBox(this, destination != nullptr);
		}
		else
		{
			// kill anything at the new position
			gameLocal.KillBox(this, true);
		}
	}
}

/*
====================
CBasePlayer::DefaultFov

Returns the base FOV
====================
*/
float CBasePlayer::DefaultFov() const
{
	float fov;

	fov = g_fov.GetFloat();
	if(mpGame->GetRules()->IsMultiplayer())
	{
		if(fov < 80.0f)
		{
			return 80.0f;
		}
		else if(fov > 120.0f)
		{
			return 120.0f;
		}
	}

	return fov;
}

/*
====================
CBasePlayer::CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
float CBasePlayer::CalcFov(bool honorZoom)
{
	float fov;

	if(fxFov)
	{
		return DefaultFov() + 10.0f + cos((gameLocal.time + 2000) * 0.01) * 10.0f;
	}

	if(influenceFov)
	{
		return influenceFov;
	}

	if(zoomFov.IsDone(gameLocal.time))
	{
		fov = (honorZoom && usercmd.buttons & BUTTON_ZOOM) && weapon.GetEntity() ? weapon.GetEntity()->GetZoomFov() : DefaultFov();
	}
	else
	{
		fov = zoomFov.GetCurrentValue(gameLocal.time);
	}

	// bound normal viewsize
	if(fov < 1)
	{
		fov = 1;
	}
	else if(fov > 179)
	{
		fov = 179;
	}

	return fov;
}

/*
==============
CBasePlayer::GunTurningOffset

generate a rotational offset for the gun based on the view angle
history in loggedViewAngles
==============
*/
idAngles CBasePlayer::GunTurningOffset()
{
	idAngles a;

	a.Zero();

	if(gameLocal.framenum < NUM_LOGGED_VIEW_ANGLES)
	{
		return a;
	}

	idAngles current = loggedViewAngles[gameLocal.framenum & (NUM_LOGGED_VIEW_ANGLES - 1)];

	idAngles av, base;
	int weaponAngleOffsetAverages;
	float weaponAngleOffsetScale, weaponAngleOffsetMax;

	weapon.GetEntity()->GetWeaponAngleOffsets(&weaponAngleOffsetAverages, &weaponAngleOffsetScale, &weaponAngleOffsetMax);

	av = current;

	// calcualte this so the wrap arounds work properly
	for(int j = 1; j < weaponAngleOffsetAverages; j++)
	{
		idAngles a2 = loggedViewAngles[(gameLocal.framenum - j) & (NUM_LOGGED_VIEW_ANGLES - 1)];

		idAngles delta = a2 - current;

		if(delta[1] > 180)
		{
			delta[1] -= 360;
		}
		else if(delta[1] < -180)
		{
			delta[1] += 360;
		}

		av += delta * (1.0f / weaponAngleOffsetAverages);
	}

	a = (av - current) * weaponAngleOffsetScale;

	for(int i = 0; i < 3; i++)
	{
		if(a[i] < -weaponAngleOffsetMax)
		{
			a[i] = -weaponAngleOffsetMax;
		}
		else if(a[i] > weaponAngleOffsetMax)
		{
			a[i] = weaponAngleOffsetMax;
		}
	}

	return a;
}

/*
==============
CBasePlayer::GunAcceleratingOffset

generate a positional offset for the gun based on the movement
history in loggedAccelerations
==============
*/
CVec3 CBasePlayer::GunAcceleratingOffset()
{
	CVec3 ofs;

	float weaponOffsetTime, weaponOffsetScale;

	ofs.Zero();

	weapon.GetEntity()->GetWeaponTimeOffsets(&weaponOffsetTime, &weaponOffsetScale);

	int stop = currentLoggedAccel - NUM_LOGGED_ACCELS;
	if(stop < 0)
	{
		stop = 0;
	}
	for(int i = currentLoggedAccel - 1; i > stop; i--)
	{
		loggedAccel_t *acc = &loggedAccel[i & (NUM_LOGGED_ACCELS - 1)];

		float f;
		float t = gameLocal.time - acc->time;
		if(t >= weaponOffsetTime)
		{
			break; // remainder are too old to care about
		}

		f = t / weaponOffsetTime;
		f = (cos(f * 2.0f * CMath::PI) - 1.0f) * 0.5f;
		ofs += f * weaponOffsetScale * acc->dir;
	}

	return ofs;
}

/*
==============
CBasePlayer::CalculateViewWeaponPos

Calculate the bobbing position of the view weapon
==============
*/
void CBasePlayer::CalculateViewWeaponPos(CVec3 &origin, idMat3 &axis)
{
	float scale;
	float fracsin;
	idAngles angles;
	int delta;

	// CalculateRenderView must have been called first
	const CVec3 &viewOrigin = firstPersonViewOrigin;
	const idMat3 &viewAxis = firstPersonViewAxis;

	// these cvars are just for hand tweaking before moving a value to the weapon def
	CVec3 gunpos(g_gun_x.GetFloat(), g_gun_y.GetFloat(), g_gun_z.GetFloat());

	// as the player changes direction, the gun will take a small lag
	CVec3 gunOfs = GunAcceleratingOffset();
	origin = viewOrigin + (gunpos + gunOfs) * viewAxis;

	// on odd legs, invert some angles
	if(bobCycle & 128)
	{
		scale = -xyspeed;
	}
	else
	{
		scale = xyspeed;
	}

	// gun angles from bobbing
	angles.roll = scale * bobfracsin * 0.005f;
	angles.yaw = scale * bobfracsin * 0.01f;
	angles.pitch = xyspeed * bobfracsin * 0.005f;

	// gun angles from turning
	if(mpGame->GetRules()->IsMultiplayer())
	{
		idAngles offset = GunTurningOffset();
		offset *= g_mpWeaponAngleScale.GetFloat();
		angles += offset;
	}
	else
	{
		angles += GunTurningOffset();
	}

	CVec3 gravity = physicsObj.GetGravityNormal();

	// drop the weapon when landing after a jump / fall
	delta = gameLocal.time - landTime;
	if(delta < LAND_DEFLECT_TIME)
	{
		origin -= gravity * (landChange * 0.25f * delta / LAND_DEFLECT_TIME);
	}
	else if(delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME)
	{
		origin -= gravity * (landChange * 0.25f * (LAND_DEFLECT_TIME + LAND_RETURN_TIME - delta) / LAND_RETURN_TIME);
	}

	// speed sensitive idle drift
	scale = xyspeed + 40.0f;
	fracsin = scale * sin(MS2SEC(gameLocal.time)) * 0.01f;
	angles.roll += fracsin;
	angles.yaw += fracsin;
	angles.pitch += fracsin;

	// decoupled weapon aiming in head mounted displays
	angles.pitch += independentWeaponPitchAngle;

	const idMat3 anglesMat = angles.ToMat3();
	const idMat3 scaledMat = anglesMat * g_gunScale.GetFloat();

	axis = scaledMat * viewAxis;
};

/*
===============
CBasePlayer::OffsetThirdPersonView
===============
*/
void CBasePlayer::OffsetThirdPersonView(float angle, float range, float height, bool clip)
{
	CVec3 view;
	CVec3 focusAngles;
	trace_t trace;
	CVec3 focusPoint;
	float focusDist;
	float forwardScale, sideScale;
	CVec3 origin;
	CAngles angles;
	CMat3 axis;
	CBounds bounds;

	angles = viewAngles;
	GetViewPos(origin, axis);

	if(angle)
		angles.pitch = 0.0f;

	if(angles.pitch > 45.0f)
		angles.pitch = 45.0f; // don't go too far overhead

	focusPoint = origin + angles.ToForward() * THIRD_PERSON_FOCUS_DISTANCE;
	focusPoint.z += height;
	view = origin;
	view.z += 8 + height;

	angles.pitch *= 0.5f;
	renderView->viewaxis = angles.ToMat3() * physicsObj.GetGravityAxis();

	CMath::SinCos(DEG2RAD(angle), sideScale, forwardScale);
	view -= range * forwardScale * renderView->viewaxis[0];
	view += range * sideScale * renderView->viewaxis[1];

	if(clip)
	{
		// trace a ray from the origin to the viewpoint to make sure the view isn't
		// in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything
		bounds = idBounds(CVec3(-4, -4, -4), CVec3(4, 4, 4));
		gameLocal.clip.TraceBounds(trace, origin, view, bounds, MASK_SOLID, this);
		if(trace.fraction != 1.0f)
		{
			view = trace.endpos;
			view.z += (1.0f - trace.fraction) * 32.0f;

			// try another trace to this position, because a tunnel may have the ceiling
			// close enough that this is poking out
			gameLocal.clip.TraceBounds(trace, origin, view, bounds, MASK_SOLID, this);
			view = trace.endpos;
		};
	};

	// select pitch to look at focus point from vieword
	focusPoint -= view;
	focusDist = CMath::Sqrt(focusPoint[0] * focusPoint[0] + focusPoint[1] * focusPoint[1]);
	if(focusDist < 1.0f)
		focusDist = 1.0f; // should never happen

	angles.pitch = -RAD2DEG(atan2(focusPoint.z, focusDist));
	angles.yaw -= angle;

	renderView->vieworg = view;
	renderView->viewaxis = angles.ToMat3() * physicsObj.GetGravityAxis();
	renderView->viewID = 0;
};

/*
===============
CBasePlayer::GetEyePosition
===============
*/
CVec3 CBasePlayer::GetEyePosition() const
{
	CVec3 org{GetPhysics()->GetOrigin()};

	// use the smoothed origin if spectating another player in multiplayer
	if constexpr (common->IsClient() && !IsLocallyControlled())
		org = smoothedOrigin;

	return org + (GetPhysics()->GetGravityNormal() * -eyeOffset.z);
};

/*
===============
CBasePlayer::GetViewPos
===============
*/
void CBasePlayer::GetViewPos(CVec3 &origin, CMat3 &axis) const
{
	CAngles angles;

	// if dead, fix the angle and don't add any kick
	if(health <= 0)
	{
		angles.yaw = viewAngles.yaw;
		angles.roll = 40;
		angles.pitch = -15;
		axis = angles.ToMat3();
		origin = GetEyePosition();
	}
	else
	{
		origin = GetEyePosition() + viewBob;
		angles = viewAngles + viewBobAngles + playerView.AngleOffset();

		axis = angles.ToMat3() * physicsObj.GetGravityAxis();

		// Move pivot point down so looking straight ahead is a no-op on the Z
		const CVec3 &gravityVector = physicsObj.GetGravityNormal();
		origin += gravityVector * g_viewNodalZ.GetFloat();

		// adjust the origin based on the camera nodal distance (eye distance from neck)
		origin += axis[0] * g_viewNodalX.GetFloat() + axis[2] * g_viewNodalZ.GetFloat();
	};
};

/*
===============
CBasePlayer::CalculateFirstPersonView
===============
*/
void CBasePlayer::CalculateFirstPersonView()
{
	if((pm_modelView.GetInteger() == 1) || ((pm_modelView.GetInteger() == 2) && (health <= 0)))
	{
		//	Displays the view from the point of view of the "camera" joint in the player model

		CMat3 axis;
		CVec3 origin;
		CAngles ang;

		ang = viewBobAngles + playerView.AngleOffset();
		ang.yaw += viewAxis[0].ToYaw();

		jointHandle_t joint = animator.GetJointHandle("camera");
		animator.GetJointTransform(joint, gameLocal.time, origin, axis);
		firstPersonViewOrigin = (origin + modelOffset) * (viewAxis * physicsObj.GetGravityAxis()) + physicsObj.GetOrigin() + viewBob;
		firstPersonViewAxis = axis * ang.ToMat3() * physicsObj.GetGravityAxis();
	}
	else
	{
		// offset for local bobbing and kicks
		GetViewPos(firstPersonViewOrigin, firstPersonViewAxis);
#if 0
		// shakefrom sound stuff only happens in first person
		firstPersonViewAxis = firstPersonViewAxis * playerView.ShakeAxis();
#endif
	};
};

/*
==================
CBasePlayer::GetRenderView

Returns the renderView that was calculated for this tic
==================
*/
renderView_t *CBasePlayer::GetRenderView()
{
	return renderView;
};

/*
==================
CBasePlayer::CalculateRenderView

create the renderView for the current tic
==================
*/
void CBasePlayer::CalculateRenderView()
{
	int i;
	float range;

	if(!renderView)
		renderView = new(TAG_ENTITY) renderView_t;

	memset(renderView, 0, sizeof(*renderView));

	// copy global shader parms
	for(i = 0; i < MAX_GLOBAL_SHADER_PARMS; i++)
		renderView->shaderParms[i] = gameLocal.globalShaderParms[i];

	renderView->globalMaterial = gameLocal.GetGlobalMaterial();

	renderView->time[0] = gameLocal.slow.time;
	renderView->time[1] = gameLocal.fast.time;

	renderView->viewID = 0;

	// check if we should be drawing from a camera's POV
	if(!noclip && (gameLocal.GetCamera() || privateCameraView))
	{
		// get origin, axis, and fov
		if(privateCameraView)
			privateCameraView->GetViewParms(renderView);
		else
			gameLocal.GetCamera()->GetViewParms(renderView);
	}
	else
	{
		if(g_stopTime.GetBool())
		{
			renderView->vieworg = firstPersonViewOrigin;
			renderView->viewaxis = firstPersonViewAxis;

			if(!pm_thirdPerson.GetBool())
			{
				// set the viewID to the clientNum + 1, so we can suppress the right player bodies and
				// allow the right player view weapons
				renderView->viewID = entityNumber + 1;
			};
		}
		else if(pm_thirdPerson.GetBool())
		{
			OffsetThirdPersonView(pm_thirdPersonAngle.GetFloat(), pm_thirdPersonRange.GetFloat(), pm_thirdPersonHeight.GetFloat(), pm_thirdPersonClip.GetBool());
		}
		else if(pm_thirdPersonDeath.GetBool())
		{
			range = gameLocal.time < minRespawnTime ? (gameLocal.time + RAGDOLL_DEATH_TIME - minRespawnTime) * (120.0f / RAGDOLL_DEATH_TIME) : 120.0f;
			OffsetThirdPersonView(0.0f, 20.0f + range, 0.0f, false);
		}
		else
		{
			renderView->vieworg = firstPersonViewOrigin;
			renderView->viewaxis = firstPersonViewAxis;

			// set the viewID to the clientNum + 1, so we can suppress the right player bodies and
			// allow the right player view weapons
			renderView->viewID = entityNumber + 1;
		};

		gameLocal.CalcFov(CalcFov(true), renderView->fov_x, renderView->fov_y);
	};

	if(renderView->fov_y == 0)
		gpSystem->Error("renderView->fov_y == 0");

	if(g_showviewpos.GetBool())
		gpSystem->Printf("%s : %s\n", renderView->vieworg.ToString(), renderView->viewaxis.ToAngles().ToString());
};

/*
=============
CBasePlayer::AddProjectilesFired
=============
*/
void CBasePlayer::AddProjectilesFired(int count)
{
	numProjectilesFired += count;
};

/*
=============
CBasePlayer::AddProjectileHites
=============
*/
void CBasePlayer::AddProjectileHits(int count)
{
	numProjectileHits += count;
};

/*
=============
CBasePlayer::SetLastHitTime
=============
*/
void CBasePlayer::SetLastHitTime(int time)
{
	CBasePlayer *aimed = nullptr;

	if(time && lastHitTime != time)
		lastHitToggle ^= 1;

	lastHitTime = time;
	
	if(!time)
	{
		// level start and inits
		return;
	};
	
	if(mpGame->GetRules()->IsMultiplayer() && (time - lastSndHitTime) > 10)
	{
		lastSndHitTime = time;
		StartSound("snd_hit_feedback", SND_CHANNEL_ANY, SSF_PRIVATE_SOUND, false, nullptr);
	};

	if(hud)
		hud->CombatCursorFlash();

	if(MPAim != -1)
	{
		if(gameLocal.entities[MPAim] && gameLocal.entities[MPAim]->IsType(CBasePlayer::Type))
			aimed = static_cast<CBasePlayer *>(gameLocal.entities[MPAim]);
		
		assert(aimed);
		
		// full highlight, no fade till loosing aim
		if(hud)
		{
			int color = 0;
			
			if(aimed)
				color = aimed->team + 1;

			hud->TriggerHitTarget(true, session->GetActingGameStateLobbyBase().GetLobbyUserName(gameLocal.lobbyUserIDs[MPAim]), color);
		};
		
		MPAimHighlight = true;
		MPAimFadeTime = 0;
	}
	else if(lastMPAim != -1)
	{
		if(gameLocal.entities[lastMPAim] && gameLocal.entities[lastMPAim]->IsType(CBasePlayer::Type))
			aimed = static_cast<CBasePlayer *>(gameLocal.entities[lastMPAim]);
		
		assert(aimed);
		
		// start fading right away
		if(hud)
		{
			int color = 0;
			
			if(aimed)
				color = aimed->team + 1;

			hud->TriggerHitTarget(true, session->GetActingGameStateLobbyBase().GetLobbyUserName(gameLocal.lobbyUserIDs[lastMPAim]), color);
			hud->TriggerHitTarget(false, "");
		};
		
		MPAimHighlight = false;
		MPAimFadeTime = gameLocal.realClientTime;
	};
};

/*
=============
CBasePlayer::SetInfluenceLevel
=============
*/
void CBasePlayer::SetInfluenceLevel(int level)
{
	if(level != influenceActive)
	{
		if(level)
		{
			for(CBaseEntity *ent = gameLocal.spawnedEntities.Next(); ent != nullptr; ent = ent->spawnNode.Next())
			{
				if(ent->IsType(idProjectile::Type))
				{
					// remove all projectiles
					ent->PostEventMS(&EV_Remove, 0);
				};
			};
			
			if(weaponEnabled && weapon.GetEntity())
				weapon.GetEntity()->EnterCinematic();
		}
		else
		{
			physicsObj.SetLinearVelocity(vec3_origin);
			if(weaponEnabled && weapon.GetEntity())
				weapon.GetEntity()->ExitCinematic();
		};
		influenceActive = level;
	};
};

/*
=============
CBasePlayer::SetInfluenceView
=============
*/
void CBasePlayer::SetInfluenceView(const char *mtr, const char *skinname, float radius, CBaseEntity *ent)
{
	influenceMaterial = nullptr;
	influenceEntity = nullptr;
	influenceSkin = nullptr;
	
	if(mtr && *mtr)
		influenceMaterial = declManager->FindMaterial(mtr);

	if(skinname && *skinname)
	{
		influenceSkin = declManager->FindSkin(skinname);
		
		if(head.GetEntity())
			head.GetEntity()->GetRenderEntity()->shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC(gameLocal.time);
		
		UpdateVisuals();
	};
	
	influenceRadius = radius;
	
	if(radius > 0.0f)
		influenceEntity = ent;
};

/*
=============
CBasePlayer::SetInfluenceFov
=============
*/
void CBasePlayer::SetInfluenceFov(float fov)
{
	influenceFov = fov;
};

/*
================
CBasePlayer::OnLadder
================
*/
bool CBasePlayer::OnLadder() const
{
	return physicsObj.OnLadder();
};

/*
==================
CBasePlayer::Event_GetButtons
==================
*/
void CBasePlayer::Event_GetButtons()
{
	idThread::ReturnInt(usercmd.buttons);
};

/*
==================
CBasePlayer::Event_GetMove
==================
*/
void CBasePlayer::Event_GetMove()
{
	int upmove = ((usercmd.buttons & BUTTON_JUMP) ? 127 : 0) - ((usercmd.buttons & BUTTON_CROUCH) ? 127 : 0);
	CVec3 move(usercmd.forwardmove, usercmd.rightmove, upmove);
	CThread::ReturnVector(move);
};

/*
================
CBasePlayer::Event_GetViewAngles
================
*/
void CBasePlayer::Event_GetViewAngles()
{
	idThread::ReturnVector(CVec3(viewAngles[0], viewAngles[1], viewAngles[2]));
};

/*
==================
CBasePlayer::Event_StopFxFov
==================
*/
void CBasePlayer::Event_StopFxFov()
{
	fxFov = false;
};

/*
==================
CBasePlayer::StartFxFov 
==================
*/
void CBasePlayer::StartFxFov(float duration)
{
	fxFov = true;
	PostEventSec(&EV_Player_StopFxFov, duration);
};

/*
==================
CBasePlayer::Event_EnableWeapon 
==================
*/
void CBasePlayer::Event_EnableWeapon()
{
	hiddenWeapon = mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons");
	weaponEnabled = true;
	if(weapon.GetEntity())
		weapon.GetEntity()->ExitCinematic();
};

/*
==================
CBasePlayer::Event_DisableWeapon
==================
*/
void CBasePlayer::Event_DisableWeapon()
{
	hiddenWeapon = mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons");
	weaponEnabled = false;
	if(weapon.GetEntity())
		weapon.GetEntity()->EnterCinematic();
};

/*
==================
CBasePlayer::Event_GiveInventoryItem 
==================
*/
void CBasePlayer::Event_GiveInventoryItem(const char *name)
{
	GiveInventoryItem(name);
};

/*
==================
CBasePlayer::Event_RemoveInventoryItem 
==================
*/
void CBasePlayer::Event_RemoveInventoryItem(const char *name)
{
	RemoveInventoryItem(name);
};

/*
==================
CBasePlayer::Event_GetIdealWeapon 
==================
*/
void CBasePlayer::Event_GetIdealWeapon()
{
	if(idealWeapon.Get() >= 0)
	{
		const char *weapon = spawnArgs.GetString(va("def_weapon%d", idealWeapon.Get()));
		idThread::ReturnString(weapon);
	}
	else
		idThread::ReturnString("");
};

/*
==================
CBasePlayer::Event_SetPowerupTime 
==================
*/
void CBasePlayer::Event_SetPowerupTime(int powerup, int time)
{
	if(time > 0)
		GivePowerUp(powerup, time, ITEM_GIVE_FEEDBACK | ITEM_GIVE_UPDATE_STATE);
	else
		ClearPowerup(powerup);
};

/*
==================
CBasePlayer::Event_IsPowerupActive 
==================
*/
void CBasePlayer::Event_IsPowerupActive(int powerup)
{
	idThread::ReturnInt(this->PowerUpActive(powerup) ? 1 : 0);
};

/*
==================
CBasePlayer::Event_StartWarp
==================
*/
void CBasePlayer::Event_StartWarp()
{
	playerView.AddWarp(CVec3(0, 0, 0), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, 1000);
};

/*
==================
CBasePlayer::Event_WeaponAvailable
==================
*/
void CBasePlayer::Event_WeaponAvailable(const char *name)
{
	idThread::ReturnInt(WeaponAvailable(name) ? 1 : 0);
};

bool CBasePlayer::WeaponAvailable(const char *name)
{
	for(int i = 0; i < MAX_WEAPONS; i++)
	{
		if(inventory.weapons & (1 << i))
		{
			const char *weap = spawnArgs.GetString(va("def_weapon%d", i));
			if(!CStr::Cmp(weap, name))
				return true;
		};
	};
	return false;
};

/*
==================
CBasePlayer::Event_GetCurrentWeapon
==================
*/
void CBasePlayer::Event_GetCurrentWeapon()
{
	if(currentWeapon >= 0)
	{
		const char *weapon = spawnArgs.GetString(va("def_weapon%d", currentWeapon));
		idThread::ReturnString(weapon);
	}
	else
		idThread::ReturnString("");
};

/*
==================
CBasePlayer::Event_GetPreviousWeapon
==================
*/
void CBasePlayer::Event_GetPreviousWeapon()
{
	if(previousWeapon >= 0)
	{
		int pw = (mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons")) ? 0 : previousWeapon;
		const char *weapon = spawnArgs.GetString(va("def_weapon%d", pw));
		idThread::ReturnString(weapon);
	}
	else
		idThread::ReturnString(spawnArgs.GetString("def_weapon0"));
};

/*
==================
CBasePlayer::Event_SelectWeapon
==================
*/
void CBasePlayer::Event_SelectWeapon(const char *weaponName)
{
	int i;
	int weaponNum;

	if constexpr (IsClient())
	{
		gpSystem->Warning("Cannot switch weapons from script in multiplayer");
		return;
	};

	if(hiddenWeapon && mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons"))
	{
		idealWeapon = weapon_fists;
		weapon.GetEntity()->HideWeapon();
		return;
	};

	weaponNum = -1;
	for(i = 0; i < MAX_WEAPONS; i++)
	{
		if(inventory.weapons & (1 << i))
		{
			const char *weap = spawnArgs.GetString(va("def_weapon%d", i));
			if(!CStr::Cmp(weap, weaponName))
			{
				weaponNum = i;
				break;
			};
		};
	};

	if(weaponNum < 0)
	{
		gpSystem->Warning("%s is not carrying weapon '%s'", name.c_str(), weaponName);
		return;
	};

	hiddenWeapon = false;
	idealWeapon = weaponNum;

	UpdateHudWeapon();
};

/*
==================
CBasePlayer::Event_GetWeaponEntity
==================
*/
void CBasePlayer::Event_GetWeaponEntity()
{
	idThread::ReturnEntity(weapon.GetEntity());
};

/*
==================
CBasePlayer::Event_OpenPDA
==================
*/
void CBasePlayer::Event_OpenPDA()
{
	if(!mpGame->GetRules()->IsMultiplayer())
		TogglePDA();
};

/*
==================
CBasePlayer::Event_InPDA
==================
*/
void CBasePlayer::Event_InPDA()
{
	idThread::ReturnInt(objectiveSystemOpen);
};

/*
==================
CBasePlayer::TeleportDeath
==================
*/
void CBasePlayer::TeleportDeath(int killer)
{
	teleportKiller = killer;
};

/*
================
CBasePlayer::GetPhysicsToVisualTransform
================
*/
bool CBasePlayer::GetPhysicsToVisualTransform(CVec3 &origin, CMat3 &axis)
{
	if(af.IsActive())
	{
		af.GetPhysicsToVisualTransform(origin, axis);
		return true;
	};

	// smoothen the rendered origin and angles of other clients
	// smooth self origin if snapshots are telling us prediction is off
	if(common->IsClient() && gameLocal.framenum >= smoothedFrame && (!IsLocallyControlled() || selfSmooth))
	{
		// render origin and axis
		CMat3 renderAxis = viewAxis * GetPhysics()->GetAxis();
		CVec3 renderOrigin = GetPhysics()->GetOrigin() + modelOffset * renderAxis;

		// update the smoothed origin
		if(!smoothedOriginUpdated)
		{
			CVec2 originDiff = renderOrigin.ToVec2() - smoothedOrigin.ToVec2();
			if(originDiff.LengthSqr() < Square(100.0f))
			{
				// smoothen by pushing back to the previous position
				if(selfSmooth)
				{
					assert(IsLocallyControlled());
					renderOrigin.ToVec2() -= net_clientSelfSmoothing.GetFloat() * originDiff;
				}
				else
					renderOrigin.ToVec2() -= gameLocal.clientSmoothing * originDiff;
			};
			smoothedOrigin = renderOrigin;

			smoothedFrame = gameLocal.framenum;
			smoothedOriginUpdated = true;
		};

		axis = CAngles(0.0f, viewAngles.yaw, 0.0f).ToMat3();
		origin = (smoothedOrigin - GetPhysics()->GetOrigin()) * axis.Transpose();
	}
	else
	{
		axis = viewAxis;
		origin = modelOffset;
	};
	
	return true;
};

/*
================
CBasePlayer::GetPhysicsToSoundTransform
================
*/
bool CBasePlayer::GetPhysicsToSoundTransform(CVec3 &origin, CMat3 &axis)
{
	CCamera *camera{mpGame->GetCamera()};

	if(privateCameraView)
		camera = privateCameraView;

	if(camera)
	{
		renderView_t view;

		memset(&view, 0, sizeof(view));
		camera->GetViewParms(&view);
		origin = view.vieworg;
		axis = view.viewaxis;
		return true;
	}
	else
		return CBaseActor::GetPhysicsToSoundTransform(origin, axis);
};

/*
================
CBasePlayer::HandleUserCmds
================
*/
void CBasePlayer::HandleUserCmds(const usercmd_t &newcmd)
{
	// latch button actions
	oldButtons = usercmd.buttons;

	// grab out usercmd
	oldCmd = usercmd;
	oldImpulseSequence = usercmd.impulseSequence;
	usercmd = newcmd;
};

/*
================
CBasePlayer::WriteToSnapshot
================
*/
void CBasePlayer::WriteToSnapshot(CBitMsg &msg) const
{
	physicsObj.WriteToSnapshot(msg);
	WriteBindToSnapshot(msg);
	
	// Only remote players will use these actual viewangles.
	idCQuat snapViewCQuat(viewAngles.ToQuat().ToCQuat());
	msg.WriteFloat(snapViewCQuat.x);
	msg.WriteFloat(snapViewCQuat.y);
	msg.WriteFloat(snapViewCQuat.z);
	msg.WriteDeltaFloat(0.0f, deltaViewAngles[0]);
	msg.WriteDeltaFloat(0.0f, deltaViewAngles[1]);
	msg.WriteDeltaFloat(0.0f, deltaViewAngles[2]);
	msg.WriteShort(health);
	msg.WriteBits(gameLocal.ServerRemapDecl(-1, DECL_ENTITYDEF, lastDamageDef), gameLocal.entityDefBits);
	msg.WriteDir(lastDamageDir, 9);
	msg.WriteShort(lastDamageLocation);
	msg.WriteBits(idealWeapon.Get(), CMath::BitsForInteger(MAX_WEAPONS));
	msg.WriteBits(inventory.weapons, MAX_WEAPONS);
	msg.WriteBits(weapon.GetSpawnId(), 32);
	msg.WriteBits(flashlight.GetSpawnId(), 32);
	msg.WriteBits(spectator, CMath::BitsForInteger(MAX_CLIENTS));
	msg.WriteBits(lastHitToggle, 1);
	msg.WriteBits(weaponGone, 1);
	msg.WriteBits(isLagged, 1);
	msg.WriteShort(team);
	WriteToBitMsg(respawn_netEvent, msg);

	/* Needed for the scoreboard */
	msg.WriteBits(carryingFlag, 1);
	msg.WriteBits(enviroSuitLight.GetSpawnId(), 32);

	msg.WriteBits(AI_CROUCH, 1);
	msg.WriteBits(AI_ONGROUND, 1);
	msg.WriteBits(AI_ONLADDER, 1);
	msg.WriteBits(AI_JUMP, 1);
	msg.WriteBits(AI_WEAPON_FIRED, 1);
	msg.WriteBits(AI_ATTACK_HELD, 1);

	msg.WriteByte(usercmd.buttons);
	msg.WriteBits(usercmd.forwardmove, -8);
	msg.WriteBits(usercmd.rightmove, -8);

	msg.WriteBool(spectating);
};

/*
================
CBasePlayer::ReadFromSnapshot
================
*/
void CBasePlayer::ReadFromSnapshot(const CBitMsg &msg)
{
	int oldHealth, newIdealWeapon, weaponSpawnId;
	int flashlightSpawnId;
	bool newHitToggle;

	oldHealth = health;

	physicsObj.ReadFromSnapshot(msg);
	ReadBindFromSnapshot(msg);

	// The remote players get updated view angles from the snapshot.
	idCQuat snapViewCQuat;
	snapViewCQuat.x = msg.ReadFloat();
	snapViewCQuat.y = msg.ReadFloat();
	snapViewCQuat.z = msg.ReadFloat();

	idAngles tempDeltaViewAngles;
	tempDeltaViewAngles[0] = msg.ReadDeltaFloat(0.0f);
	tempDeltaViewAngles[1] = msg.ReadDeltaFloat(0.0f);
	tempDeltaViewAngles[2] = msg.ReadDeltaFloat(0.0f);

	deltaViewAngles = tempDeltaViewAngles;

	health = msg.ReadShort();
	lastDamageDef = gameLocal.ClientRemapDecl(DECL_ENTITYDEF, msg.ReadBits(gameLocal.entityDefBits));
	lastDamageDir = msg.ReadDir(9);
	lastDamageLocation = msg.ReadShort();
	newIdealWeapon = msg.ReadBits(CMath::BitsForInteger(MAX_WEAPONS));
	inventory.weapons = msg.ReadBits(MAX_WEAPONS);
	weaponSpawnId = msg.ReadBits(32);
	flashlightSpawnId = msg.ReadBits(32);
	spectator = msg.ReadBits(CMath::BitsForInteger(MAX_CLIENTS));
	newHitToggle = msg.ReadBits(1) != 0;
	weaponGone = msg.ReadBits(1) != 0;
	isLagged = msg.ReadBits(1) != 0;
	team = msg.ReadShort();
	ReadFromBitMsg(respawn_netEvent, msg);

	carryingFlag = msg.ReadBits(1) != 0;
	int enviroSpawnId;
	enviroSpawnId = msg.ReadBits(32);
	enviroSuitLight.SetSpawnId(enviroSpawnId);

	bool snapshotCrouch = msg.ReadBool();
	bool snapshotOnGround = msg.ReadBool();
	bool snapshotOnLadder = msg.ReadBool();
	bool snapshotJump = msg.ReadBool();
	bool snapShotFired = msg.ReadBool();
	bool snapShotAttackHeld = msg.ReadBool();

	byte snapshotButtons = msg.ReadByte();
	signed char snapshotForward = msg.ReadBits(-8);
	signed char snapshotRight = msg.ReadBits(-8);

	const bool snapshotSpectating = msg.ReadBool();

	// no msg reading below this

	// Update remote remote player state.
	if(!IsLocallyControlled())
	{
		previousViewQuat = nextViewQuat;
		nextViewQuat = snapViewCQuat.ToQuat();

		AI_CROUCH = snapshotCrouch;
		AI_ONGROUND = snapshotOnGround;
		AI_ONLADDER = snapshotOnLadder;
		AI_JUMP = snapshotJump;
		AI_WEAPON_FIRED = snapShotFired;
		AI_ATTACK_HELD = snapShotAttackHeld;

		oldCmd = usercmd;

		usercmd.buttons = snapshotButtons;
		usercmd.forwardmove = snapshotForward;
		usercmd.rightmove = snapshotRight;
	};

	if(weapon.SetSpawnId(weaponSpawnId))
	{
		if(weapon.GetEntity())
		{
			// maintain ownership locally
			weapon.GetEntity()->SetOwner(this);
		};
		currentWeapon = -1;
	};

	if(flashlight.SetSpawnId(flashlightSpawnId))
		if(flashlight.GetEntity())
			flashlight.GetEntity()->SetFlashlightOwner(this);

	/*
	// if not a local client
	if ( !IsLocallyControlled() ) {
		// assume the client has all ammo types
		inventory.SetRemoteClientAmmo( GetEntityNumber() );
	}
	*/

	// Update spectating state
	const bool wasSpectating = spectating;
	spectating = snapshotSpectating;

	if(spectating != wasSpectating)
		Spectate(spectating, false);

	if(oldHealth > 0 && health <= 0)
	{
		if(snapshotStale)
		{
			// so we just hide and don't show a death skin
			UpdateDeathSkin(true);
		};
		
		// die
		AI_DEAD = true;
		ClearPowerUps();
		SetAnimState(ANIMCHANNEL_LEGS, "Legs_Death", 4);
		SetAnimState(ANIMCHANNEL_TORSO, "Torso_Death", 4);
		SetWaitState("");
		animator.ClearAllJoints();
		StartRagdoll();
		physicsObj.SetMovementType(PM_DEAD);
		
		if(!snapshotStale)
			StartSound("snd_death", SND_CHANNEL_VOICE, 0, false, nullptr);
		
		if(weapon.GetEntity())
			weapon.GetEntity()->OwnerDied();
		
		if(flashlight.GetEntity())
		{
			FlashlightOff();
			flashlight.GetEntity()->OwnerDied();
		};

		if(IsLocallyControlled())
			ControllerShakeFromDamage(oldHealth - health);
	}
	else if(health < oldHealth && health > 0)
	{
		if(snapshotStale)
			lastDmgTime = gameLocal.time;
		else
		{
			// damage feedback
			const CDeclEntityDef *def = static_cast<const CDeclEntityDef *>(declManager->DeclByIndex(DECL_ENTITYDEF, lastDamageDef, false));
			if(def)
			{
				if(IsLocallyControlled())
				{
					playerView.DamageImpulse(lastDamageDir * viewAxis.Transpose(), &def->dict);
					AI_PAIN = Pain(nullptr, nullptr, oldHealth - health, lastDamageDir, lastDamageLocation);
				};
				
				lastDmgTime = gameLocal.time;
			}
			else
				gpSystem->Warning("NET: no damage def for damage feedback '%d'\n", lastDamageDef);

			if(IsLocallyControlled())
				ControllerShakeFromDamage(oldHealth - health);
		};
	}
	else if(health > oldHealth && PowerUpActive(MEGAHEALTH) && !snapshotStale)
	{
		// just pulse, for any health raise
		healthPulse = true;
	};

	// handle respawns
	if(respawn_netEvent.Get())
	{
		Init();
		StopRagdoll();
		SetPhysics(&physicsObj);
		// Explicitly set the current origin, since locally-controlled clients
		// don't interpolate. Reading the physics object from the snapshot only
		// updates the next state, not the current state.
		physicsObj.SnapToNextState();
		physicsObj.EnableClip();
		SetCombatContents(true);
		if(flashlight.GetEntity())
			flashlight.GetEntity()->Show();

		Respawn_Shared();
	};

	// If the player is alive, restore proper physics object
	if(health > 0 && IsActiveAF())
	{
		StopRagdoll();
		SetPhysics(&physicsObj);
		physicsObj.EnableClip();
		SetCombatContents(true);
	};

	const int oldIdealWeapon = idealWeapon.Get();
	idealWeapon.UpdateFromSnapshot(newIdealWeapon, GetEntityNumber());

	if(oldIdealWeapon != idealWeapon.Get())
	{
		if(snapshotStale)
			weaponCatchup = true;

		UpdateHudWeapon();
	};

	if(lastHitToggle != newHitToggle)
		SetLastHitTime(gameLocal.realClientTime);

	if(msg.HasChanged())
		UpdateVisuals();
};

/*
================
CBasePlayer::WritePlayerStateToSnapshot
================
*/
void CBasePlayer::WritePlayerStateToSnapshot(CBitMsg &msg) const
{
	msg.WriteByte(bobCycle);
	msg.WriteLong(stepUpTime);
	msg.WriteFloat(stepUpDelta);
	msg.WriteLong(inventory.weapons);
	msg.WriteByte(inventory.armor);

	inventory.WriteAmmoToSnapshot(msg);
};

/*
================
CBasePlayer::ReadPlayerStateFromSnapshot
================
*/
void CBasePlayer::ReadPlayerStateFromSnapshot(const CBitMsg &msg)
{
	int newBobCycle = 0;
	int newStepUpTime = 0;
	int newStepUpDelta = 0;

	newBobCycle = msg.ReadByte();
	newStepUpTime = msg.ReadLong();
	newStepUpDelta = msg.ReadFloat();

	inventory.weapons = msg.ReadLong();
	inventory.armor = msg.ReadByte();

	inventory.ReadAmmoFromSnapshot(msg, GetEntityNumber());
};

/*
================
CBasePlayer::ServerReceiveEvent
================
*/
bool CBasePlayer::ServerReceiveEvent(int event, int time, const idBitMsg &msg)
{
	if(CBaseEntity::ServerReceiveEvent(event, time, msg))
		return true;

	return false;
};

/*
===============
CBasePlayer::IsSoundChannelPlaying
===============
*/
bool CBasePlayer::IsSoundChannelPlaying(const s_channelType channel)
{
	if(GetSoundEmitter() != nullptr)
		return GetSoundEmitter()->CurrentlyPlaying(channel);

	return false;
};

/*
===============
CBasePlayer::Event_HideTip
===============
*/
void CBasePlayer::Event_HideTip()
{
	HideTip();
};

/*
===============
CBasePlayer::HideObjective
===============
*/
void CBasePlayer::HideObjective()
{
	StartSound("snd_objectivedown", SND_CHANNEL_ANY, 0, false, nullptr);

	if(hud)
	{
		if(objectiveUp)
		{
			hud->HideObjective(false);
			objectiveUp = false;
		}
		else
			hud->HideObjective(true);
	};
};

/*
===============
CBasePlayer::Event_StopAudioLog
===============
*/
void CBasePlayer::Event_StopAudioLog()
{
	//EndAudioLog();
};

/*
===============
CBasePlayer::SetSpectateOrigin
===============
*/
void CBasePlayer::SetSpectateOrigin()
{
	CVec3 neworig;

	neworig = GetPhysics()->GetOrigin();
	neworig[2] += pm_normalviewheight.GetFloat();
	neworig[2] += SPECTATE_RAISE;
	SetOrigin(neworig);
};

/*
===============
CBasePlayer::RemoveWeapon
===============
*/
void CBasePlayer::RemoveWeapon(const char *weap)
{
	if(weap && *weap)
		inventory.Drop(spawnArgs, spawnArgs.GetString(weap), -1);
};

/*
===============
CBasePlayer::RemoveAllButEssentialWeapons
===============
*/
void CBasePlayer::RemoveAllButEssentialWeapons()
{
	const idKeyValue *kv = spawnArgs.MatchPrefix("def_weapon", nullptr);
	for(; kv != nullptr; kv = spawnArgs.MatchPrefix("def_weapon", kv))
	{
		// This list probably ought to be placed int the player's def
		if(kv->GetValue() == "weapon_fists" || kv->GetValue() == "weapon_pda" || kv->GetValue() == "weapon_flashlight" || kv->GetValue() == "weapon_flashlight_new")
			continue;

		inventory.Drop(spawnArgs, kv->GetValue(), -1);
	};
};

/*
===============
CBasePlayer::CanShowWeaponViewmodel
===============
*/
bool CBasePlayer::CanShowWeaponViewmodel() const
{
	return ui_showGun.GetBool();
};

/*
===============
CBasePlayer::SetLevelTrigger
===============
*/
void CBasePlayer::SetLevelTrigger(const char *levelName, const char *triggerName)
{
	if(levelName && *levelName && triggerName && *triggerName)
	{
		CLevelTriggerInfo lti;
		lti.levelName = levelName;
		lti.triggerName = triggerName;
		inventory.levelTriggers.Append(lti);
	};
};

/*
===============
CBasePlayer::Event_LevelTrigger
===============
*/
void CBasePlayer::Event_LevelTrigger()
{
	CStr mapName = gameLocal.GetMapName();
	mapName.StripPath();
	mapName.StripFileExtension();
	for(int i = inventory.levelTriggers.Num() - 1; i >= 0; i--)
	{
		if(CStr::Icmp(mapName, inventory.levelTriggers[i].levelName) == 0)
		{
			CBaseEntity *ent = gameLocal.FindEntity(inventory.levelTriggers[i].triggerName);
			if(ent)
				ent->PostEventMS(&EV_Activate, 1, this);
		};
	};
};

/*
===============
CBasePlayer::Event_Gibbed
===============
*/
void CBasePlayer::Event_Gibbed()
{
	// do nothing
};

extern idCVar net_clientMaxPrediction;

/*
===============
CBasePlayer::UpdatePlayerIcons
===============
*/
void CBasePlayer::UpdatePlayerIcons()
{
	idLobbyBase &lobby = session->GetActingGameStateLobbyBase();
	int lastPacketTime = lobby.GetPeerTimeSinceLastPacket(lobby.PeerIndexFromLobbyUser(gameLocal.lobbyUserIDs[entityNumber]));
	isLagged = (lastPacketTime > net_clientMaxPrediction.GetInteger());
	//isChatting = ( ( usercmd.buttons & BUTTON_CHATTING ) && ( health > 0 ) );
};

/*
===============
CBasePlayer::DrawPlayerIcons
===============
*/
void CBasePlayer::DrawPlayerIcons()
{
	if(!NeedsIcon())
	{
		playerIcon.FreeIcon();
		return;
	};

	// Never draw icons for hidden players.
	if(this->IsHidden())
		return;

	playerIcon.Draw(this, headJoint);
};

/*
===============
CBasePlayer::HidePlayerIcons
===============
*/
void CBasePlayer::HidePlayerIcons()
{
	playerIcon.FreeIcon();
};

/*
===============
CBasePlayer::NeedsIcon
==============
*/
bool CBasePlayer::NeedsIcon()
{
	// local clients don't render their own icons... they're only info for other clients
	// always draw icons in CTF games
	return !IsLocallyControlled() && ((g_CTFArrows.GetBool() && gameLocal.mpGame.IsGametypeFlagBased() && !IsHidden() && !AI_DEAD) || (isLagged));
};

void CBasePlayer::FreeModelDef()
{
	idAFEntity_Base::FreeModelDef();
	if(mpGame->GetRules()->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased())
		playerIcon.FreeIcon();
};

/*
========================
idView::SetControllerShake
========================
*/
void CBasePlayer::SetControllerShake(float highMagnitude, int highDuration, float lowMagnitude, int lowDuration)
{
	// the main purpose of having these buffer is so multiple, individual shake events can co-exist with each other,
	// for instance, a constant low rumble from the chainsaw when it's idle and a harsh rumble when it's being used.

	// find active buffer with similar magnitude values
	int activeBufferWithSimilarMags = -1;
	int inactiveBuffer = -1;
	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
	{
		if(gameLocal.GetTime() <= controllerShakeHighTime[i] || gameLocal.GetTime() <= controllerShakeLowTime[i])
		{
			if(CMath::Fabs(highMagnitude - controllerShakeHighMag[i]) <= 0.1f && CMath::Fabs(lowMagnitude - controllerShakeLowMag[i]) <= 0.1f)
			{
				activeBufferWithSimilarMags = i;
				break;
			}
		}
		else
		{
			if(inactiveBuffer == -1)
			{
				inactiveBuffer = i; // first, inactive buffer..
			}
		}
	}

	if(activeBufferWithSimilarMags > -1)
	{
		// average the magnitudes and adjust the time
		controllerShakeHighMag[activeBufferWithSimilarMags] += highMagnitude;
		controllerShakeHighMag[activeBufferWithSimilarMags] *= 0.5f;

		controllerShakeLowMag[activeBufferWithSimilarMags] += lowMagnitude;
		controllerShakeLowMag[activeBufferWithSimilarMags] *= 0.5f;

		controllerShakeHighTime[activeBufferWithSimilarMags] = gameLocal.GetTime() + highDuration;
		controllerShakeLowTime[activeBufferWithSimilarMags] = gameLocal.GetTime() + lowDuration;
		controllerShakeTimeGroup = gameLocal.selectedGroup;
		return;
	};

	if(inactiveBuffer == -1)
		inactiveBuffer = 0; // FIXME: probably want to use the oldest buffer..

	controllerShakeHighMag[inactiveBuffer] = highMagnitude;
	controllerShakeLowMag[inactiveBuffer] = lowMagnitude;
	controllerShakeHighTime[inactiveBuffer] = gameLocal.GetTime() + highDuration;
	controllerShakeLowTime[inactiveBuffer] = gameLocal.GetTime() + lowDuration;
	controllerShakeTimeGroup = gameLocal.selectedGroup;
};

/*
========================
idView::ResetControllerShake
========================
*/
void CBasePlayer::ResetControllerShake()
{
	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
		controllerShakeHighTime[i] = 0;

	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
		controllerShakeHighMag[i] = 0.0f;

	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
		controllerShakeLowTime[i] = 0;

	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
		controllerShakeLowMag[i] = 0.0f;
};

/*
========================
CBasePlayer::GetControllerShake
========================
*/
void CBasePlayer::GetControllerShake(int &highMagnitude, int &lowMagnitude) const
{
	if(gameLocal.inCinematic)
	{
		// no controller shake during cinematics
		highMagnitude = 0;
		lowMagnitude = 0;
		return;
	}

	float lowMag = 0.0f;
	float highMag = 0.0f;

	lowMagnitude = 0;
	highMagnitude = 0;

	// use highest values from active buffers
	for(int i = 0; i < MAX_SHAKE_BUFFER; i++)
	{
		if(gameLocal.GetTimeGroupTime(controllerShakeTimeGroup) < controllerShakeLowTime[i])
		{
			if(controllerShakeLowMag[i] > lowMag)
				lowMag = controllerShakeLowMag[i];
		}
		if(gameLocal.GetTimeGroupTime(controllerShakeTimeGroup) < controllerShakeHighTime[i])
		{
			if(controllerShakeHighMag[i] > highMag)
				highMag = controllerShakeHighMag[i];
		};
	};

	lowMagnitude = CMath::Ftoi(lowMag * 65535.0f);
	highMagnitude = CMath::Ftoi(highMag * 65535.0f);
};

/*
========================
CBasePlayer::GetExpansionType
========================
*/
gameExpansionType_t CBasePlayer::GetExpansionType() const
{
	const char *expansion = spawnArgs.GetString("player_expansion", "d3");
	
	if(idStr::Icmp(expansion, "d3") == 0)
		return GAME_BASE;
	
	return GAME_UNKNOWN;
};