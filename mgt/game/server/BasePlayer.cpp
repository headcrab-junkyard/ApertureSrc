/*
	player.qc

	player functions/definitions

	Copyright (C) 1996-1997  Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

/// @file

void bubble_bob();

/*
==============================================================================

PLAYER

==============================================================================
*/

$cd /raid/quake/id1/models/player_4
$origin 0 -6 24
$base base              
$skin skin

//
// running
//
$frame axrun1 axrun2 axrun3 axrun4 axrun5 axrun6

$frame rockrun1 rockrun2 rockrun3 rockrun4 rockrun5 rockrun6

//
// standing
//
$frame stand1 stand2 stand3 stand4 stand5

$frame axstnd1 axstnd2 axstnd3 axstnd4 axstnd5 axstnd6
$frame axstnd7 axstnd8 axstnd9 axstnd10 axstnd11 axstnd12


//
// pain
//
$frame axpain1 axpain2 axpain3 axpain4 axpain5 axpain6

$frame pain1 pain2 pain3 pain4 pain5 pain6


//
// death
//

$frame axdeth1 axdeth2 axdeth3 axdeth4 axdeth5 axdeth6
$frame axdeth7 axdeth8 axdeth9

$frame deatha1 deatha2 deatha3 deatha4 deatha5 deatha6 deatha7 deatha8
$frame deatha9 deatha10 deatha11

$frame deathb1 deathb2 deathb3 deathb4 deathb5 deathb6 deathb7 deathb8
$frame deathb9

$frame deathc1 deathc2 deathc3 deathc4 deathc5 deathc6 deathc7 deathc8
$frame deathc9 deathc10 deathc11 deathc12 deathc13 deathc14 deathc15

$frame deathd1 deathd2 deathd3 deathd4 deathd5 deathd6 deathd7
$frame deathd8 deathd9

$frame deathe1 deathe2 deathe3 deathe4 deathe5 deathe6 deathe7
$frame deathe8 deathe9

//
// attacks
//
$frame nailatt1 nailatt2

$frame light1 light2

$frame rockatt1 rockatt2 rockatt3 rockatt4 rockatt5 rockatt6

$frame shotatt1 shotatt2 shotatt3 shotatt4 shotatt5 shotatt6

$frame axatt1 axatt2 axatt3 axatt4 axatt5 axatt6

$frame axattb1 axattb2 axattb3 axattb4 axattb5 axattb6

$frame axattc1 axattc2 axattc3 axattc4 axattc5 axattc6

$frame axattd1 axattd2 axattd3 axattd4 axattd5 axattd6


/*
==============================================================================
PLAYER
==============================================================================
*/

void player_run();

void()  player_stand1 =[        $axstnd1,       player_stand1   ]
{
	self.weaponframe=0;
	if (self.velocity_x || self.velocity_y)
	{
		self.walkframe=0;
		player_run();
		return;
	}

	if (self.weapon == IT_AXE)
	{
		if (self.walkframe >= 12)
			self.walkframe = 0;
		self.frame = $axstnd1 + self.walkframe;
	}
	else
	{
		if (self.walkframe >= 5)
			self.walkframe = 0;
		self.frame = $stand1 + self.walkframe;
	}
	self.walkframe = self.walkframe + 1;    
};

void()  player_run =[   $rockrun1,      player_run      ]
{
	self.weaponframe=0;
	if (!self.velocity_x && !self.velocity_y)
	{
		self.walkframe=0;
		player_stand1();
		return;
	}

	if (self.weapon == IT_AXE)
	{
		if (self.walkframe == 6)
			self.walkframe = 0;
		self.frame = $axrun1 + self.walkframe;
	}
	else
	{
		if (self.walkframe == 6)
			self.walkframe = 0;
		self.frame = self.frame + self.walkframe;
	}
	self.walkframe = self.walkframe + 1;
};

void()muzzleflash =
{
	WriteByte (MSG_MULTICAST, SVC_MUZZLEFLASH);
	WriteEntity (MSG_MULTICAST, self);
	multicast (self.origin, MULTICAST_PVS);
};


void()  player_shot1 =  [$shotatt1, player_shot2        ] {self.weaponframe=1;muzzleflash();};
void()  player_shot2 =  [$shotatt2, player_shot3        ] {self.weaponframe=2;};
void()  player_shot3 =  [$shotatt3, player_shot4        ] {self.weaponframe=3;};
void()  player_shot4 =  [$shotatt4, player_shot5        ] {self.weaponframe=4;};
void()  player_shot5 =  [$shotatt5, player_shot6        ] {self.weaponframe=5;};
void()  player_shot6 =  [$shotatt6, player_run  ] {self.weaponframe=6;};

void()  player_axe1 =   [$axatt1, player_axe2   ] {self.weaponframe=1;};
void()  player_axe2 =   [$axatt2, player_axe3   ] {self.weaponframe=2;};
void()  player_axe3 =   [$axatt3, player_axe4   ] {self.weaponframe=3;W_FireAxe();};
void()  player_axe4 =   [$axatt4, player_run    ] {self.weaponframe=4;};

void()  player_axeb1 =  [$axattb1, player_axeb2 ] {self.weaponframe=5;};
void()  player_axeb2 =  [$axattb2, player_axeb3 ] {self.weaponframe=6;};
void()  player_axeb3 =  [$axattb3, player_axeb4 ] {self.weaponframe=7;W_FireAxe();};
void()  player_axeb4 =  [$axattb4, player_run   ] {self.weaponframe=8;};

void()  player_axec1 =  [$axattc1, player_axec2 ] {self.weaponframe=1;};
void()  player_axec2 =  [$axattc2, player_axec3 ] {self.weaponframe=2;};
void()  player_axec3 =  [$axattc3, player_axec4 ] {self.weaponframe=3;W_FireAxe();};
void()  player_axec4 =  [$axattc4, player_run   ] {self.weaponframe=4;};

void()  player_axed1 =  [$axattd1, player_axed2 ] {self.weaponframe=5;};
void()  player_axed2 =  [$axattd2, player_axed3 ] {self.weaponframe=6;};
void()  player_axed3 =  [$axattd3, player_axed4 ] {self.weaponframe=7;W_FireAxe();};
void()  player_axed4 =  [$axattd4, player_run   ] {self.weaponframe=8;};


//============================================================================

void() player_nail1   =[$nailatt1, player_nail2  ] 
{
	muzzleflash();

	if (!self.button0 || intermission_running || self.impulse)
		{player_run ();return;}
	self.weaponframe = self.weaponframe + 1;
	if (self.weaponframe == 9)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireSpikes (4);
	self.attack_finished = time + 0.2;
};
void() player_nail2   =[$nailatt2, player_nail1  ]
{
	muzzleflash();

	if (!self.button0 || intermission_running || self.impulse)
		{player_run ();return;}
	self.weaponframe = self.weaponframe + 1;
	if (self.weaponframe == 9)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireSpikes (-4);
	self.attack_finished = time + 0.2;
};

//============================================================================

void() player_light1   =[$light1, player_light2  ] 
{
	muzzleflash();

	if (!self.button0 || intermission_running)
		{player_run ();return;}
	self.weaponframe = self.weaponframe + 1;
	if (self.weaponframe == 5)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireLightning();
	self.attack_finished = time + 0.2;
};
void() player_light2   =[$light2, player_light1  ]
{
	muzzleflash();

	if (!self.button0 || intermission_running)
		{player_run ();return;}
	self.weaponframe = self.weaponframe + 1;
	if (self.weaponframe == 5)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireLightning();
	self.attack_finished = time + 0.2;
};

//============================================================================


void() player_rocket1   =[$rockatt1, player_rocket2  ] {self.weaponframe=1; muzzleflash();};
void() player_rocket2   =[$rockatt2, player_rocket3  ] {self.weaponframe=2;};
void() player_rocket3   =[$rockatt3, player_rocket4  ] {self.weaponframe=3;};
void() player_rocket4   =[$rockatt4, player_rocket5  ] {self.weaponframe=4;};
void() player_rocket5   =[$rockatt5, player_rocket6  ] {self.weaponframe=5;};
void() player_rocket6   =[$rockatt6, player_run  ] {self.weaponframe=6;};
void DeathBubbles(float num_bubbles);

CBasePlayer::CBasePlayer() = default;
CBasePlayer::~CBasePlayer() = default;

/*
==============
idPlayer::Spawn

Prepare any resources used by the player.
==============
*/
void CBasePlayer::Spawn()
{
	idStr		temp;
	idBounds	bounds;

	if ( entityNumber >= MAX_CLIENTS ) {
		gameLocal.Error( "entityNum > MAX_CLIENTS for player.  Player may only be spawned with a client." );
	}

	// allow thinking during cinematics
	cinematic = true;

	if ( common->IsMultiplayer() ) {
		// always start in spectating state waiting to be spawned in
		// do this before SetClipModel to get the right bounding box
		spectating = true;
	}

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
	if ( !common->IsMultiplayer() || IsLocallyControlled() ) {

		// load HUD
		if ( hudManager != NULL ) {
			hudManager->Initialize( "hud", common->SW() );
			hudManager->ActivateMenu( true );
			hud = hudManager->GetHud();
		}

		// load cursor
		if ( spawnArgs.GetString( "cursor", "", temp ) ) {
			cursor = uiManager->FindGui( temp, true, common->IsMultiplayer(), common->IsMultiplayer() );
		}
		if ( cursor ) {
			cursor->Activate( true, gameLocal.time );
		}

		if ( pdaMenu != NULL ) {
			pdaMenu->Initialize( "pda", common->SW() );
		}
		objectiveSystemOpen = false;
	} 

	if ( common->IsMultiplayer() && mpMessages == NULL ) {
		mpMessages = new idSWF( "mp_messages", common->SW() );
		mpMessages->Activate( true );
	}

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
	if ( headEnt ) {
		headEnt->GetRenderEntity()->suppressSurfaceInViewID = entityNumber+1;
		headEnt->GetRenderEntity()->noSelfShadow = true;
	}

	if ( common->IsMultiplayer() ) {
		Init();
		Hide();	// properly hidden if starting as a spectator
		if ( !common->IsClient() ) {
			// set yourself ready to spawn. idMultiplayerGame will decide when/if appropriate and call SpawnFromSpawnSpot
			SetupWeaponEntity();
			SpawnFromSpawnSpot();
			forceRespawn = true;
			wantSpectate = true;
			assert( spectating );
		}
	} else {
		SetupWeaponEntity();
		SpawnFromSpawnSpot();
	}

	// trigger playtesting item gives, if we didn't get here from a previous level
	// the devmap key will be set on the first devmap, but cleared on any level
	// transitions
	if ( !common->IsMultiplayer() && gameLocal.serverInfo.FindKey( "devmap" ) ) {
		// fire a trigger with the name "devmap"
		idEntity *ent = gameLocal.FindEntity( "devmap" );
		if ( ent ) {
			ent->ActivateTargets( this );
		}
	}

	if ( hud ) {
		if ( weapon_soulcube > 0 && ( inventory.weapons & ( 1 << weapon_soulcube ) ) ) {
			int max_souls = inventory.MaxAmmoForAmmoClass( this, "ammo_souls" );
			if ( inventory.GetInventoryAmmoForType( idWeapon::GetAmmoNumForName( "ammo_souls" ) ) >= max_souls ) {
				hud->SetShowSoulCubeOnLoad( true );
			}
		}		
	}

	if ( GetPDA() ) {
		// Add any emails from the inventory
		for ( int i = 0; i < inventory.emails.Num(); i++ ) {
			GetPDA()->AddEmail( inventory.emails[i] );
		}
		GetPDA()->SetSecurity( idLocalization::GetString( "#str_00066" ) );
	}

	if ( gameLocal.world->spawnArgs.GetBool( "no_Weapons" ) ) {
		hiddenWeapon = true;
		if ( weapon.GetEntity() ) {
			weapon.GetEntity()->LowerWeapon();
		}
		idealWeapon = weapon_fists;
	} else {
		hiddenWeapon = false;
	}
	
	UpdateHudWeapon();

	tipUp = false;
	objectiveUp = false;

	if ( inventory.levelTriggers.Num() ) {
		PostEventMS( &EV_Player_LevelTrigger, 0 );
	}

	inventory.pdaOpened = false;
	inventory.selPDA = 0;

	if ( !common->IsMultiplayer() ) {
		int startingHealth = gameLocal.world->spawnArgs.GetInt( "startingHealth", health );
		if ( health > startingHealth ) {
			health = startingHealth;
		}
		if ( g_skill.GetInteger() < 2 ) {
			if ( health < 25 ) {
				health = 25;
			}
			if ( g_useDynamicProtection.GetBool() ) {
				new_g_damageScale = 1.0f;
			}
		} else {
			new_g_damageScale = 1.0f;
			g_armorProtection.SetFloat( ( g_skill.GetInteger() < 2 ) ? 0.4f : 0.2f );
			if ( g_skill.GetInteger() == 3 ) {
				nextHealthTake = gameLocal.time + g_healthTakeTime.GetInteger() * 1000;
			}
		}
	}

	//Setup the weapon toggle lists
	const idKeyValue *kv;
	kv = spawnArgs.MatchPrefix( "weapontoggle", NULL );
	while( kv ) {
		WeaponToggle_t newToggle;
		strcpy(newToggle.name, kv->GetKey().c_str());

		idStr toggleData = kv->GetValue();

		idLexer src;
		idToken token;
		src.LoadMemory(toggleData, toggleData.Length(), "toggleData");
		while(1) {
			if(!src.ReadToken(&token)) {
				break;
			}
			int index = atoi(token.c_str());
			newToggle.toggleList.Append(index);

			//Skip the ,
			src.ReadToken(&token);
		}
		newToggle.lastUsed = 0;
		weaponToggles.Set(newToggle.name, newToggle);

		kv = spawnArgs.MatchPrefix( "weapontoggle", kv );
	}

	if( g_skill.GetInteger() >= 3 || cvarSystem->GetCVarBool( "fs_buildresources" ) ) {
		if(!WeaponAvailable("weapon_bloodstone_passive")) {
			GiveInventoryItem("weapon_bloodstone_passive");
		}
		if(!WeaponAvailable("weapon_bloodstone_active1")) {
			GiveInventoryItem("weapon_bloodstone_active1");
		}
		if(!WeaponAvailable("weapon_bloodstone_active2")) {
			GiveInventoryItem("weapon_bloodstone_active2");
		}
		if(!WeaponAvailable("weapon_bloodstone_active3")) {
			GiveInventoryItem("weapon_bloodstone_active3");
		}
	}

	bloomEnabled			= false;
	bloomSpeed				= 1;
	bloomIntensity			= -0.01f;

	if ( g_demoMode.GetBool() && weapon.GetEntity() && weapon.GetEntity()->AmmoInClip() == 0 ) {
		weapon.GetEntity()->ForceAmmoInClip();
	}

}

void CBasePlayer::Think()
{
};

/*
==================
idPlayer::Killed
==================
*/
void CBasePlayer::Killed( CBaseEntity *inflictor, CBaseEntity *attacker, int damage, const idVec3 &dir, int location )
{
	float delay;

	assert( !common->IsClient() );

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

	if ( !g_testDeath.GetBool() && !common->IsMultiplayer() )
		playerView.Fade( colorBlack, 3000 );

	AI_DEAD = true;
	SetAnimState( ANIMCHANNEL_LEGS, "Legs_Death", 4 );
	SetAnimState( ANIMCHANNEL_TORSO, "Torso_Death", 4 );
	SetWaitState( "" );

	animator.ClearAllJoints();

	if ( StartRagdoll() ) {
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
	StartSound( "snd_death", SND_CHANNEL_VOICE, 0, false, NULL );
	StopSound( SND_CHANNEL_BODY2, false );

	fl.takedamage = true;		// can still be gibbed

	// get rid of weapon
	weapon.GetEntity()->OwnerDied();
	
	// In multiplayer, get rid of the flashlight, or other players
	// will see it floating after the player is dead.
	if ( common->IsMultiplayer() )
	{
		FlashlightOff();
		if ( flashlight.GetEntity() )
			flashlight.GetEntity()->OwnerDied();
	};

	// drop the weapon as an item
	DropWeapon( true );

	// drop the flag if player was carrying it
	if ( common->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased() && carryingFlag )
		DropFlag();

	if ( !g_testDeath.GetBool() )
		LookAtKiller( inflictor, attacker );

	if ( common->IsMultiplayer() || g_testDeath.GetBool() )
	{
		idPlayer *killer = NULL;
		// no gibbing in MP. Event_Gib will early out in MP
		if ( attacker->IsType( idPlayer::Type ) ) {
			killer = static_cast<idPlayer*>(attacker);
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

/*
============
Damage

this		entity that is being damaged
inflictor	entity that is causing the damage
attacker	entity that caused the inflictor to damage targ
	example: this=monster, inflictor=rocket, attacker=player

dir			direction of the attack for knockback in global space

damageDef	an idDict with all the options for damage effects

inflictor, attacker, dir, and point can be NULL for environmental effects
============
*/
void idPlayer::TakeDamage( CBaseEntity *inflictor, CBaseEntity *attacker, const idVec3 &dir, const char *damageDefName, const float damageScale, const int location )
{
	idVec3		kick;
	int			damage;
	int			armorSave;

	SetTimeState ts( timeGroup );
	
	if ( !fl.takedamage || noclip || spectating || gameLocal.inCinematic )
		return;

	if ( !inflictor )
		inflictor = gameLocal.world;

	if ( !attacker )
		attacker = gameLocal.world;

	if ( attacker->IsType( idAI::Type ) )
	{
		if ( PowerUpActive( BERSERK ) )
			return;

		// don't take damage from monsters during influences
		if ( influenceActive != 0 )
			return;
	}

	const idDeclEntityDef *damageDef = gameLocal.FindEntityDef( damageDefName, false );
	if ( !damageDef )
	{
		gameLocal.Warning( "Unknown damageDef '%s'", damageDefName );
		return;
	}

	if ( damageDef->dict.GetBool( "ignore_player" ) )
		return;

	// determine knockback
	int knockback = 0;
	damageDef->dict.GetInt( "knockback", "20", knockback );

	if ( knockback != 0 && !fl.noknockback )
	{
		float attackerPushScale = 0.0f;

		if ( attacker == this )
			damageDef->dict.GetFloat( "attackerPushScale", "0", attackerPushScale );
		else
			attackerPushScale = 1.0f;

		idVec3 kick = dir;
		kick.Normalize();
		kick *= g_knockback.GetFloat() * knockback * attackerPushScale / 200.0f;
		physicsObj.SetLinearVelocity( physicsObj.GetLinearVelocity() + kick );

		// set the timer so that the player can't cancel out the movement immediately
		physicsObj.SetKnockBack( idMath::ClampInt( 50, 200, knockback * 2 ) );	

		if ( common->IsServer() )
		{
			idBitMsg	msg;
			byte		msgBuf[MAX_EVENT_PARAM_SIZE];

			msg.InitWrite( msgBuf, sizeof( msgBuf ) );
			msg.WriteFloat( physicsObj.GetLinearVelocity()[0] );
			msg.WriteFloat( physicsObj.GetLinearVelocity()[1] );
			msg.WriteFloat( physicsObj.GetLinearVelocity()[2] );
			msg.WriteByte( idMath::ClampInt( 50, 200, knockback * 2 ) );
			ServerSendEvent( idPlayer::EVENT_KNOCKBACK, &msg, false );
		}
	}

	// If this is a locally controlled MP client, don't apply damage effects predictively here.
	// Local clients will see the damage feedback (view kick, etc) when their health changes
	// in a snapshot. This ensures that any feedback the local player sees is in sync with
	// his actual health reported by the server.
	if( common->IsMultiplayer() && common->IsClient() && IsLocallyControlled() )
		return;
	
	CalcDamagePoints( inflictor, attacker, &damageDef->dict, damageScale, location, &damage, &armorSave );

	// give feedback on the player view and audibly when armor is helping
	if ( armorSave )
	{
		inventory.armor -= armorSave;

		if ( gameLocal.time > lastArmorPulse + 200 )
			StartSound( "snd_hitArmor", SND_CHANNEL_ITEM, 0, false, NULL );

		lastArmorPulse = gameLocal.time;
	};
	
	if ( damageDef->dict.GetBool( "burn" ) )
		StartSound( "snd_burn", SND_CHANNEL_BODY3, 0, false, NULL );
	else if ( damageDef->dict.GetBool( "no_air" ) )
	{
		if ( !armorSave && health > 0 )
			StartSound( "snd_airGasp", SND_CHANNEL_ITEM, 0, false, NULL );
	};

	if ( g_debugDamage.GetInteger() )
		gameLocal.Printf( "client:%02d\tdamage type:%s\t\thealth:%03d\tdamage:%03d\tarmor:%03d\n", entityNumber, damageDef->GetName(), health, damage, armorSave );

	if ( common->IsMultiplayer() && IsLocallyControlled() )
		ControllerShakeFromDamage( damage );

	// The client needs to know the final damage amount for predictive pain animations.
	const int finalDamage = AdjustDamageAmount( damage );

	if ( health > 0 )
	{
		// force a blink
		blink_time = 0;

		// let the anim script know we took damage
		AI_PAIN = Pain( inflictor, attacker, damage, dir, location );
	}

	// Only actually deal the damage here in singleplayer and for locally controlled servers.
	if ( !common->IsMultiplayer() || common->IsServer() )
	{
		// Server will deal his damage normally
		ServerDealDamage( finalDamage, *inflictor, *attacker, dir, damageDefName, location );
	}
	else if ( attacker->GetEntityNumber() == gameLocal.GetLocalClientNum() )
	{
		// Clients send a reliable message to the server with the parameters of the hit. The
		// server should make sure the client still has line-of-sight to its target before
		// actually applying the damage.

		byte msgBuffer[MAX_GAME_MESSAGE_SIZE]; 
		idBitMsg msg;

		msg.InitWrite( msgBuffer, sizeof( msgBuffer ) );
		msg.BeginWriting();

		msg.WriteShort( attacker->GetEntityNumber() );
		msg.WriteShort( GetEntityNumber() );		// victim
		msg.WriteVectorFloat( dir );
		msg.WriteLong( damageDef->Index() );
		msg.WriteFloat( damageScale );
		msg.WriteLong( location );

		idLobbyBase & lobby = session->GetActingGameStateLobbyBase();
		lobby.SendReliableToHost( GAME_RELIABLE_MESSAGE_CLIENT_HITSCAN_HIT, msg );
	};

	lastDamageDef = damageDef->Index();
	lastDamageDir = dir;
	lastDamageDir.Normalize();
	lastDamageLocation = location;
};

/*
=================
idPlayer::StartHealthRecharge
=================
*/
void CBasePlayer::StartHealthRecharge(int speed)
{
	lastHealthRechargeTime = gameLocal.time;
	healthRecharge = true;
	rechargeSpeed = speed;
};

/*
=================
idPlayer::StopHealthRecharge
=================
*/
void CBasePlayer::StopHealthRecharge()
{
	healthRecharge = false;
};

/*
================
idPlayer::Hide
================
*/
void CBasePlayer::Hide()
{
	idActor::Hide();
	
	CBaseWeapon *weap = weapon.GetEntity();
	if ( weap )
		weap->HideWorldModel();

	CBaseWeapon *flash = flashlight.GetEntity();
	if( flash )
		flash->HideWorldModel();
};

/*
================
idPlayer::Show
================
*/
void CBasePlayer::Show()
{
	idActor::Show();
	
	CBaseWeapon *weap = weapon.GetEntity();
	if ( weap )
		weap->ShowWorldModel();
	
	CBaseWeapon * flash = flashlight.GetEntity();
	if( flash )
		flash->ShowWorldModel();
};

/*
==================
idPlayer::LookAtKiller
==================
*/
void CBasePlayer::LookAtKiller( CBaseEntity *inflictor, CBaseEntity *attacker )
{
	idVec3 dir;
	
	if ( attacker && attacker != this )
		dir = attacker->GetPhysics()->GetOrigin() - GetPhysics()->GetOrigin();
	else if ( inflictor && inflictor != this )
		dir = inflictor->GetPhysics()->GetOrigin() - GetPhysics()->GetOrigin();
	else
		dir = viewAxis[ 0 ];

	idAngles ang( 0, dir.ToYaw(), 0 );
	SetViewAngles( ang );
};

/*
==============
idPlayer::TogglePDA
==============
*/
void CBasePlayer::TogglePDA()
{
	if ( inventory.pdas.Num() == 0 ) {
		ShowTip( spawnArgs.GetString( "text_infoTitle" ), spawnArgs.GetString( "text_noPDA" ), true );
		return;
	}

	if ( pdaMenu != NULL ) {
		objectiveSystemOpen = !objectiveSystemOpen;
		pdaMenu->ActivateMenu( objectiveSystemOpen );
		
		if ( objectiveSystemOpen ) {
			if ( hud ) {
				hud->ClearNewPDAInfo();
			}
		}		
	}
};

/*
==============
idPlayer::PlayAudioLog
==============
*/
void CBasePlayer::PlayAudioLog( const idSoundShader * shader )
{
	EndVideoDisk();
	if ( name.Length() > 0 )
	{
		int ms;
		StartSoundShader( shader, SND_CHANNEL_PDA_AUDIO, 0, false, &ms );
		CancelEvents( &EV_Player_StopAudioLog );
		PostEventMS( &EV_Player_StopAudioLog, ms + 150 );
	};
};

/*
==============
idPlayer::EndAudioLog
==============
*/
void CBasePlayer::EndAudioLog()
{
	StopSound( SND_CHANNEL_PDA_AUDIO, false );
};

/*
===============
idPlayer::UpdateWeapon
===============
*/
void CBasePlayer::UpdateWeapon()
{
	if ( health <= 0 ) {
		return;
	}

	assert( !spectating );

	if ( common->IsClient() ) {
		// clients need to wait till the weapon and it's world model entity
		// are present and synchronized ( weapon.worldModel idEntityPtr to idAnimatedEntity )
		if ( !weapon.GetEntity()->IsWorldModelReady() ) {
			return;
		}
	}

	// always make sure the weapon is correctly setup before accessing it
	if ( !weapon.GetEntity()->IsLinked() ) {
		if ( idealWeapon != -1 ) {
			animPrefix = spawnArgs.GetString( va( "def_weapon%d", idealWeapon.Get() ) );
			int ammoInClip = inventory.GetClipAmmoForWeapon( idealWeapon.Get() );
			if ( common->IsMultiplayer() && respawning ) {
				// Do not load ammo into the clip here on MP respawn, as it will be done
				// elsewhere. If we take ammo out here then the player will end up losing
				// a clip of ammo for their initial weapon upon respawn.
				ammoInClip = 0;
			}
			weapon.GetEntity()->GetWeaponDef( animPrefix, ammoInClip );
			assert( weapon.GetEntity()->IsLinked() );
		} else {
			return;
		}
	}

	if ( hiddenWeapon && tipUp && usercmd.buttons & BUTTON_ATTACK ) {
		HideTip();
	}
	
	if ( g_dragEntity.GetBool() ) {
		StopFiring();
		weapon.GetEntity()->LowerWeapon();
		dragEntity.Update( this );
	} else if ( ActiveGui() ) {
		// gui handling overrides weapon use
		Weapon_GUI();
	} else 	if ( focusCharacter && ( focusCharacter->health > 0 ) ) {
		Weapon_NPC();
	} else {
		Weapon_Combat();
	}
	
	if ( hiddenWeapon ) {
		weapon.GetEntity()->LowerWeapon();
	}

	// update weapon state, particles, dlights, etc
	weapon.GetEntity()->PresentWeapon( CanShowWeaponViewmodel() );
}

/*
===============
idPlayer::UpdateFlashLight
===============
*/
void CBasePlayer::UpdateFlashlight()
{
	if ( idealWeapon == weapon_flashlight ) {
		// force classic flashlight to go away
		NextWeapon();
	}

	if ( !flashlight.IsValid() ) {
		return;
	}

	if ( !flashlight.GetEntity()->GetOwner() ) {
		return;
	}

	// Don't update the flashlight if dead in MP.
	// Otherwise you can see a floating flashlight worldmodel near player's skeletons.
	if ( common->IsMultiplayer() ) {
		if ( health < 0 ) {
			return;
		}
	}

	// Flashlight has an infinite battery in multiplayer.
	if ( !common->IsMultiplayer() ) {
		if ( flashlight.GetEntity()->lightOn ) {
			if ( flashlight_batteryDrainTimeMS.GetInteger() > 0 ) {
				flashlightBattery -= ( gameLocal.time - gameLocal.previousTime );
				if ( flashlightBattery < 0 ) {
					FlashlightOff();
					flashlightBattery = 0;
				}
			}
		} else {
			if ( flashlightBattery < flashlight_batteryDrainTimeMS.GetInteger() ) {
				flashlightBattery += ( gameLocal.time - gameLocal.previousTime ) * Max( 1, ( flashlight_batteryDrainTimeMS.GetInteger() / flashlight_batteryChargeTimeMS.GetInteger() ) );
				if ( flashlightBattery > flashlight_batteryDrainTimeMS.GetInteger() ) {
					flashlightBattery = flashlight_batteryDrainTimeMS.GetInteger();
				}
			}
		}
	}

	if ( hud ) {
		hud->UpdateFlashlight( this );
	}

	if ( common->IsClient() ) {
		// clients need to wait till the weapon and it's world model entity
		// are present and synchronized ( weapon.worldModel idEntityPtr to idAnimatedEntity )
		if ( !flashlight.GetEntity()->IsWorldModelReady() ) {
			return;
		}
	}

	// always make sure the weapon is correctly setup before accessing it
	if ( !flashlight.GetEntity()->IsLinked() ) {
		flashlight.GetEntity()->GetWeaponDef( "weapon_flashlight_new", 0 );
		flashlight.GetEntity()->SetIsPlayerFlashlight( true );

		// adjust position / orientation of flashlight
		idAnimatedEntity *worldModel = flashlight.GetEntity()->GetWorldModel();
		worldModel->BindToJoint( this, "Chest", true );
		// Don't interpolate the flashlight world model in mp, let it bind like normal.
		worldModel->SetUseClientInterpolation( false );

		assert( flashlight.GetEntity()->IsLinked() );
	}

	// this positions the third person flashlight model! (as seen in the mirror)
	idAnimatedEntity *worldModel = flashlight.GetEntity()->GetWorldModel();
	static const idVec3 fl_pos = idVec3( 3.0f, 9.0f, 2.0f );
	worldModel->GetPhysics()->SetOrigin( fl_pos );
	static float fl_pitch = 0.0f;
	static float fl_yaw = 0.0f;
	static float fl_roll = 0.0f;
	static idAngles ang = ang_zero;
	ang.Set( fl_pitch, fl_yaw, fl_roll );
	worldModel->GetPhysics()->SetAxis( ang.ToMat3() );

	if ( flashlight.GetEntity()->lightOn ) {
		if ( ( flashlightBattery < flashlight_batteryChargeTimeMS.GetInteger() / 2 ) && ( gameLocal.random.RandomFloat() < flashlight_batteryFlickerPercent.GetFloat() ) ) {
			flashlight.GetEntity()->RemoveMuzzleFlashlight();
		} else {
			flashlight.GetEntity()->MuzzleFlashLight();
		}
	}

	flashlight.GetEntity()->PresentWeapon( true );

	if ( gameLocal.world->spawnArgs.GetBool( "no_Weapons" ) || gameLocal.inCinematic || spectating || fl.hidden ) {
		worldModel->Hide();
	} else {
		worldModel->Show();
	}
}

/*
=================
idPlayer::DropWeapon
=================
*/
void CBasePlayer::DropWeapon( bool died )
{
	idVec3 forward, up;
	int inclip, ammoavailable;

	if( died == false  ){ 
		return;
	}

	assert( !common->IsClient() );
	
	if ( spectating || weaponGone || weapon.GetEntity() == NULL ) {
		return;
	}
	
	if ( ( !died && !weapon.GetEntity()->IsReady() ) || weapon.GetEntity()->IsReloading() ) {
		return;
	}
	// ammoavailable is how many shots we can fire
	// inclip is which amount is in clip right now
	ammoavailable = weapon.GetEntity()->AmmoAvailable();
	inclip = weapon.GetEntity()->AmmoInClip();
	
	// don't drop a grenade if we have none left
	if ( !idStr::Icmp( idWeapon::GetAmmoNameForNum( weapon.GetEntity()->GetAmmoType() ), "ammo_grenades" ) && ( ammoavailable - inclip <= 0 ) ) {
		return;
	}

	ammoavailable += inclip;

	// expect an ammo setup that makes sense before doing any dropping
	// ammoavailable is -1 for infinite ammo, and weapons like chainsaw
	// a bad ammo config usually indicates a bad weapon state, so we should not drop
	// used to be an assertion check, but it still happens in edge cases

	if ( ( ammoavailable != -1 ) && ( ammoavailable < 0 ) ) {
		common->DPrintf( "idPlayer::DropWeapon: bad ammo setup\n" );
		return;
	}
	idEntity *item = NULL;
	if ( died ) {
		// ain't gonna throw you no weapon if I'm dead
		item = weapon.GetEntity()->DropItem( vec3_origin, 0, WEAPON_DROP_TIME, died );
	} else {
		viewAngles.ToVectors( &forward, NULL, &up );
		item = weapon.GetEntity()->DropItem( 250.0f * forward + 150.0f * up, 500, WEAPON_DROP_TIME, died );
	}
	if ( !item ) {
		return;
	}
	// set the appropriate ammo in the dropped object
	const idKeyValue * keyval = item->spawnArgs.MatchPrefix( "inv_ammo_" );
	if ( keyval ) {
		item->spawnArgs.SetInt( keyval->GetKey(), ammoavailable );
		idStr inclipKey = keyval->GetKey();
		inclipKey.Insert( "inclip_", 4 );
		inclipKey.Insert( va("%.2d", currentWeapon), 11);
		item->spawnArgs.SetInt( inclipKey, inclip );
	}
	if ( !died ) {
		// remove from our local inventory completely
		inventory.Drop( spawnArgs, item->spawnArgs.GetString( "inv_weapon" ), -1 );
		weapon.GetEntity()->ResetAmmoClip();
		NextWeapon();
		weapon.GetEntity()->WeaponStolen();
		weaponGone = true;
	}
}

/*
===============
idPlayer::NextWeapon
===============
*/
void CBasePlayer::NextWeapon()
{

	if ( !weaponEnabled || spectating || hiddenWeapon || gameLocal.inCinematic || gameLocal.world->spawnArgs.GetBool( "no_Weapons" ) || health < 0 ) {
		return;
	}

	// check if we have any weapons
	if ( !inventory.weapons ) {
		return;
	}
	
	int w = idealWeapon.Get();
	while( 1 ) {
		w++;
		if ( w >= MAX_WEAPONS ) {
			w = 0;
		}
		if ( w == idealWeapon ) {
			w = weapon_fists;
			break;
		}
		if ( ( inventory.weapons & ( 1 << w ) ) == 0 ) {
			continue;
		}
		const char * weap = spawnArgs.GetString( va( "def_weapon%d", w ) );
		if ( !spawnArgs.GetBool( va( "weapon%d_cycle", w ) ) ) {
			continue;
		}
		if ( !weap[ 0 ] ) {
			continue;
		}

		if ( inventory.HasAmmo( weap, true, this ) || w == weapon_bloodstone ) {
			break;
		}
	}

	if ( ( w != currentWeapon ) && ( w != idealWeapon ) ) {
		idealWeapon = w;
		weaponSwitchTime = gameLocal.time + WEAPON_SWITCH_DELAY;
		UpdateHudWeapon();
	}
}

/*
===============
idPlayer::PrevWeapon
===============
*/
void CBasePlayer::PrevWeapon()
{

	if ( !weaponEnabled || spectating || hiddenWeapon || gameLocal.inCinematic || gameLocal.world->spawnArgs.GetBool( "no_Weapons" ) || health < 0 ) {
		return;
	}

	// check if we have any weapons
	if ( !inventory.weapons ) {
		return;
	}

	int w = idealWeapon.Get();
	while( 1 ) {
		w--;
		if ( w < 0 ) {
			w = MAX_WEAPONS - 1;
		}
		if ( w == idealWeapon ) {
			w = weapon_fists;
			break;
		}
		if ( ( inventory.weapons & ( 1 << w ) ) == 0 ) {
			continue;
		}
		const char * weap = spawnArgs.GetString( va( "def_weapon%d", w ) );
		if ( !spawnArgs.GetBool( va( "weapon%d_cycle", w ) ) ) {
			continue;
		}
		if ( !weap[ 0 ] ) {
			continue;
		}
		if ( inventory.HasAmmo( weap, true, this ) || w == weapon_bloodstone ) {
			break;
		}
	}

	if ( ( w != currentWeapon ) && ( w != idealWeapon ) ) {
		idealWeapon = w;
		weaponSwitchTime = gameLocal.time + WEAPON_SWITCH_DELAY;
		UpdateHudWeapon();
	}
}

/*
===============
idPlayer::SelectWeapon
===============
*/
void CBasePlayer::SelectWeapon( int num, bool force )
{
	const char *weap;

	if ( !weaponEnabled || spectating || gameLocal.inCinematic || health < 0 ) {
		return;
	}

	if ( ( num < 0 ) || ( num >= MAX_WEAPONS ) ) {
		return;
	}

	if ( num == weapon_flashlight ) {
		return;
	}

	if ( ( num != weapon_pda ) && gameLocal.world->spawnArgs.GetBool( "no_Weapons" ) ) {
		num = weapon_fists;
		hiddenWeapon ^= 1;
		if ( hiddenWeapon && weapon.GetEntity() ) {
			weapon.GetEntity()->LowerWeapon();
		} else {
			weapon.GetEntity()->RaiseWeapon();
		}
	}	

	//Is the weapon a toggle weapon
	WeaponToggle_t* weaponToggle;
	if(weaponToggles.Get(va("weapontoggle%d", num), &weaponToggle)) {

		int weaponToggleIndex = 0;

		//Find the current Weapon in the list
		int currentIndex = -1;
		for(int i = 0; i < weaponToggle->toggleList.Num(); i++) {
			if(weaponToggle->toggleList[i] == idealWeapon) {
				currentIndex = i;
				break;
			}
		}
		if ( currentIndex == -1 ) {
			//Didn't find the current weapon so select the first item
			weaponToggleIndex = weaponToggle->lastUsed;
		} else {
			//Roll to the next available item in the list
			weaponToggleIndex = currentIndex;
			weaponToggleIndex++;
			if(weaponToggleIndex >= weaponToggle->toggleList.Num()) {
				weaponToggleIndex = 0;
			}
		}

		for(int i = 0; i < weaponToggle->toggleList.Num(); i++) {
			int weapNum = weaponToggle->toggleList[weaponToggleIndex];
			//Is it available
			if(inventory.weapons & ( 1 << weapNum)) {
				//Do we have ammo for it
				if ( inventory.HasAmmo( spawnArgs.GetString( va( "def_weapon%d", weapNum ) ), true, this ) || spawnArgs.GetBool( va( "weapon%d_allowempty", weapNum ) ) ) {
					break;
				}
			}

			weaponToggleIndex++;
			if(weaponToggleIndex >= weaponToggle->toggleList.Num()) {
				weaponToggleIndex = 0;
			}
		}
		weaponToggle->lastUsed = weaponToggleIndex;
		num = weaponToggle->toggleList[weaponToggleIndex];
	}

	weap = spawnArgs.GetString( va( "def_weapon%d", num ) );
	if ( !weap[ 0 ] ) {
		gameLocal.Printf( "Invalid weapon\n" );
		return;
	}

	if ( force || ( inventory.weapons & ( 1 << num ) ) ) {
		if ( !inventory.HasAmmo( weap, true, this ) && !spawnArgs.GetBool( va( "weapon%d_allowempty", num ) ) ) {
			return;
		}
		if ( ( previousWeapon >= 0 ) && ( idealWeapon == num ) && ( spawnArgs.GetBool( va( "weapon%d_toggle", num ) ) ) ) {
			weap = spawnArgs.GetString( va( "def_weapon%d", previousWeapon ) );
			if ( !inventory.HasAmmo( weap, true, this ) && !spawnArgs.GetBool( va( "weapon%d_allowempty", previousWeapon ) ) ) {
				return;
			}
			idealWeapon = previousWeapon;
		} else if ( ( weapon_pda >= 0 ) && ( num == weapon_pda ) && ( inventory.pdas.Num() == 0 ) ) {
			ShowTip( spawnArgs.GetString( "text_infoTitle" ), spawnArgs.GetString( "text_noPDA" ), true );
			return;
		} else {
			idealWeapon = num;
		}
		UpdateHudWeapon();
	}
}

/*
===============
idPlayer::GiveObjective
===============
*/
void CBasePlayer::GiveObjective( const char * title, const char * text, const idMaterial * screenshot )
{
	idObjectiveInfo & info = inventory.objectiveNames.Alloc();
	info.title = title;
	info.text = text;
	info.screenshot = screenshot;

	StartSound( "snd_objectiveup", SND_CHANNEL_ANY, 0, false, NULL );

	if ( hud ) {
		hud->SetupObjective( title, text, screenshot );
		hud->ShowObjective( false );
		objectiveUp = true;
	}
}

/*
===============
idPlayer::CompleteObjective
===============
*/
void CBasePlayer::CompleteObjective( const char *title )
{
	int c = inventory.objectiveNames.Num();
	for ( int i = 0;  i < c; i++ ) {
		if ( idStr::Icmp(inventory.objectiveNames[i].title, title) == 0 ) {
			inventory.objectiveNames.RemoveIndex( i );
			break;
		}
	}

	StartSound( "snd_objectiveup", SND_CHANNEL_ANY, 0, false, NULL );
	
	if ( hud ) {
		hud->SetupObjectiveComplete( title );
		hud->ShowObjective( true );
	}	
}

/*
===============
idPlayer::GiveItem

Returns false if the item shouldn't be picked up
===============
*/
bool CBasePlayer::GiveItem( idItem *item, unsigned int giveFlags )
{
	int					i;
	const idKeyValue	*arg;
	idDict				attr;
	bool				gave;
	int					numPickup;

	if ( common->IsMultiplayer() && spectating ) {
		return false;
	}

	if ( idStr::FindText( item->GetName(), "weapon_flashlight_new" ) > -1 ) {
		return false;
	}

	if ( idStr::FindText( item->GetName(), "weapon_flashlight" ) > -1 ) {
		// don't allow flashlight weapon unless classic mode is enabled
		return false;
	}

	item->GetAttributes( attr );
	
	gave = false;
	numPickup = inventory.pickupItemNames.Num();
	for( i = 0; i < attr.GetNumKeyVals(); i++ ) {
		arg = attr.GetKeyVal( i );
		if ( Give( arg->GetKey(), arg->GetValue(), giveFlags ) ) {
			gave = true;
		}
	}

	if ( giveFlags & ITEM_GIVE_FEEDBACK ) {
		arg = item->spawnArgs.MatchPrefix( "inv_weapon", NULL );
		if ( arg ) {
			// We need to update the weapon hud manually, but not
			// the armor/ammo/health because they are updated every
			// frame no matter what
			UpdateHudWeapon( false );
		}

		// display the pickup feedback on the hud
		if ( gave && ( numPickup == inventory.pickupItemNames.Num() ) ) {
			inventory.AddPickupName( item->spawnArgs.GetString( "inv_name" ), this ); //_D3XP
		}
	}

	return gave;
}

/*
===============
idPlayer::FireWeapon
===============
*/
idCVar g_infiniteAmmo( "g_infiniteAmmo", "0", CVAR_GAME | CVAR_BOOL, "infinite ammo" );
extern idCVar ui_autoSwitch;
void CBasePlayer::FireWeapon()
{
	idMat3 axis;
	idVec3 muzzle;

	if ( privateCameraView ) {
		return;
	}

	if ( g_editEntityMode.GetInteger() ) {
		GetViewPos( muzzle, axis );
		if ( gameLocal.editEntities->SelectEntity( muzzle, axis[0], this ) ) {
			return;
		}
	}

	if ( !hiddenWeapon && myCurrentWeapon.GetEntity()->IsReady() ) {
		if ( g_infiniteAmmo.GetBool() || myCurrentWeapon.GetEntity()->AmmoInClip() || myCurrentWeapon.GetEntity()->AmmoAvailable() ) {
			AI_ATTACK_HELD = true;
			myCurrentWeapon.GetEntity()->BeginAttack();
			if ( ( weapon_soulcube >= 0 ) && ( currentWeapon == weapon_soulcube ) ) {
				if ( hud ) {
					hud->UpdateSoulCube( false );
				}
				SelectWeapon( previousWeapon, false );
			}
			if( (weapon_bloodstone >= 0) && (currentWeapon == weapon_bloodstone) && inventory.weapons & ( 1 << weapon_bloodstone_active1 ) && myCurrentWeapon.GetEntity()->GetStatus() == WP_READY) {
				// tell it to switch to the previous weapon. Only do this once to prevent
				// weapon toggling messing up the previous weapon
				if(idealWeapon == weapon_bloodstone) {
					if(previousWeapon == weapon_bloodstone || previousWeapon == -1) {
						NextBestWeapon();
					} else {
						//Since this is a toggle weapon just select itself and it will toggle to the last weapon
						SelectWeapon( weapon_bloodstone, false );
					}
				}
			}
		} else {

			idLobbyBase & lobby = session->GetActingGameStateLobbyBase();
			lobbyUserID_t & lobbyUserID = gameLocal.lobbyUserIDs[ entityNumber ];
			bool autoSwitch = lobby.GetLobbyUserWeaponAutoSwitch( lobbyUserID );
			if ( !autoSwitch ) {
				return;
			}

			// update our ammo clip in our inventory
			if ( ( currentWeapon >= 0 ) && ( currentWeapon < MAX_WEAPONS ) ) {
				inventory.SetClipAmmoForWeapon( currentWeapon, weapon.GetEntity()->AmmoInClip() );
			}

			NextBestWeapon();
		}
	}

	
	if ( tipUp ) {
		HideTip();
	}
	
	if ( objectiveUp ) {
		HideObjective();
	}
}

/*
===============
idPlayer::StopFiring
===============
*/
void CBasePlayer::StopFiring()
{
	AI_ATTACK_HELD	= false;
	AI_WEAPON_FIRED = false;
	AI_RELOAD		= false;
	if ( weapon.GetEntity() ) {
		weapon.GetEntity()->EndAttack();
	}
}

/*
===========
idPlayer::SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
void CBasePlayer::SelectInitialSpawnPoint( idVec3 &origin, idAngles &angles )
{
	CBaseEntity *spot;
	idStr skin;

	spot = gameLocal.SelectInitialSpawnPoint( this );

	// set the player skin from the spawn location
	if ( spot->spawnArgs.GetString( "skin", NULL, skin ) ) {
		spawnArgs.Set( "spawn_skin", skin );
	}

	// activate the spawn locations targets
	spot->PostEventMS( &EV_ActivateTargets, 0, this );

	origin = spot->GetPhysics()->GetOrigin();
	origin[2] += 4.0f + CM_BOX_EPSILON;		// move up to make sure the player is at least an epsilon above the floor
	angles = spot->GetPhysics()->GetAxis().ToAngles();
}

/*
===========
idPlayer::SpawnFromSpawnSpot

Chooses a spawn location and spawns the player
============
*/
void CBasePlayer::SpawnFromSpawnSpot()
{
	idVec3		spawn_origin;
	idAngles	spawn_angles;
	
	SelectInitialSpawnPoint( spawn_origin, spawn_angles );
	SpawnToPoint( spawn_origin, spawn_angles );
}

/*
===========
idPlayer::SpawnToPoint

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState

when called here with spectating set to true, just place yourself and init
============
*/
void CBasePlayer::SpawnToPoint( const idVec3 &spawn_origin, const idAngles &spawn_angles )
{
	idVec3 spec_origin;

	respawning = true;

	Init();

	fl.noknockback = false;

	// stop any ragdolls being used
	StopRagdoll();

	// set back the player physics
	SetPhysics( &physicsObj );

	physicsObj.SetClipModelAxis();
	physicsObj.EnableClip();

	if ( !spectating ) {
		SetCombatContents( true );
	}

	physicsObj.SetLinearVelocity( vec3_origin );

	// setup our initial view
	if ( !spectating ) {
		SetOrigin( spawn_origin );
	} else {
		spec_origin = spawn_origin;
		spec_origin[ 2 ] += pm_normalheight.GetFloat();
		spec_origin[ 2 ] += SPECTATE_RAISE;
		SetOrigin( spec_origin );
	}

	// if this is the first spawn of the map, we don't have a usercmd yet,
	// so the delta angles won't be correct.  This will be fixed on the first think.
	viewAngles = ang_zero;
	SetDeltaViewAngles( ang_zero );
	SetViewAngles( spawn_angles );
	spawnAngles = spawn_angles;
	spawnAnglesSet = false;

	legsForward = true;
	legsYaw = 0.0f;
	idealLegsYaw = 0.0f;
	oldViewYaw = viewAngles.yaw;

	if ( spectating ) {
		Hide();
	} else {
		Show();
	}

	if ( common->IsMultiplayer() ) {
		if ( !spectating ) {
			// we may be called twice in a row in some situations. avoid a double fx and 'fly to the roof'
			if ( lastTeleFX < gameLocal.time - 1000 ) {
				idEntityFx::StartFx( spawnArgs.GetString( "fx_spawn" ), &spawn_origin, NULL, this, true );
				lastTeleFX = gameLocal.time;
			}
		}
		AI_TELEPORT = true;
	} else {
		AI_TELEPORT = false;
	}

	// kill anything at the new position
	if ( !spectating ) {
		physicsObj.SetClipMask( MASK_PLAYERSOLID ); // the clip mask is usually maintained in Move(), but KillBox requires it
		gameLocal.KillBox( this );
	}

	// don't allow full run speed for a bit
	physicsObj.SetKnockBack( 100 );

	// set our respawn time and buttons so that if we're killed we don't respawn immediately
	minRespawnTime = gameLocal.time;
	maxRespawnTime = gameLocal.time;
	if ( !spectating ) {
		forceRespawn = false;
	}

	Respawn_Shared();

	privateCameraView = NULL;

	BecomeActive( TH_THINK );

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	Think();

	respawning			= false;
	lastManOver			= false;
	lastManPlayAgain	= false;
	isTelefragged		= false;
}

/*
===============
idPlayer::ServerSpectate
================
*/
void CBasePlayer::ServerSpectate( bool spectate )
{
	if ( spectating != spectate )
	{
		Spectate( spectate );
		if ( !spectate ) {
			// When coming out of spectate, join the team with the least number of players
			if ( gameLocal.mpGame.IsGametypeTeamBased() ) {
				int teamCounts[2] = { 0, 0 };
				gameLocal.mpGame.NumActualClients( false, teamCounts );
				teamCounts[team]--;
				if ( teamCounts[0] < teamCounts[1] ) {
					team = 0;
				} else if ( teamCounts[1] < teamCounts[0] ) {
					team = 1;
				}
				gameLocal.mpGame.SwitchToTeam( entityNumber, -1, team );
			}
			if ( gameLocal.gameType == GAME_DM ) {
				// make sure the scores are reset so you can't exploit by spectating and entering the game back
				// other game types don't matter, as you either can't join back, or it's team scores
				gameLocal.mpGame.ClearFrags( entityNumber );
			}
		}
	}
	if ( !spectate ) {
		SpawnFromSpawnSpot();
	}

	// drop the flag if player was carrying it
	if ( spectate && common->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased() && 
		 carryingFlag )
	{
		DropFlag();
	}
}

/*
===============
idPlayer::ShowTip
===============
*/
void CBasePlayer::ShowTip( const char *title, const char *tip, bool autoHide )
{
	if ( tipUp ) {
		return;
	}

	if ( hudManager ) {
		hudManager->ShowTip( title, tip, autoHide );		
	}
	tipUp = true;
}

/*
===============
idPlayer::HideTip
===============
*/
void CBasePlayer::HideTip()
{
	if ( hudManager ) {
		hudManager->HideTip();		
	}
	tipUp = false;
}

/*
============
ServerDealDamage

Only called on the server and in singleplayer. This is where
the player's health is actually modified, but the visual and
sound effects happen elsewhere so that clients can get instant
feedback and hide lag.
============
*/
void CBasePlayer::ServerDealDamage( int damage, idEntity & inflictor, idEntity & attacker, const idVec3 & dir, const char * damageDefName, const int location  )
{
	const idDeclEntityDef *damageDef = gameLocal.FindEntityDef( damageDefName, false );
	if ( !damageDef ) {
		gameLocal.Warning( "Unknown damageDef '%s'", damageDefName );
		return;
	}

	// move the world direction vector to local coordinates
	idVec3		damage_from;
	idVec3		localDamageVector;	

	damage_from = dir;
	damage_from.Normalize();
	
	viewAxis.ProjectVector( damage_from, localDamageVector );

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if ( health > 0 ) {
		playerView.DamageImpulse( localDamageVector, &damageDef->dict );
	}

	// do the damage
	if ( damage > 0 ) {
		GetAchievementManager().SetPlayerTookDamage( true );

		int oldHealth = health;
		health -= damage;

		if ( health <= 0 ) {

			if ( health < -999 ) {
				health = -999;
			}

			// HACK - A - LICIOUS - Check to see if we are being damaged by the frag chamber.
			if ( oldHealth > 0 && strcmp( gameLocal.GetMapName(), "maps/game/mp/d3dm3.map" ) == 0 && strcmp( damageDefName, "damage_triggerhurt_1000_chamber" ) == 0 ) {
				idPlayer * fragChamberActivator = gameLocal.playerActivateFragChamber;
				if ( fragChamberActivator != NULL ) {
					fragChamberActivator->GetAchievementManager().EventCompletesAchievement( ACHIEVEMENT_MP_CATCH_ENEMY_IN_ROFC );
				}
				gameLocal.playerActivateFragChamber = NULL;
			}

			isTelefragged = damageDef->dict.GetBool( "telefrag" );

			lastDmgTime = gameLocal.time;
			Killed( &inflictor, &attacker, damage, dir, location );
		} else {
			if ( !g_testDeath.GetBool() ) {
				lastDmgTime = gameLocal.time;
			}
		}
	} else {
		// don't accumulate impulses
		if ( af.IsLoaded() ) {
			// clear impacts
			af.Rest();

			// physics is turned off by calling af.Rest()
			BecomeActive( TH_PHYSICS );
		}
	}
}

// idtech4 imports end

void CBasePlayer::PainSound()
{
	float rs;

	if (self.health < 0)
		return;

	if (damage_attacker.classname == "teledeath")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};

// water pain sounds
	if (self.watertype == CONTENT_WATER && self.waterlevel == 3)
	{
		DeathBubbles(1);
		if (random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/drown1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/drown2.wav", 1, ATTN_NORM);
		return;
	};

// slime pain sounds
	if (self.watertype == CONTENT_SLIME)
	{
// FIX ME       put in some steam here
		if (random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn2.wav", 1, ATTN_NORM);
		return;
	};

	if (self.watertype == CONTENT_LAVA)
	{
		if (random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn2.wav", 1, ATTN_NORM);
		return;
	};

	if (self.pain_finished > time)
	{
		self.axhitme = 0;
		return;
	};
	
	self.pain_finished = time + 0.5;

// don't make multiple pain sounds right after each other

// ax pain sound
	if (self.axhitme == 1)
	{
		self.axhitme = 0;
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/axhit1.wav", 1, ATTN_NORM);
		return;
	};

	rs = rint((random() * 5) + 1);

	self.noise = "";
	if (rs == 1)
		self.noise = "player/pain1.wav";
	else if (rs == 2)
		self.noise = "player/pain2.wav";
	else if (rs == 3)
		self.noise = "player/pain3.wav";
	else if (rs == 4)
		self.noise = "player/pain4.wav";
	else if (rs == 5)
		self.noise = "player/pain5.wav";
	else
		self.noise = "player/pain6.wav";

	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise, 1, ATTN_NORM);
	return;
};

void()  player_pain1 =  [       $pain1, player_pain2    ] {PainSound();self.weaponframe=0;};
void()  player_pain2 =  [       $pain2, player_pain3    ] {};
void()  player_pain3 =  [       $pain3, player_pain4    ] {};
void()  player_pain4 =  [       $pain4, player_pain5    ] {};
void()  player_pain5 =  [       $pain5, player_pain6    ] {};
void()  player_pain6 =  [       $pain6, player_run      ] {};

void()  player_axpain1 =        [       $axpain1,       player_axpain2  ] {PainSound();self.weaponframe=0;};
void()  player_axpain2 =        [       $axpain2,       player_axpain3  ] {};
void()  player_axpain3 =        [       $axpain3,       player_axpain4  ] {};
void()  player_axpain4 =        [       $axpain4,       player_axpain5  ] {};
void()  player_axpain5 =        [       $axpain5,       player_axpain6  ] {};
void()  player_axpain6 =        [       $axpain6,       player_run      ] {};

void player_pain()
{
	if (self.weaponframe)
		return;

	if (self.invisible_finished > time)
		return;         // eyes don't have pain frames

	if (self.weapon == IT_AXE)
		player_axpain1 ();
	else
		player_pain1 ();
};

void player_diea1();
void player_dieb1();
void player_diec1();
void player_died1();
void player_diee1();
void player_die_ax1();

void DeathBubblesSpawn()
{
	entity    bubble;
	
	if (self.owner.waterlevel != 3)
		return;
	
	bubble = spawn();
	setmodel (bubble, "sprites/s_bubble.spr");
	setorigin (bubble, self.owner.origin + '0 0 24');
	bubble.movetype = MOVETYPE_NOCLIP;
	bubble.solid = SOLID_NOT;
	bubble.velocity = '0 0 15';
	bubble.nextthink = time + 0.5;
	bubble.think = bubble_bob;
	bubble.classname = "bubble";
	bubble.frame = 0;
	bubble.cnt = 0;
	setsize (bubble, '-8 -8 -8', '8 8 8');
	self.nextthink = time + 0.1;
	self.think = DeathBubblesSpawn;
	self.air_finished = self.air_finished + 1;
	if (self.air_finished >= self.bubble_count)
		remove(self);
};

void CBasePlayer::DeathBubbles(float num_bubbles)
{
	entity    bubble_spawner;
	
	bubble_spawner = spawn();
	gpEngine->pfnSetOrigin (bubble_spawner, self.origin);
	bubble_spawner.movetype = MOVETYPE_NONE;
	bubble_spawner.solid = SOLID_NOT;
	bubble_spawner.nextthink = time + 0.1;
	bubble_spawner.SetThinkCallback(DeathBubblesSpawn);
	bubble_spawner.air_finished = 0;
	bubble_spawner.owner = self;
	bubble_spawner.bubble_count = num_bubbles;
};

void CBasePlayer::DeathSound()
{
	float rs;

	// water death sounds
	if (self.waterlevel == 3)
	{
		DeathBubbles(5);
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/h2odeath.wav", 1, ATTN_NONE);
		return;
	};
	
	rs = rint ((random() * 4) + 1);
	if (rs == 1)
		self.noise = "player/death1.wav";
	if (rs == 2)
		self.noise = "player/death2.wav";
	if (rs == 3)
		self.noise = "player/death3.wav";
	if (rs == 4)
		self.noise = "player/death4.wav";
	if (rs == 5)
		self.noise = "player/death5.wav";

	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise, 1, ATTN_NONE);
	return;
};

void CBasePlayer::PlayerDead()
{
	self.nextthink = -1;
// allow respawn after a certain time
	self.deadflag = DEAD_DEAD;
};

vector VelocityForDamage(float dm)
{
	vector v;

	if (vlen(damage_inflictor.velocity)>0)
	{
		v = 0.5 * damage_inflictor.velocity;
		v = v + (25 * normalize(self.origin-damage_inflictor.origin));
		v_z = 100 + 240 * random();
		v_x = v_x + (200 * crandom());
		v_y = v_y + (200 * crandom());
		//dprint ("Velocity gib\n");                
	}
	else
	{
		v_x = 100 * crandom();
		v_y = 100 * crandom();
		v_z = 200 + 100 * random();
	};

	//v_x = 100 * crandom();
	//v_y = 100 * crandom();
	//v_z = 200 + 100 * random();

	if (dm > -50)
	{
	//      dprint ("level 1\n");
		v = v * 0.7;
	}
	else if (dm > -200)
	{
	//      dprint ("level 3\n");
		v = v * 2;
	}
	else
		v = v * 10;

	return v;
};

void CBasePlayer::ThrowGib(string gibname, float dm)
{
	entity new;

	new = spawn();
	new.origin = self.origin;
	gpEngine->pfnSetModel (new, gibname);
	gpEngine->pfnSetSize (new, '0 0 0', '0 0 0');
	new.velocity = VelocityForDamage (dm);
	new.movetype = MOVETYPE_BOUNCE;
	new.solid = SOLID_NOT;
	new.avelocity_x = random()*600;
	new.avelocity_y = random()*600;
	new.avelocity_z = random()*600;
	new.think = SUB_Remove;
	new.ltime = time;
	new.nextthink = time + 10 + random()*10;
	new.frame = 0;
	new.flags = 0;
};

void CBasePlayer::ThrowHead(string gibname, float dm)
{
	gpEngine->pfnSetModel (self, gibname);
	self.frame = 0;
	self.nextthink = -1;
	self.movetype = MOVETYPE_BOUNCE;
	self.takedamage = DAMAGE_NO;
	self.solid = SOLID_NOT;
	self.view_ofs = '0 0 8';
	gpEngine->pfnSetSize (self, '-16 -16 0', '16 16 56');
	self.velocity = VelocityForDamage (dm);
	self.origin_z = self.origin_z - 24;
	self.flags = self.flags - (self.flags & FL_ONGROUND);
	self.avelocity = crandom() * '0 600 0';
};

void CBasePlayer::GibPlayer()
{
	ThrowHead ("models/h_player.mdl", self.health);
	ThrowGib ("models/gib1.mdl", self.health);
	ThrowGib ("models/gib2.mdl", self.health);
	ThrowGib ("models/gib3.mdl", self.health);

	self.deadflag = DEAD_DEAD;

	if (damage_attacker.classname == "teledeath")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};

	if (damage_attacker.classname == "teledeath2")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};
		
	if (random() < 0.5)
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/gib.wav", 1, ATTN_NONE);
	else
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/udeath.wav", 1, ATTN_NONE);
};

void CBasePlayer::Die()
{
	float   i;
	string	s;
	
	self.items = self.items - (self.items & IT_INVISIBILITY);

	if ((stof(infokey(world,"dq"))) != 0)
	{
		if (self.super_damage_finished > 0)
		{
			DropQuad (self.super_damage_finished - time);
			bprint (PRINT_LOW, self.netname);
			if (deathmatch == 4)
				bprint (PRINT_LOW, " lost an OctaPower with ");
			else
				bprint (PRINT_LOW, " lost a quad with ");
			s = ftos(rint(self.super_damage_finished - time));
			bprint (PRINT_LOW, s);
			bprint (PRINT_LOW, " seconds remaining\n");
		};
	};

	if ((stof(infokey(world,"dr"))) != 0)
	{
		if (self.invisible_finished > 0)
		{
			bprint (PRINT_LOW, self.netname);
			bprint (PRINT_LOW, " lost a ring with ");
			s = ftos(rint(self.invisible_finished - time));
			bprint (PRINT_LOW, s);
			bprint (PRINT_LOW, " seconds remaining\n");
			DropRing (self.invisible_finished - time);
		};
	};

	self.invisible_finished = 0;    // don't die as eyes
	self.invincible_finished = 0;
	self.super_damage_finished = 0;
	self.radsuit_finished = 0;
	self.modelindex = modelindex_player;    // don't use eyes

	DropBackpack();
	
	self.weaponmodel="";
	self.view_ofs = '0 0 -8';
	self.deadflag = DEAD_DYING;
	self.solid = SOLID_NOT;
	self.flags = self.flags - (self.flags & FL_ONGROUND);
	self.movetype = MOVETYPE_TOSS;
	if (self.velocity_z < 10)
		self.velocity_z = self.velocity_z + random()*300;

	if (self.health < -40)
	{
		GibPlayer ();
		return;
	};

	DeathSound();
	
	self.angles_x = 0;
	self.angles_z = 0;
	
	if (self.weapon == IT_AXE)
	{
		player_die_ax1 ();
		return;
	};
	
	i = cvar("temp1");
	if (!i)
		i = 1 + floor(random()*6);
	
	if (i == 1)
		player_diea1();
	else if (i == 2)
		player_dieb1();
	else if (i == 3)
		player_diec1();
	else if (i == 4)
		player_died1();
	else
		player_diee1();
};

/*
===============
DropBackpack
===============
*/
void CBasePlayer::DropBackpack()
{
	entvars_t item;

	if (!(self.ammo_shells + self.ammo_nails + self.ammo_rockets + self.ammo_cells))
		return; // nothing in it

	item = spawn();
	item.origin = self.origin - '0 0 24';
	
	item.items = self.weapon;
	if (item.items == IT_AXE)
		item.netname = "Axe";
	else if (item.items == IT_SHOTGUN)
		item.netname = "Shotgun";
	else if (item.items == IT_SUPER_SHOTGUN)
		item.netname = "Double-barrelled Shotgun";
	else if (item.items == IT_NAILGUN)
		item.netname = "Nailgun";
	else if (item.items == IT_SUPER_NAILGUN)
		item.netname = "Super Nailgun";
	else if (item.items == IT_GRENADE_LAUNCHER)
		item.netname = "Grenade Launcher";
	else if (item.items == IT_ROCKET_LAUNCHER)
		item.netname = "Rocket Launcher";
	else if (item.items == IT_LIGHTNING)
		item.netname = "Thunderbolt";
	else
		item.netname = "";

	item.ammo_shells = self.ammo_shells;
	item.ammo_nails = self.ammo_nails;
	item.ammo_rockets = self.ammo_rockets;
	item.ammo_cells = self.ammo_cells;

	item.velocity_z = 300;
	item.velocity_x = -100 + (random() * 200);
	item.velocity_y = -100 + (random() * 200);
	
	item.flags = FL_ITEM;
	item.solid = SOLID_TRIGGER;
	item.movetype = MOVETYPE_TOSS;
	gpEngine->pfnSetModel (item, "models/backpack.mdl");
	gpEngine->pfnSetSize (item, '-16 -16 0', '16 16 56');
	item.SetTouchCallback(BackpackTouch);
	
	item.nextthink = time + 120;    // remove after 2 minutes
	item.SetThinkCallback(SUB_Remove);
};

float CBasePlayer::GetBestWeapon()
{
	float it = self.items;

	if (self.waterlevel <= 1 && self.ammo_cells >= 1 && (it & IT_LIGHTNING) )
		return IT_LIGHTNING;
	else if(self.ammo_nails >= 2 && (it & IT_SUPER_NAILGUN) )
		return IT_SUPER_NAILGUN;
	else if(self.ammo_shells >= 2 && (it & IT_SUPER_SHOTGUN) )
		return IT_SUPER_SHOTGUN;
	else if(self.ammo_nails >= 1 && (it & IT_NAILGUN) )
		return IT_NAILGUN;
	else if(self.ammo_shells >= 1 && (it & IT_SHOTGUN)  )
		return IT_SHOTGUN;
		
/*
	if(self.ammo_rockets >= 1 && (it & IT_ROCKET_LAUNCHER) )
		return IT_ROCKET_LAUNCHER;
	else if(self.ammo_rockets >= 1 && (it & IT_GRENADE_LAUNCHER) )
		return IT_GRENADE_LAUNCHER;

*/

	return IT_AXE;
};

bool CBasePlayer::CheckNoAmmo()
{
	if(self.currentammo > 0)
		return true;

	if(self.weapon == IT_AXE)
		return true;
	
	self.weapon = GetBestWeapon();

	W_SetCurrentAmmo();
	
	// drop the weapon down
	return false;
};

void set_suicide_frame(entvars_t *self)
{
	// used by kill and diconnect commands
	if(self->model != "models/player.mdl")
		return; // already gibbed
	
	self->frame = $deatha11;
	self->solid = SOLID_NOT;
	self->movetype = MOVETYPE_TOSS;
	self->deadflag = DEAD_DEAD;
	self->nextthink = -1;
};

void()  player_diea1    =       [       $deatha1,       player_diea2    ] {};
void()  player_diea2    =       [       $deatha2,       player_diea3    ] {};
void()  player_diea3    =       [       $deatha3,       player_diea4    ] {};
void()  player_diea4    =       [       $deatha4,       player_diea5    ] {};
void()  player_diea5    =       [       $deatha5,       player_diea6    ] {};
void()  player_diea6    =       [       $deatha6,       player_diea7    ] {};
void()  player_diea7    =       [       $deatha7,       player_diea8    ] {};
void()  player_diea8    =       [       $deatha8,       player_diea9    ] {};
void()  player_diea9    =       [       $deatha9,       player_diea10   ] {};
void()  player_diea10   =       [       $deatha10,      player_diea11   ] {};
void()  player_diea11   =       [       $deatha11,      player_diea11 ] {PlayerDead();};

void()  player_dieb1    =       [       $deathb1,       player_dieb2    ] {};
void()  player_dieb2    =       [       $deathb2,       player_dieb3    ] {};
void()  player_dieb3    =       [       $deathb3,       player_dieb4    ] {};
void()  player_dieb4    =       [       $deathb4,       player_dieb5    ] {};
void()  player_dieb5    =       [       $deathb5,       player_dieb6    ] {};
void()  player_dieb6    =       [       $deathb6,       player_dieb7    ] {};
void()  player_dieb7    =       [       $deathb7,       player_dieb8    ] {};
void()  player_dieb8    =       [       $deathb8,       player_dieb9    ] {};
void()  player_dieb9    =       [       $deathb9,       player_dieb9    ] {PlayerDead();};

void()  player_diec1    =       [       $deathc1,       player_diec2    ] {};
void()  player_diec2    =       [       $deathc2,       player_diec3    ] {};
void()  player_diec3    =       [       $deathc3,       player_diec4    ] {};
void()  player_diec4    =       [       $deathc4,       player_diec5    ] {};
void()  player_diec5    =       [       $deathc5,       player_diec6    ] {};
void()  player_diec6    =       [       $deathc6,       player_diec7    ] {};
void()  player_diec7    =       [       $deathc7,       player_diec8    ] {};
void()  player_diec8    =       [       $deathc8,       player_diec9    ] {};
void()  player_diec9    =       [       $deathc9,       player_diec10   ] {};
void()  player_diec10   =       [       $deathc10,      player_diec11   ] {};
void()  player_diec11   =       [       $deathc11,      player_diec12   ] {};
void()  player_diec12   =       [       $deathc12,      player_diec13   ] {};
void()  player_diec13   =       [       $deathc13,      player_diec14   ] {};
void()  player_diec14   =       [       $deathc14,      player_diec15   ] {};
void()  player_diec15   =       [       $deathc15,      player_diec15 ] {PlayerDead();};

void()  player_died1    =       [       $deathd1,       player_died2    ] {};
void()  player_died2    =       [       $deathd2,       player_died3    ] {};
void()  player_died3    =       [       $deathd3,       player_died4    ] {};
void()  player_died4    =       [       $deathd4,       player_died5    ] {};
void()  player_died5    =       [       $deathd5,       player_died6    ] {};
void()  player_died6    =       [       $deathd6,       player_died7    ] {};
void()  player_died7    =       [       $deathd7,       player_died8    ] {};
void()  player_died8    =       [       $deathd8,       player_died9    ] {};
void()  player_died9    =       [       $deathd9,       player_died9    ] {PlayerDead();};

void()  player_diee1    =       [       $deathe1,       player_diee2    ] {};
void()  player_diee2    =       [       $deathe2,       player_diee3    ] {};
void()  player_diee3    =       [       $deathe3,       player_diee4    ] {};
void()  player_diee4    =       [       $deathe4,       player_diee5    ] {};
void()  player_diee5    =       [       $deathe5,       player_diee6    ] {};
void()  player_diee6    =       [       $deathe6,       player_diee7    ] {};
void()  player_diee7    =       [       $deathe7,       player_diee8    ] {};
void()  player_diee8    =       [       $deathe8,       player_diee9    ] {};
void()  player_diee9    =       [       $deathe9,       player_diee9    ] {PlayerDead();};

void()  player_die_ax1  =       [       $axdeth1,       player_die_ax2  ] {};
void()  player_die_ax2  =       [       $axdeth2,       player_die_ax3  ] {};
void()  player_die_ax3  =       [       $axdeth3,       player_die_ax4  ] {};
void()  player_die_ax4  =       [       $axdeth4,       player_die_ax5  ] {};
void()  player_die_ax5  =       [       $axdeth5,       player_die_ax6  ] {};
void()  player_die_ax6  =       [       $axdeth6,       player_die_ax7  ] {};
void()  player_die_ax7  =       [       $axdeth7,       player_die_ax8  ] {};
void()  player_die_ax8  =       [       $axdeth8,       player_die_ax9  ] {};
void()  player_die_ax9  =       [       $axdeth9,       player_die_ax9  ] {PlayerDead();};

bool CBasePlayer::Pickup_Weapon(edict_t *ent)
{
	int			index;
	gitem_t		*ammo;

	index = ITEM_INDEX(ent->item);

	if ( ( ((int)(dmflags->value) & DF_WEAPONS_STAY) || coop->value) 
		&& this->client->pers.inventory[index])
	{
		if (!(ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM) ) )
			return false;	// leave the weapon for others to pickup
	}

	this->client->pers.inventory[index]++;

	if (!(ent->spawnflags & DROPPED_ITEM) )
	{
		// give them some ammo with it
		ammo = FindItem (ent->item->ammo);
		if ( (int)dmflags->value & DF_INFINITE_AMMO )
			this->Add_Ammo (ammo, 1000);
		else
			this->Add_Ammo (ammo, ammo->quantity);

		if (! (ent->spawnflags & DROPPED_PLAYER_ITEM) )
		{
			if (deathmatch->value)
			{
				if ((int)(dmflags->value) & DF_WEAPONS_STAY)
					ent->flags |= FL_RESPAWN;
				else
					SetRespawn (ent, 30);
			}
			if (coop->value)
				ent->flags |= FL_RESPAWN;
		}
	}

	if (this->client->pers.weapon != ent->item && 
		(this->client->pers.inventory[index] == 1) &&
		( !deathmatch->value || this->client->pers.weapon == FindItem("blaster") ) )
		this->client->newweapon = ent->item;

	return true;
}

/*
===============
ChangeWeapon

The old weapon has been dropped all the way, so make the new one
current
===============
*/
void CBasePlayer::ChangeWeapon (edict_t *ent)
{
	int i;

	if (this->grenade_time)
	{
		this->grenade_time = level.time;
		this->weapon_sound = 0;
		weapon_grenade_fire (this, false);
		this->grenade_time = 0;
	};

	this->lastweapon = this->pers.weapon;
	this->weapon = this->newweapon;
	this->newweapon = nullptr;
	this->machinegun_shots = 0;

	// set visible model
	if (ent->s.modelindex == 255) {
		if (this->pers.weapon)
			i = ((this->pers.weapon->weapmodel & 0xff) << 8);
		else
			i = 0;
		ent->s.skinnum = (ent - g_edicts - 1) | i;
	}

	if (this->pers.weapon && this->pers.weapon->ammo)
		this->ammo_index = ITEM_INDEX(FindItem(this->pers.weapon->ammo));
	else
		this->ammo_index = 0;

	if (!this->pers.weapon)
	{	// dead
		this->ps.gunindex = 0;
		return;
	}

	this->weaponstate = WEAPON_ACTIVATING;
	this->ps.gunframe = 0;
	this->ps.gunindex = gi.modelindex(this->pers.weapon->view_model);

	this->anim_priority = ANIM_PAIN;
	if(this->ps.pmove.pm_flags & PMF_DUCKED)
	{
		ent->s.frame = FRAME_crpain1;
		this->anim_end = FRAME_crpain4;
	}
	else
	{
		ent->s.frame = FRAME_pain301;
		this->anim_end = FRAME_pain304;
	};
};

/*
=================
NoAmmoWeaponChange
=================
*/
void CBasePlayer::NoAmmoWeaponChange(edict_t *ent)
{
	if ( this->pers.inventory[ITEM_INDEX(FindItem("slugs"))]
		&&  this->pers.inventory[ITEM_INDEX(FindItem("railgun"))] )
	{
		this->newweapon = FindItem ("railgun");
		return;
	}
	if ( this->pers.inventory[ITEM_INDEX(FindItem("cells"))]
		&&  this->pers.inventory[ITEM_INDEX(FindItem("hyperblaster"))] )
	{
		this->newweapon = FindItem ("hyperblaster");
		return;
	}
	if ( this->pers.inventory[ITEM_INDEX(FindItem("bullets"))]
		&&  this->pers.inventory[ITEM_INDEX(FindItem("chaingun"))] )
	{
		this->newweapon = FindItem ("chaingun");
		return;
	}
	if ( this->pers.inventory[ITEM_INDEX(FindItem("bullets"))]
		&&  this->pers.inventory[ITEM_INDEX(FindItem("machinegun"))] )
	{
		this->newweapon = FindItem ("machinegun");
		return;
	}
	if ( this->pers.inventory[ITEM_INDEX(FindItem("shells"))] > 1
		&&  this->pers.inventory[ITEM_INDEX(FindItem("super shotgun"))] )
	{
		this->newweapon = FindItem ("super shotgun");
		return;
	}
	if ( this->pers.inventory[ITEM_INDEX(FindItem("shells"))]
		&&  this->pers.inventory[ITEM_INDEX(FindItem("shotgun"))] )
	{
		this->newweapon = FindItem ("shotgun");
		return;
	}
	this->newweapon = FindItem ("blaster");
}

/*
=================
Think_Weapon

Called by ClientBeginServerFrame and ClientThink
=================
*/
void CBasePlayer::Think_Weapon (edict_t *ent)
{
	// if just died, put the weapon away
	if (this->health < 1)
	{
		this->newweapon = nullptr;
		this->ChangeWeapon ();
	}

	// call active weapon think routine
	if (this->pers.weapon && this->pers.weapon->weaponthink)
	{
		is_quad = (this->quad_framenum > level.framenum);
		if (this->silencer_shots)
			is_silenced = MZ_SILENCED;
		else
			is_silenced = 0;
		this->pers.weapon->weaponthink (ent);
	}
}

/*
================
Use_Weapon

Make the weapon ready if there is ammo
================
*/
void CBasePlayer::Use_Weapon (gitem_t *item)
{
	int			ammo_index;
	gitem_t		*ammo_item;

	// see if we're already using it
	if (item == this->GetCurrentWeapon())
		return;

	if (item->ammo && !g_select_empty->value && !(item->flags & IT_AMMO))
	{
		ammo_item = FindItem(item->ammo);
		ammo_index = ITEM_INDEX(ammo_item);

		if (!this->pers.inventory[ammo_index])
		{
			gi.cprintf (ent, PRINT_HIGH, "No %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
			return;
		}

		if (this->pers.inventory[ammo_index] < item->quantity)
		{
			gi.cprintf (ent, PRINT_HIGH, "Not enough %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
			return;
		}
	}

	// change to this weapon when down
	this->newweapon = item;
};

/*
================
Drop_Weapon
================
*/
void CBasePlayer::Drop_Weapon(gitem_t *item)
{
	if ((int)(dmflags->value) & DF_WEAPONS_STAY)
		return;

	int index = ITEM_INDEX(item);
	
	// see if we're already using it
	if ( ((item == this->GetCurrentWeapon()) || (item == this->newweapon)) && (this->pers.inventory[index] == 1) )
	{
		gi.cprintf (this, PRINT_HIGH, "Can't drop current weapon\n");
		return;
	};

	Drop_Item (this, item);
	this->pers.inventory[index]--;
}

/*
===============
idPlayer::Reload
===============
*/
// from idtech4
void CBasePlayer::Reload()
{
	if ( spectating || gameLocal.inCinematic || influenceActive )
		return;

	if ( common->IsClient() && !IsLocallyControlled() )
		return;

	if ( myCurrentWeapon.GetEntity() && myCurrentWeapon.GetEntity()->IsLinked() )
		myCurrentWeapon.GetEntity()->Reload();
};

/*
===============
idPlayer::RaiseWeapon
===============
*/
void CBasePlayer::RaiseWeapon()
{
	if ( myCurrentWeapon.GetEntity() && myCurrentWeapon.GetEntity()->IsHidden() )
		myCurrentWeapon.GetEntity()->RaiseWeapon();
};

/*
===============
idPlayer::LowerWeapon
===============
*/
void CBasePlayer::LowerWeapon()
{
	if ( myCurrentWeapon.GetEntity() && !myCurrentWeapon.GetEntity()->IsHidden() )
		myCurrentWeapon.GetEntity()->LowerWeapon();
};

/*
===============
idPlayer::FlashlightOn
===============
*/
void CBasePlayer::FlashlightOn()
{
	if ( !flashlight.IsValid() )
		return;

	if ( flashlightBattery < idMath::Ftoi( flashlight_minActivatePercent.GetFloat() * flashlight_batteryDrainTimeMS.GetFloat() ) )
		return;

	if ( gameLocal.inCinematic )
		return;

	if ( flashlight.GetEntity()->lightOn )
		return;

	if ( health <= 0 )
		return;

	if ( spectating )
		return;

	flashlight->FlashlightOn();
};

/*
===============
idPlayer::FlashlightOff
===============
*/
void CBasePlayer::FlashlightOff()
{
	if ( !flashlight.IsValid() )
		return;
	
	if ( !flashlight.GetEntity()->lightOn )
		return;
	
	flashlight->FlashlightOff();
};

/*
===============
FireWeapon
===============
*/
// from Q3
void CBasePlayer::FireWeapon( gentity_t *ent )
{
	if (this->ps.powerups[PW_QUAD] )
		s_quadFactor = g_quadfactor.value;
	else
		s_quadFactor = 1;
	
#ifdef MISSIONPACK
	if( this->persistantPowerup && this->persistantPowerup->item && this->persistantPowerup->item->giTag == PW_DOUBLER )
		s_quadFactor *= 2;
#endif

	// track shots taken for accuracy tracking.  Grapple is not a weapon and gauntet is just not tracked
	if( ent->s.weapon != WP_GRAPPLING_HOOK && ent->s.weapon != WP_GAUNTLET ) {
#ifdef MISSIONPACK
		if( ent->s.weapon == WP_NAILGUN ) {
			this->accuracy_shots += NUM_NAILSHOTS;
		} else {
			this->accuracy_shots++;
		}
#else
		this->accuracy_shots++;
#endif
	};

	// set aiming directions
	AngleVectors (this->ps.viewangles, forward, right, up);

	CalcMuzzlePointOrigin ( ent, this->oldOrigin, forward, right, up, muzzle );

	// fire the specific weapon
	switch( ent->s.weapon ) // this->GetCurrentWeapon()
	//this->GetCurrentWeapon()->Fire();
	{
	case WP_GAUNTLET:
		Weapon_Gauntlet( ent );
		break;
	case WP_LIGHTNING:
		Weapon_LightningFire( ent );
		break;
	case WP_SHOTGUN:
		weapon_supershotgun_fire( ent );
		break;
	case WP_MACHINEGUN:
		if ( g_gametype.integer != GT_TEAM )
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_DAMAGE );
		else
			Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_TEAM_DAMAGE );
		break;
	case WP_GRENADE_LAUNCHER:
		weapon_grenadelauncher_fire( ent );
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_RocketLauncher_Fire( ent );
		break;
	case WP_PLASMAGUN:
		Weapon_Plasmagun_Fire( ent );
		break;
	case WP_RAILGUN:
		weapon_railgun_fire( ent );
		break;
	case WP_BFG:
		BFG_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
	case WP_NAILGUN:
		Weapon_Nailgun_Fire( ent );
		break;
	case WP_PROX_LAUNCHER:
		weapon_proxlauncher_fire( ent );
		break;
	case WP_CHAINGUN:
		Bullet_Fire( ent, CHAINGUN_SPREAD, MACHINEGUN_DAMAGE );
		break;
	default:
// FIXME		G_Error( "Bad ent->s.weapon" );
		break;
	};
};

void CBasePlayer::PlayerJump(edict_t *self)
{
	vector start, end;

	if (self->v.flags & FL_WATERJUMP)
		return;
	
	if (self->v.waterlevel >= 2)
	{
// play swiming sound
		if (self->v.swim_flag < time)
		{
			self->v.swim_flag = time + 1;
			if (random() < 0.5)
				gpEngine->pfnEmitSound(self, CHAN_BODY, "misc/water1.wav", 1, ATTN_NORM);
			else
				gpEngine->pfnEmitSound(self, CHAN_BODY, "misc/water2.wav", 1, ATTN_NORM);
		}

		return;
	}

	if (!(self->v.flags & FL_ONGROUND))
		return;

	if ( !(self->v.flags & FL_JUMPRELEASED) )
		return;         // don't pogo stick

	self->v.flags = self->v.flags - (self->v.flags & FL_JUMPRELEASED);       
	self->v.button2 = 0;

// player jumping sound
	gpEngine->pfnEmitSound(self, CHAN_BODY, "player/plyrjmp8.wav", 1, ATTN_NORM);
};

/*
===========
WaterMove

============
*/
.float  dmgtime;

void CBasePlayer::WaterMove(edict_t *self)
{
//dprint (ftos(self.waterlevel));
	if (self->GetMoveType() == MOVETYPE_NOCLIP)
		return;
	
	if (self->GetHealth() < 0)
		return;

	if (self->v.waterlevel != 3)
	{
		if (self->v.air_finished < time)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/gasp2.wav", 1, ATTN_NORM);
		else if (self->v.air_finished < time + 9)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/gasp1.wav", 1, ATTN_NORM);
		self->v.air_finished = time + 12;
		self->v.dmg = 2;
	}
	else if (self->v.air_finished < time)
	{       // drown!
		if (self->v.pain_finished < time)
		{
			self->v.dmg = self->v.dmg + 2;
			if (self->v.dmg > 15)
				self->v.dmg = 10;
			T_Damage (self, world, world, self->v.dmg);
			self->v.pain_finished = time + 1;
		}
	}
	
	if (!self->v.waterlevel)
	{
		if (self->v.flags & FL_INWATER)
		{       
			// play leave water sound
			gpEngine->pfnEmitSound (self, CHAN_BODY, "misc/outwater.wav", 1, ATTN_NORM);
			self->v.flags = self->v.flags - FL_INWATER;
		}
		return;
	}

	if (self->v.watertype == CONTENT_LAVA)
	{       // do damage
		if (self->v.dmgtime < time)
		{
			if (self->v.radsuit_finished > time)
				self->v.dmgtime = time + 1;
			else
				self->v.dmgtime = time + 0.2;

			T_Damage (self, world, world, 10*self->v.waterlevel);
		}
	}
	else if (self->v.watertype == CONTENT_SLIME)
	{       // do damage
		if (self->v.dmgtime < time && self->v.radsuit_finished < time)
		{
			self->v.dmgtime = time + 1;
			T_Damage (self, world, world, 4*self->v.waterlevel);
		}
	}
	
	if ( !(self->v.flags & FL_INWATER) )
	{       

// player enter water sound

		if (self->v.watertype == CONTENT_LAVA)
			gpEngine->pfnEmitSound(self, CHAN_BODY, "player/inlava.wav", 1, ATTN_NORM);
		if (self->v.watertype == CONTENT_WATER)
			gpEngine->pfnEmitSound(self, CHAN_BODY, "player/inh2o.wav", 1, ATTN_NORM);
		if (self->v.watertype == CONTENT_SLIME)
			gpEngine->pfnEmitSound(self, CHAN_BODY, "player/slimbrn2.wav", 1, ATTN_NORM);

		self->v.flags += FL_INWATER;
		self->v.dmgtime = 0;
	}       
};

void CBasePlayer::CheckWaterJump(edict_t *self)
{
	vector start, end;

// check for a jump-out-of-water
	makevectors (self->v.angles);
	start = self->v.origin;
	start_z = start_z + 8; 
	v_forward_z = 0;
	normalize(v_forward);
	end = start + v_forward*24;
	traceline (start, end, TRUE, self);
	if (trace_fraction < 1)
	{       // solid at waist
		start_z = start_z + self->v.maxs_z - 8;
		end = start + v_forward*24;
		self->v.movedir = trace_plane_normal * -50;
		traceline (start, end, TRUE, self);
		if (trace_fraction == 1)
		{       // open at eye level
			self->v.flags |= FL_WATERJUMP;
			self->v.velocity_z = 225;
			self->v.flags = self->v.flags - (self->v.flags & FL_JUMPRELEASED);
			self->v.teleport_time = time + 2;  // safety net
			return;
		};
	};
};