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

$cd / raid / quake / id1 / models / player_4
                                    $origin 0 -
6 24 $base base
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

void
player_run();

void() player_stand1 = [$axstnd1, player_stand1] {
	self.weaponframe = 0;
	if(self.velocity_x || self.velocity_y)
	{
		self.walkframe = 0;
		player_run();
		return;
	}

	if(self.weapon == IT_AXE)
	{
		if(self.walkframe >= 12)
			self.walkframe = 0;
		self.frame = $axstnd1 + self.walkframe;
	}
	else
	{
		if(self.walkframe >= 5)
			self.walkframe = 0;
		self.frame = $stand1 + self.walkframe;
	}
	self.walkframe = self.walkframe + 1;
};

void() player_run = [$rockrun1, player_run] {
	self.weaponframe = 0;
	if(!self.velocity_x && !self.velocity_y)
	{
		self.walkframe = 0;
		player_stand1();
		return;
	}

	if(self.weapon == IT_AXE)
	{
		if(self.walkframe == 6)
			self.walkframe = 0;
		self.frame = $axrun1 + self.walkframe;
	}
	else
	{
		if(self.walkframe == 6)
			self.walkframe = 0;
		self.frame = self.frame + self.walkframe;
	}
	self.walkframe = self.walkframe + 1;
};

void muzzleflash()
{
	WriteByte(MSG_MULTICAST, SVC_MUZZLEFLASH);
	WriteEntity(MSG_MULTICAST, self);
	multicast(self.origin, MULTICAST_PVS);
};

void() player_shot1 = [$shotatt1, player_shot2] {self.weaponframe=1;muzzleflash(); };
void() player_shot2 = [$shotatt2, player_shot3] { self.weaponframe = 2; };
void() player_shot3 = [$shotatt3, player_shot4] { self.weaponframe = 3; };
void() player_shot4 = [$shotatt4, player_shot5] { self.weaponframe = 4; };
void() player_shot5 = [$shotatt5, player_shot6] { self.weaponframe = 5; };
void() player_shot6 = [$shotatt6, player_run] { self.weaponframe = 6; };

void() player_axe1 = [$axatt1, player_axe2] { self.weaponframe = 1; };
void() player_axe2 = [$axatt2, player_axe3] { self.weaponframe = 2; };
void() player_axe3 = [$axatt3, player_axe4] {self.weaponframe=3;W_FireAxe(); };
void() player_axe4 = [$axatt4, player_run] { self.weaponframe = 4; };

void() player_axeb1 = [$axattb1, player_axeb2] { self.weaponframe = 5; };
void() player_axeb2 = [$axattb2, player_axeb3] { self.weaponframe = 6; };
void() player_axeb3 = [$axattb3, player_axeb4] {self.weaponframe=7;W_FireAxe(); };
void() player_axeb4 = [$axattb4, player_run] { self.weaponframe = 8; };

void() player_axec1 = [$axattc1, player_axec2] { self.weaponframe = 1; };
void() player_axec2 = [$axattc2, player_axec3] { self.weaponframe = 2; };
void() player_axec3 = [$axattc3, player_axec4] {self.weaponframe=3;W_FireAxe(); };
void() player_axec4 = [$axattc4, player_run] { self.weaponframe = 4; };

void() player_axed1 = [$axattd1, player_axed2] { self.weaponframe = 5; };
void() player_axed2 = [$axattd2, player_axed3] { self.weaponframe = 6; };
void() player_axed3 = [$axattd3, player_axed4] {self.weaponframe=7;W_FireAxe(); };
void() player_axed4 = [$axattd4, player_run] { self.weaponframe = 8; };

//============================================================================

void() player_nail1 = [$nailatt1, player_nail2] {
	muzzleflash();

	if(!self.button0 || intermission_running || self.impulse)
	{
		player_run();
		return;
	}
	self.weaponframe = self.weaponframe + 1;
	if(self.weaponframe == 9)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireSpikes(4);
	self.attack_finished = time + 0.2;
};
void() player_nail2 = [$nailatt2, player_nail1] {
	muzzleflash();

	if(!self.button0 || intermission_running || self.impulse)
	{
		player_run();
		return;
	}
	self.weaponframe = self.weaponframe + 1;
	if(self.weaponframe == 9)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireSpikes(-4);
	self.attack_finished = time + 0.2;
};

//============================================================================

void() player_light1 = [$light1, player_light2] {
	muzzleflash();

	if(!self.button0 || intermission_running)
	{
		player_run();
		return;
	}
	self.weaponframe = self.weaponframe + 1;
	if(self.weaponframe == 5)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireLightning();
	self.attack_finished = time + 0.2;
};
void() player_light2 = [$light2, player_light1] {
	muzzleflash();

	if(!self.button0 || intermission_running)
	{
		player_run();
		return;
	}
	self.weaponframe = self.weaponframe + 1;
	if(self.weaponframe == 5)
		self.weaponframe = 1;
	SuperDamageSound();
	W_FireLightning();
	self.attack_finished = time + 0.2;
};

//============================================================================

void() player_rocket1 = [$rockatt1, player_rocket2] {self.weaponframe=1; muzzleflash(); };
void() player_rocket2 = [$rockatt2, player_rocket3] { self.weaponframe = 2; };
void() player_rocket3 = [$rockatt3, player_rocket4] { self.weaponframe = 3; };
void() player_rocket4 = [$rockatt4, player_rocket5] { self.weaponframe = 4; };
void() player_rocket5 = [$rockatt5, player_rocket6] { self.weaponframe = 5; };
void() player_rocket6 = [$rockatt6, player_run] { self.weaponframe = 6; };
void DeathBubbles(float num_bubbles);

CBasePlayer::CBasePlayer() = default;
CBasePlayer::~CBasePlayer() = default;

/*
==============
CBasePlayer::Think

Called every tic for each player
==============
*/
void CBasePlayer::Think()
{
	playedTimeResidual += (gameLocal.time - gameLocal.previousTime);
	playedTimeSecs += playedTimeResidual / 1000;
	playedTimeResidual = playedTimeResidual % 1000;

	aimAssist.Update();

	UpdatePlayerIcons();

	UpdateSkinSetup();

	buttonMask &= usercmd.buttons;
	usercmd.buttons &= ~buttonMask;

	// clear the ik before we do anything else so the skeleton doesn't get updated twice
	walkIK.ClearJointMods();

	// if this is the very first frame of the map, set the delta view angles
	// based on the usercmd angles
	if(!spawnAnglesSet && (gameLocal.GameState() != GAMESTATE_STARTUP))
	{
		spawnAnglesSet = true;
		SetViewAngles(spawnAngles);
		oldImpulseSequence = usercmd.impulseSequence;
	}

	if(mountedObject)
	{
		usercmd.forwardmove = 0;
		usercmd.rightmove = 0;
		usercmd.buttons &= ~(BUTTON_JUMP | BUTTON_CROUCH);
	}

	if(objectiveSystemOpen || gameLocal.inCinematic || influenceActive)
	{
		if(objectiveSystemOpen && AI_PAIN)
		{
			TogglePDA();
		}
		usercmd.forwardmove = 0;
		usercmd.rightmove = 0;
		usercmd.buttons &= ~(BUTTON_JUMP | BUTTON_CROUCH);
	}

	// log movement changes for weapon bobbing effects
	if(usercmd.forwardmove != oldCmd.forwardmove)
	{
		loggedAccel_t *acc = &loggedAccel[currentLoggedAccel & (NUM_LOGGED_ACCELS - 1)];
		currentLoggedAccel++;
		acc->time = gameLocal.time;
		acc->dir[0] = usercmd.forwardmove - oldCmd.forwardmove;
		acc->dir[1] = acc->dir[2] = 0;
	}

	if(usercmd.rightmove != oldCmd.rightmove)
	{
		loggedAccel_t *acc = &loggedAccel[currentLoggedAccel & (NUM_LOGGED_ACCELS - 1)];
		currentLoggedAccel++;
		acc->time = gameLocal.time;
		acc->dir[1] = usercmd.rightmove - oldCmd.rightmove;
		acc->dir[0] = acc->dir[2] = 0;
	}

	// zooming
	if((usercmd.buttons ^ oldCmd.buttons) & BUTTON_ZOOM)
	{
		if((usercmd.buttons & BUTTON_ZOOM) && weapon.GetEntity())
		{
			zoomFov.Init(gameLocal.time, 200.0f, CalcFov(false), weapon.GetEntity()->GetZoomFov());
		}
		else
		{
			zoomFov.Init(gameLocal.time, 200.0f, zoomFov.GetCurrentValue(gameLocal.time), DefaultFov());
		}
	}

	// if we have an active gui, we will unrotate the view angles as
	// we turn the mouse movements into gui events
	idUserInterface *gui = ActiveGui();
	if(gui && gui != focusUI)
		RouteGuiMouse(gui);

	// set the push velocity on the weapon before running the physics
	if(weapon.GetEntity())
		weapon.GetEntity()->SetPushVelocity(physicsObj.GetPushedLinearVelocity());

	EvaluateControls();

	if(!af.IsActive())
	{
		AdjustBodyAngles();
		CopyJointsFromBodyToHead();
	}

	if(IsLocallyControlled())
	{
		// Local player on the server. Do normal movement.
		Move();
	}
	else
	{
		// Server is processing a client. Run client's commands like normal...
		Move();

		// ...then correct if needed.
		RunPhysics_RemoteClientCorrection();
	}

	if(!g_stopTime.GetBool())
	{
		if(!noclip && !spectating && (health > 0) && !IsHidden())
			TouchTriggers();

		// not done on clients for various reasons. don't do it on server and save the sound channel for other things
		if(!mpGame->GetRules()->IsMultiplayer())
		{
			SetCurrentHeartRate();
			float scale = new_g_damageScale;
			if(g_useDynamicProtection.GetBool() && scale < 1.0f && gameLocal.time - lastDmgTime > 500)
			{
				if(scale < 1.0f)
				{
					scale += 0.05f;
				}
				if(scale > 1.0f)
				{
					scale = 1.0f;
				}
				new_g_damageScale = scale;
			}
		}

		// update GUIs, Items, and character interactions
		UpdateFocus();

		UpdateLocation();

		// update player script
		UpdateScript();

		// service animations
		if(!spectating && !af.IsActive() && !gameLocal.inCinematic)
		{
			UpdateConditions();
			UpdateAnimState();
			CheckBlink();
		};

		// clear out our pain flag so we can tell if we recieve any damage between now and the next time we think
		AI_PAIN = false;
	};

	// calculate the exact bobbed view position, which is used to
	// position the view weapon, among other things
	CalculateFirstPersonView();

	// this may use firstPersonView, or a thirdPeroson / camera view
	CalculateRenderView();

	inventory.UpdateArmor();

	if(spectating)
		UpdateSpectating();
	else if(health > 0)
		UpdateWeapon();

	UpdateFlashlight();

	UpdateAir();

	UpdatePowerupHud();

	UpdateHud();

	UpdatePowerUps();

	UpdateDeathSkin(false);

	if(mpGame->GetRules()->IsMultiplayer())
	{
		DrawPlayerIcons();

		if(enviroSuitLight.IsValid())
		{
			idAngles lightAng = firstPersonViewAxis.ToAngles();
			idVec3 lightOrg = firstPersonViewOrigin;
			const idDict *lightDef = gameLocal.FindEntityDefDict("envirosuit_light", false);

			idVec3 enviroOffset = lightDef->GetVector("enviro_offset");
			idVec3 enviroAngleOffset = lightDef->GetVector("enviro_angle_offset");

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

	renderEntity_t *headRenderEnt = nullptr;
	if(head.GetEntity())
	{
		headRenderEnt = head.GetEntity()->GetRenderEntity();
	}
	if(headRenderEnt)
	{
		if(influenceSkin)
		{
			headRenderEnt->customSkin = influenceSkin;
		}
		else
		{
			headRenderEnt->customSkin = nullptr;
		}
	}

	if(mpGame->GetRules()->IsMultiplayer() || g_showPlayerShadow.GetBool())
	{
		renderEntity.suppressShadowInViewID = 0;
		if(headRenderEnt)
		{
			headRenderEnt->suppressShadowInViewID = 0;
		}
	}
	else
	{
		renderEntity.suppressShadowInViewID = entityNumber + 1;
		if(headRenderEnt)
		{
			headRenderEnt->suppressShadowInViewID = entityNumber + 1;
		}
	}
	// never cast shadows from our first-person muzzle flashes
	renderEntity.suppressShadowInLightID = LIGHTID_VIEW_MUZZLE_FLASH + entityNumber;
	if(headRenderEnt)
	{
		headRenderEnt->suppressShadowInLightID = LIGHTID_VIEW_MUZZLE_FLASH + entityNumber;
	}

	if(!g_stopTime.GetBool())
	{
		UpdateAnimation();

		Present();

		UpdateDamageEffects();

		LinkCombat();

		playerView.CalculateShake();
	}

	if(!(thinkFlags & TH_THINK))
	{
		gpSystem->Printf("player %d not thinking?\n", entityNumber);
	}

	if(g_showEnemies.GetBool())
	{
		CBaseActor *ent;
		int num = 0;
		for(ent = enemyList.Next(); ent != nullptr; ent = ent->enemyNode.Next())
		{
			gpSystem->Printf("enemy (%d)'%s'\n", ent->entityNumber, ent->name.c_str());
			gameRenderWorld->DebugBounds(colorRed, ent->GetPhysics()->GetBounds().Expand(2), ent->GetPhysics()->GetOrigin());
			num++;
		}
		gpSystem->Printf("%d: enemies\n", num);
	}

	inventory.RechargeAmmo(this);

	if(healthRecharge)
	{
		int elapsed = gameLocal.time - lastHealthRechargeTime;
		if(elapsed >= rechargeSpeed)
		{
			int intervals = (gameLocal.time - lastHealthRechargeTime) / rechargeSpeed;
			Give("health", va("%d", intervals), ITEM_GIVE_FEEDBACK | ITEM_GIVE_UPDATE_STATE);
			lastHealthRechargeTime += intervals * rechargeSpeed;
		}
	}

	// determine if portal sky is in pvs
	gameLocal.portalSkyActive = gameLocal.pvs.CheckAreasForPortalSky(gameLocal.GetPlayerPVS(), GetPhysics()->GetOrigin());

	// stereo rendering laser sight that replaces the crosshair
	UpdateLaserSight();

	// Show the respawn hud message if necessary.
	if(mpGame->GetRules()->IsMultiplayer() && (minRespawnTime != maxRespawnTime))
	{
		if(gameLocal.previousTime < minRespawnTime && minRespawnTime <= gameLocal.time)
		{
			// Server will show the hud message directly.
			ShowRespawnHudMessage();
		}
	}

	// Make sure voice groups are set to the right team
	if(mpGame->GetRules()->IsMultiplayer() && session->GetState() >= idSession::INGAME && entityNumber < MAX_CLIENTS)
	{
		// The entityNumber < MAX_CLIENTS seems to quiet the static analyzer
		// Make sure we're on the right team (at the lobby level)
		const int voiceTeam = spectating ? LOBBY_SPECTATE_TEAM_FOR_VOICE_CHAT : team;

		//idLib::Printf( "SERVER: Sending voice %i / %i\n", entityNumber, voiceTeam );

		// Update lobby team
		session->GetActingGameStateLobbyBase().SetLobbyUserTeam(gameLocal.lobbyUserIDs[entityNumber], voiceTeam);

		// Update voice groups to match in case something changed
		session->SetVoiceGroupsToTeams();
	};
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

inflictor, attacker, dir, and point can be nullptr for environmental effects
============
*/
void CBasePlayer::TakeDamage(CBaseEntity *inflictor, CBaseEntity *attacker, const idVec3 &dir, const char *damageDefName, const float damageScale, const int location)
{
	idVec3 kick;
	int damage;
	int armorSave;

	SetTimeState ts(timeGroup);

	if(!fl.takedamage || noclip || spectating || gameLocal.inCinematic)
		return;

	if(!inflictor)
		inflictor = mpGame->GetWorld();

	if(!attacker)
		attacker = mpGame->GetWorld();

	if(attacker->IsType(idAI::Type))
	{
		if(PowerUpActive(BERSERK))
			return;

		// don't take damage from monsters during influences
		if(influenceActive != 0)
			return;
	}

	const idDeclEntityDef *damageDef = gameLocal.FindEntityDef(damageDefName, false);
	if(!damageDef)
	{
		gameLocal.Warning("Unknown damageDef '%s'", damageDefName);
		return;
	}

	if(damageDef->dict.GetBool("ignore_player"))
		return;

	// determine knockback
	int knockback = 0;
	damageDef->dict.GetInt("knockback", "20", knockback);

	if(knockback != 0 && !fl.noknockback)
	{
		float attackerPushScale = 0.0f;

		if(attacker == this)
			damageDef->dict.GetFloat("attackerPushScale", "0", attackerPushScale);
		else
			attackerPushScale = 1.0f;

		idVec3 kick = dir;
		kick.Normalize();
		kick *= g_knockback.GetFloat() * knockback * attackerPushScale / 200.0f;
		physicsObj.SetLinearVelocity(physicsObj.GetLinearVelocity() + kick);

		// set the timer so that the player can't cancel out the movement immediately
		physicsObj.SetKnockBack(idMath::ClampInt(50, 200, knockback * 2));

		if(common->IsServer())
		{
			idBitMsg msg;
			byte msgBuf[MAX_EVENT_PARAM_SIZE];

			msg.InitWrite(msgBuf, sizeof(msgBuf));
			msg.WriteFloat(physicsObj.GetLinearVelocity()[0]);
			msg.WriteFloat(physicsObj.GetLinearVelocity()[1]);
			msg.WriteFloat(physicsObj.GetLinearVelocity()[2]);
			msg.WriteByte(idMath::ClampInt(50, 200, knockback * 2));
			ServerSendEvent(CBasePlayer::EVENT_KNOCKBACK, &msg, false);
		}
	}

	// If this is a locally controlled MP client, don't apply damage effects predictively here.
	// Local clients will see the damage feedback (view kick, etc) when their health changes
	// in a snapshot. This ensures that any feedback the local player sees is in sync with
	// his actual health reported by the server.
	if(mpGame->GetRules()->IsMultiplayer() && common->IsClient() && IsLocallyControlled())
		return;

	CalcDamagePoints(inflictor, attacker, &damageDef->dict, damageScale, location, &damage, &armorSave);

	// give feedback on the player view and audibly when armor is helping
	if(armorSave)
	{
		inventory.armor -= armorSave;

		if(gameLocal.time > lastArmorPulse + 200)
			StartSound("snd_hitArmor", SND_CHANNEL_ITEM, 0, false, nullptr);

		lastArmorPulse = gameLocal.time;
	};

	if(damageDef->dict.GetBool("burn"))
		StartSound("snd_burn", SND_CHANNEL_BODY3, 0, false, nullptr);
	else if(damageDef->dict.GetBool("no_air"))
	{
		if(!armorSave && health > 0)
			StartSound("snd_airGasp", SND_CHANNEL_ITEM, 0, false, nullptr);
	};

	if(g_debugDamage.GetInteger())
		gpSystem->Printf("client:%02d\tdamage type:%s\t\thealth:%03d\tdamage:%03d\tarmor:%03d\n", entityNumber, damageDef->GetName(), health, damage, armorSave);

	if(mpGame->GetRules()->IsMultiplayer() && IsLocallyControlled())
		ControllerShakeFromDamage(damage);

	// The client needs to know the final damage amount for predictive pain animations.
	const int finalDamage = AdjustDamageAmount(damage);

	if(health > 0)
	{
		// force a blink
		blink_time = 0;

		// let the anim script know we took damage
		AI_PAIN = Pain(inflictor, attacker, damage, dir, location);
	}

	// Only actually deal the damage here in singleplayer and for locally controlled servers.
	if(!mpGame->GetRules()->IsMultiplayer() || common->IsServer())
	{
		// Server will deal his damage normally
		ServerDealDamage(finalDamage, *inflictor, *attacker, dir, damageDefName, location);
	}
	else if(attacker->GetEntityNumber() == gameLocal.GetLocalClientNum())
	{
		// Clients send a reliable message to the server with the parameters of the hit. The
		// server should make sure the client still has line-of-sight to its target before
		// actually applying the damage.

		byte msgBuffer[MAX_GAME_MESSAGE_SIZE];
		CBitMsg msg;

		msg.InitWrite(msgBuffer, sizeof(msgBuffer));
		msg.BeginWriting();

		msg.WriteShort(attacker->GetEntityNumber());
		msg.WriteShort(GetEntityNumber()); // victim
		msg.WriteVectorFloat(dir);
		msg.WriteLong(damageDef->Index());
		msg.WriteFloat(damageScale);
		msg.WriteLong(location);

		idLobbyBase &lobby = session->GetActingGameStateLobbyBase();
		lobby.SendReliableToHost(GAME_RELIABLE_MESSAGE_CLIENT_HITSCAN_HIT, msg);
	};

	lastDamageDef = damageDef->Index();
	lastDamageDir = dir;
	lastDamageDir.Normalize();
	lastDamageLocation = location;
};

/*
=================
CBasePlayer::StartHealthRecharge
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
CBasePlayer::StopHealthRecharge
=================
*/
void CBasePlayer::StopHealthRecharge()
{
	healthRecharge = false;
};

/*
================
CBasePlayer::Hide
================
*/
void CBasePlayer::Hide()
{
	CBaseActor::Hide();

	CBaseWeapon *weap = weapon.GetEntity();
	if(weap)
		weap->HideWorldModel();

	CBaseWeapon *flash = flashlight.GetEntity();
	if(flash)
		flash->HideWorldModel();
};

/*
================
CBasePlayer::Show
================
*/
void CBasePlayer::Show()
{
	CBaseActor::Show();

	CBaseWeapon *weap = weapon.GetEntity();
	if(weap)
		weap->ShowWorldModel();

	CBaseWeapon *flash = flashlight.GetEntity();
	if(flash)
		flash->ShowWorldModel();
};

/*
==================
CBasePlayer::LookAtKiller
==================
*/
void CBasePlayer::LookAtKiller(CBaseEntity *inflictor, CBaseEntity *attacker)
{
	CVec3 dir{viewAxis[0]};

	if(attacker && attacker != this)
		dir = attacker->GetPhysics()->GetOrigin() - GetPhysics()->GetOrigin();
	else if(inflictor && inflictor != this)
		dir = inflictor->GetPhysics()->GetOrigin() - GetPhysics()->GetOrigin();

	CAngles ang(0, dir.ToYaw(), 0);
	SetViewAngles(ang);
};

/*
==============
CBasePlayer::TogglePDA
==============
*/
void CBasePlayer::TogglePDA()
{
	if(inventory.pdas.Num() == 0)
	{
		ShowTip(spawnArgs.GetString("text_infoTitle"), spawnArgs.GetString("text_noPDA"), true);
		return;
	};

	if(pdaMenu != nullptr)
	{
		objectiveSystemOpen = !objectiveSystemOpen;
		pdaMenu->ActivateMenu(objectiveSystemOpen);

		if(objectiveSystemOpen)
		{
			if(hud)
				hud->ClearNewPDAInfo();
		};
	};
};

/*
==============
CBasePlayer::PlayAudioLog
==============
*/
void CBasePlayer::PlayAudioLog(const idSoundShader *shader)
{
	EndVideoDisk();
	if(name.Length() > 0)
	{
		int ms;
		StartSoundShader(shader, SND_CHANNEL_PDA_AUDIO, 0, false, &ms);
		CancelEvents(&EV_Player_StopAudioLog);
		PostEventMS(&EV_Player_StopAudioLog, ms + 150);
	};
};

/*
==============
CBasePlayer::EndAudioLog
==============
*/
void CBasePlayer::EndAudioLog()
{
	StopSound(SND_CHANNEL_PDA_AUDIO, false);
};

/*
===============
CBasePlayer::UpdateWeapon
===============
*/
void CBasePlayer::UpdateWeapon()
{
	if(health <= 0)
	{
		return;
	}

	assert(!spectating);

	if(common->IsClient())
	{
		// clients need to wait till the weapon and it's world model entity
		// are present and synchronized ( weapon.worldModel idEntityPtr to idAnimatedEntity )
		if(!weapon.GetEntity()->IsWorldModelReady())
		{
			return;
		}
	}

	// always make sure the weapon is correctly setup before accessing it
	if(!weapon.GetEntity()->IsLinked())
	{
		if(idealWeapon != -1)
		{
			animPrefix = spawnArgs.GetString(va("def_weapon%d", idealWeapon.Get()));
			int ammoInClip = inventory.GetClipAmmoForWeapon(idealWeapon.Get());
			if(mpGame->GetRules()->IsMultiplayer() && respawning)
			{
				// Do not load ammo into the clip here on MP respawn, as it will be done
				// elsewhere. If we take ammo out here then the player will end up losing
				// a clip of ammo for their initial weapon upon respawn.
				ammoInClip = 0;
			}
			weapon.GetEntity()->GetWeaponDef(animPrefix, ammoInClip);
			assert(weapon.GetEntity()->IsLinked());
		}
		else
		{
			return;
		}
	}

	if(hiddenWeapon && tipUp && usercmd.buttons & BUTTON_ATTACK)
	{
		HideTip();
	}

	if(g_dragEntity.GetBool())
	{
		StopFiring();
		weapon.GetEntity()->LowerWeapon();
		dragEntity.Update(this);
	}
	else if(ActiveGui())
	{
		// gui handling overrides weapon use
		Weapon_GUI();
	}
	else if(focusCharacter && (focusCharacter->health > 0))
	{
		Weapon_NPC();
	}
	else
	{
		Weapon_Combat();
	}

	if(hiddenWeapon)
	{
		weapon.GetEntity()->LowerWeapon();
	}

	// update weapon state, particles, dlights, etc
	weapon.GetEntity()->PresentWeapon(CanShowWeaponViewmodel());
}

/*
===============
CBasePlayer::UpdateFlashLight
===============
*/
void CBasePlayer::UpdateFlashlight()
{
	if(idealWeapon == weapon_flashlight)
	{
		// force classic flashlight to go away
		NextWeapon();
	}

	if(!flashlight.IsValid())
	{
		return;
	}

	if(!flashlight.GetEntity()->GetOwner())
	{
		return;
	}

	// Don't update the flashlight if dead in MP.
	// Otherwise you can see a floating flashlight worldmodel near player's skeletons.
	if(mpGame->GetRules()->IsMultiplayer())
	{
		if(health < 0)
		{
			return;
		}
	}

	// Flashlight has an infinite battery in multiplayer.
	if(!mpGame->GetRules()->IsMultiplayer())
	{
		if(flashlight.GetEntity()->lightOn)
		{
			if(flashlight_batteryDrainTimeMS.GetInteger() > 0)
			{
				flashlightBattery -= (gameLocal.time - gameLocal.previousTime);
				if(flashlightBattery < 0)
				{
					FlashlightOff();
					flashlightBattery = 0;
				}
			}
		}
		else
		{
			if(flashlightBattery < flashlight_batteryDrainTimeMS.GetInteger())
			{
				flashlightBattery += (gameLocal.time - gameLocal.previousTime) * Max(1, (flashlight_batteryDrainTimeMS.GetInteger() / flashlight_batteryChargeTimeMS.GetInteger()));
				if(flashlightBattery > flashlight_batteryDrainTimeMS.GetInteger())
				{
					flashlightBattery = flashlight_batteryDrainTimeMS.GetInteger();
				}
			}
		}
	}

	if(hud)
	{
		hud->UpdateFlashlight(this);
	}

	if(common->IsClient())
	{
		// clients need to wait till the weapon and it's world model entity
		// are present and synchronized ( weapon.worldModel idEntityPtr to idAnimatedEntity )
		if(!flashlight.GetEntity()->IsWorldModelReady())
		{
			return;
		}
	}

	// always make sure the weapon is correctly setup before accessing it
	if(!flashlight.GetEntity()->IsLinked())
	{
		flashlight.GetEntity()->GetWeaponDef("weapon_flashlight_new", 0);
		flashlight.GetEntity()->SetIsPlayerFlashlight(true);

		// adjust position / orientation of flashlight
		idAnimatedEntity *worldModel = flashlight.GetEntity()->GetWorldModel();
		worldModel->BindToJoint(this, "Chest", true);
		// Don't interpolate the flashlight world model in mp, let it bind like normal.
		worldModel->SetUseClientInterpolation(false);

		assert(flashlight.GetEntity()->IsLinked());
	}

	// this positions the third person flashlight model! (as seen in the mirror)
	idAnimatedEntity *worldModel = flashlight.GetEntity()->GetWorldModel();
	static const idVec3 fl_pos = idVec3(3.0f, 9.0f, 2.0f);
	worldModel->GetPhysics()->SetOrigin(fl_pos);
	static float fl_pitch = 0.0f;
	static float fl_yaw = 0.0f;
	static float fl_roll = 0.0f;
	static idAngles ang = ang_zero;
	ang.Set(fl_pitch, fl_yaw, fl_roll);
	worldModel->GetPhysics()->SetAxis(ang.ToMat3());

	if(flashlight.GetEntity()->lightOn)
	{
		if((flashlightBattery < flashlight_batteryChargeTimeMS.GetInteger() / 2) && (gameLocal.random.RandomFloat() < flashlight_batteryFlickerPercent.GetFloat()))
		{
			flashlight.GetEntity()->RemoveMuzzleFlashlight();
		}
		else
		{
			flashlight.GetEntity()->MuzzleFlashLight();
		}
	}

	flashlight.GetEntity()->PresentWeapon(true);

	if(mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons") || gameLocal.inCinematic || spectating || fl.hidden)
	{
		worldModel->Hide();
	}
	else
	{
		worldModel->Show();
	}
}

/*
=================
CBasePlayer::DropWeapon
=================
*/
void CBasePlayer::DropWeapon(bool died)
{
	idVec3 forward, up;
	int inclip, ammoavailable;

	if(died == false)
	{
		return;
	}

	assert(!common->IsClient());

	if(spectating || weaponGone || weapon.GetEntity() == nullptr)
	{
		return;
	}

	if((!died && !weapon.GetEntity()->IsReady()) || weapon.GetEntity()->IsReloading())
	{
		return;
	}
	// ammoavailable is how many shots we can fire
	// inclip is which amount is in clip right now
	ammoavailable = weapon.GetEntity()->AmmoAvailable();
	inclip = weapon.GetEntity()->AmmoInClip();

	// don't drop a grenade if we have none left
	if(!idStr::Icmp(idWeapon::GetAmmoNameForNum(weapon.GetEntity()->GetAmmoType()), "ammo_grenades") && (ammoavailable - inclip <= 0))
	{
		return;
	}

	ammoavailable += inclip;

	// expect an ammo setup that makes sense before doing any dropping
	// ammoavailable is -1 for infinite ammo, and weapons like chainsaw
	// a bad ammo config usually indicates a bad weapon state, so we should not drop
	// used to be an assertion check, but it still happens in edge cases

	if((ammoavailable != -1) && (ammoavailable < 0))
	{
		common->DPrintf("CBasePlayer::DropWeapon: bad ammo setup\n");
		return;
	}
	idEntity *item = nullptr;
	if(died)
	{
		// ain't gonna throw you no weapon if I'm dead
		item = weapon.GetEntity()->DropItem(vec3_origin, 0, WEAPON_DROP_TIME, died);
	}
	else
	{
		viewAngles.ToVectors(&forward, nullptr, &up);
		item = weapon.GetEntity()->DropItem(250.0f * forward + 150.0f * up, 500, WEAPON_DROP_TIME, died);
	}
	if(!item)
	{
		return;
	}
	// set the appropriate ammo in the dropped object
	const idKeyValue *keyval = item->spawnArgs.MatchPrefix("inv_ammo_");
	if(keyval)
	{
		item->spawnArgs.SetInt(keyval->GetKey(), ammoavailable);
		idStr inclipKey = keyval->GetKey();
		inclipKey.Insert("inclip_", 4);
		inclipKey.Insert(va("%.2d", currentWeapon), 11);
		item->spawnArgs.SetInt(inclipKey, inclip);
	}
	if(!died)
	{
		// remove from our local inventory completely
		inventory.Drop(spawnArgs, item->spawnArgs.GetString("inv_weapon"), -1);
		weapon.GetEntity()->ResetAmmoClip();
		NextWeapon();
		weapon.GetEntity()->WeaponStolen();
		weaponGone = true;
	}
}

/*
===============
CBasePlayer::NextWeapon
===============
*/
void CBasePlayer::NextWeapon()
{
	if(!weaponEnabled || spectating || hiddenWeapon || gameLocal.inCinematic || mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons") || health < 0)
	{
		return;
	}

	// check if we have any weapons
	if(!inventory.weapons)
	{
		return;
	}

	int w = idealWeapon.Get();
	while(1)
	{
		w++;
		if(w >= MAX_WEAPONS)
		{
			w = 0;
		}
		if(w == idealWeapon)
		{
			w = weapon_fists;
			break;
		}
		if((inventory.weapons & (1 << w)) == 0)
		{
			continue;
		}
		const char *weap = spawnArgs.GetString(va("def_weapon%d", w));
		if(!spawnArgs.GetBool(va("weapon%d_cycle", w)))
		{
			continue;
		}
		if(!weap[0])
		{
			continue;
		}

		if(inventory.HasAmmo(weap, true, this))
		{
			break;
		}
	}

	if((w != currentWeapon) && (w != idealWeapon))
	{
		idealWeapon = w;
		weaponSwitchTime = gameLocal.time + WEAPON_SWITCH_DELAY;
		UpdateHudWeapon();
	}
}

/*
===============
CBasePlayer::PrevWeapon
===============
*/
void CBasePlayer::PrevWeapon()
{
	if(!weaponEnabled || spectating || hiddenWeapon || gameLocal.inCinematic || mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons") || health < 0)
		return;

	// check if we have any weapons
	if(!inventory.weapons)
		return;

	int w = idealWeapon.Get();
	while(1)
	{
		w--;
		if(w < 0)
		{
			w = MAX_WEAPONS - 1;
		}
		if(w == idealWeapon)
		{
			w = weapon_fists;
			break;
		}
		if((inventory.weapons & (1 << w)) == 0)
		{
			continue;
		}
		const char *weap = spawnArgs.GetString(va("def_weapon%d", w));
		if(!spawnArgs.GetBool(va("weapon%d_cycle", w)))
		{
			continue;
		}
		if(!weap[0])
		{
			continue;
		}
		if(inventory.HasAmmo(weap, true, this))
		{
			break;
		}
	}

	if((w != currentWeapon) && (w != idealWeapon))
	{
		idealWeapon = w;
		weaponSwitchTime = gameLocal.time + WEAPON_SWITCH_DELAY;
		UpdateHudWeapon();
	};
};

/*
===============
CBasePlayer::SelectWeapon
===============
*/
void CBasePlayer::SelectWeapon(int num, bool force)
{
	const char *weap{""};

	if(!weaponEnabled || spectating || gameLocal.inCinematic || health < 0)
		return;

	if((num < 0) || (num >= MAX_WEAPONS))
		return;

	if(num == weapon_flashlight)
		return;

	if((num != weapon_pda) && mpGame->GetWorld()->spawnArgs.GetBool("no_Weapons"))
	{
		num = weapon_fists;
		hiddenWeapon ^= 1;
		if(hiddenWeapon && weapon.GetEntity())
			weapon.GetEntity()->LowerWeapon();
		else
			weapon.GetEntity()->RaiseWeapon();
	};

	//Is the weapon a toggle weapon
	WeaponToggle_t *weaponToggle;
	if(weaponToggles.Get(va("weapontoggle%d", num), &weaponToggle))
	{
		int weaponToggleIndex = 0;

		//Find the current Weapon in the list
		int currentIndex = -1;
		for(int i = 0; i < weaponToggle->toggleList.Num(); i++)
		{
			if(weaponToggle->toggleList[i] == idealWeapon)
			{
				currentIndex = i;
				break;
			}
		}
		if(currentIndex == -1)
		{
			//Didn't find the current weapon so select the first item
			weaponToggleIndex = weaponToggle->lastUsed;
		}
		else
		{
			//Roll to the next available item in the list
			weaponToggleIndex = currentIndex;
			weaponToggleIndex++;
			if(weaponToggleIndex >= weaponToggle->toggleList.Num())
			{
				weaponToggleIndex = 0;
			}
		}

		for(int i = 0; i < weaponToggle->toggleList.Num(); i++)
		{
			int weapNum = weaponToggle->toggleList[weaponToggleIndex];
			//Is it available
			if(inventory.weapons & (1 << weapNum))
			{
				//Do we have ammo for it
				if(inventory.HasAmmo(spawnArgs.GetString(va("def_weapon%d", weapNum)), true, this) || spawnArgs.GetBool(va("weapon%d_allowempty", weapNum)))
				{
					break;
				}
			}

			weaponToggleIndex++;
			if(weaponToggleIndex >= weaponToggle->toggleList.Num())
			{
				weaponToggleIndex = 0;
			}
		}
		weaponToggle->lastUsed = weaponToggleIndex;
		num = weaponToggle->toggleList[weaponToggleIndex];
	}

	weap = spawnArgs.GetString(va("def_weapon%d", num));
	if(!weap[0])
	{
		gameLocal.Printf("Invalid weapon\n");
		return;
	}

	if(force || (inventory.weapons & (1 << num)))
	{
		if(!inventory.HasAmmo(weap, true, this) && !spawnArgs.GetBool(va("weapon%d_allowempty", num)))
		{
			return;
		}
		if((previousWeapon >= 0) && (idealWeapon == num) && (spawnArgs.GetBool(va("weapon%d_toggle", num))))
		{
			weap = spawnArgs.GetString(va("def_weapon%d", previousWeapon));
			if(!inventory.HasAmmo(weap, true, this) && !spawnArgs.GetBool(va("weapon%d_allowempty", previousWeapon)))
			{
				return;
			}
			idealWeapon = previousWeapon;
		}
		else if((weapon_pda >= 0) && (num == weapon_pda) && (inventory.pdas.Num() == 0))
		{
			ShowTip(spawnArgs.GetString("text_infoTitle"), spawnArgs.GetString("text_noPDA"), true);
			return;
		}
		else
			idealWeapon = num;

		UpdateHudWeapon();
	};
};

/*
===============
CBasePlayer::GiveObjective
===============
*/
void CBasePlayer::GiveObjective(const char *title, const char *text, const idMaterial *screenshot)
{
	idObjectiveInfo &info = inventory.objectiveNames.Alloc();
	info.title = title;
	info.text = text;
	info.screenshot = screenshot;

	StartSound("snd_objectiveup", SND_CHANNEL_ANY, 0, false, nullptr);

	if(hud)
	{
		hud->SetupObjective(title, text, screenshot);
		hud->ShowObjective(false);
		objectiveUp = true;
	};
};

/*
===============
CBasePlayer::CompleteObjective
===============
*/
void CBasePlayer::CompleteObjective(const char *title)
{
	int c = inventory.objectiveNames.Num();
	for(int i = 0; i < c; i++)
	{
		if(idStr::Icmp(inventory.objectiveNames[i].title, title) == 0)
		{
			inventory.objectiveNames.RemoveIndex(i);
			break;
		};
	};

	StartSound("snd_objectiveup", SND_CHANNEL_ANY, 0, false, nullptr);

	if constexpr(IsClient())
	{
		if(hud)
		{
			hud->SetupObjectiveComplete(title);
			hud->ShowObjective(true);
		};
	};
};

/*
===============
CBasePlayer::GiveItem

Returns false if the item shouldn't be picked up
===============
*/
bool CBasePlayer::GiveItem(CBaseItem *item, uint giveFlags)
{
	int i;
	const CKeyValue *arg;
	CDict attr;
	bool gave;
	int numPickup;

	if(mpGame->GetRules()->IsMultiplayer() && spectating)
		return false;

	if(idStr::FindText(item->GetName(), "weapon_flashlight_new") > -1)
		return false;

	if(idStr::FindText(item->GetName(), "weapon_flashlight") > -1)
	{
		// don't allow flashlight weapon unless classic mode is enabled
		return false;
	}

	item->GetAttributes(attr);

	gave = false;
	numPickup = inventory.pickupItemNames.Num();
	for(i = 0; i < attr.GetNumKeyVals(); i++)
	{
		arg = attr.GetKeyVal(i);
		if(Give(arg->GetKey(), arg->GetValue(), giveFlags))
			gave = true;
	};

	if(giveFlags & ITEM_GIVE_FEEDBACK)
	{
		arg = item->spawnArgs.MatchPrefix("inv_weapon", nullptr);
		if(arg)
		{
			// We need to update the weapon hud manually, but not
			// the armor/ammo/health because they are updated every
			// frame no matter what
			UpdateHudWeapon(false);
		}

		// display the pickup feedback on the hud
		if(gave && (numPickup == inventory.pickupItemNames.Num()))
			inventory.AddPickupName(item->spawnArgs.GetString("inv_name"), this); //_D3XP
	};

	return gave;
};

/*
===============
CBasePlayer::FireWeapon
===============
*/
idCVar g_infiniteAmmo("g_infiniteAmmo", "0", CVAR_GAME | CVAR_BOOL, "infinite ammo");
extern idCVar ui_autoSwitch;
void CBasePlayer::FireWeapon()
{
	CMat3 axis;
	CVec3 muzzle;

	if(privateCameraView)
		return;

	if(g_editEntityMode.GetInteger())
	{
		GetViewPos(muzzle, axis);
		if(gameLocal.editEntities->SelectEntity(muzzle, axis[0], this))
			return;
	};

	// TODO: Q3 block
	// set aiming directions
	//AngleVectors(this->ps.viewangles, forward, right, up);

	//CalcMuzzlePointOrigin(ent, this->oldOrigin, forward, right, up, muzzle);
	//
	
	if(!hiddenWeapon && myCurrentWeapon.GetEntity()->IsReady())
	{
		if(g_infiniteAmmo.GetBool() || myCurrentWeapon.GetEntity()->AmmoInClip() || myCurrentWeapon.GetEntity()->AmmoAvailable())
		{
			AI_ATTACK_HELD = true;
			
			// fire the specific weapon
			myCurrentWeapon.GetEntity()->BeginAttack();
		}
		else
		{
			idLobbyBase &lobby = session->GetActingGameStateLobbyBase();
			lobbyUserID_t &lobbyUserID = gameLocal.lobbyUserIDs[entityNumber];
			
			bool autoSwitch = lobby.GetLobbyUserWeaponAutoSwitch(lobbyUserID);
			if(!autoSwitch)
				return;

			// update our ammo clip in our inventory
			if((currentWeapon >= 0) && (currentWeapon < MAX_WEAPONS))
				inventory.SetClipAmmoForWeapon(currentWeapon, weapon.GetEntity()->AmmoInClip());

			NextBestWeapon();
		};
	};

	if constexpr(IsClient())
	{
		if(tipUp)
			HideTip();

		if(objectiveUp)
			HideObjective();
	};
};

/*
===============
CBasePlayer::StopFiring
===============
*/
void CBasePlayer::StopFiring()
{
	AI_ATTACK_HELD = false;
	AI_WEAPON_FIRED = false;
	AI_RELOAD = false;
	
	if(weapon.GetEntity())
		weapon.GetEntity()->EndAttack();
}

/*
===========
CBasePlayer::SelectInitialSpawnPoint

Try to find a spawn point marked 'initial', otherwise
use normal spawn selection.
============
*/
void CBasePlayer::SelectInitialSpawnPoint(idVec3 &origin, idAngles &angles)
{
	CBaseEntity *spot;
	idStr skin;

	spot = gameLocal.SelectInitialSpawnPoint(this);

	// set the player skin from the spawn location
	if(spot->spawnArgs.GetString("skin", nullptr, skin))
	{
		spawnArgs.Set("spawn_skin", skin);
	}

	// activate the spawn locations targets
	spot->PostEventMS(&EV_ActivateTargets, 0, this);

	origin = spot->GetPhysics()->GetOrigin();
	origin[2] += 4.0f + CM_BOX_EPSILON; // move up to make sure the player is at least an epsilon above the floor
	angles = spot->GetPhysics()->GetAxis().ToAngles();
}

/*
===========
CBasePlayer::SpawnFromSpawnSpot

Chooses a spawn location and spawns the player
============
*/
void CBasePlayer::SpawnFromSpawnSpot()
{
	idVec3 spawn_origin;
	idAngles spawn_angles;

	SelectInitialSpawnPoint(spawn_origin, spawn_angles);
	SpawnToPoint(spawn_origin, spawn_angles);
}

/*
===========
CBasePlayer::SpawnToPoint

Called every time a client is placed fresh in the world:
after the first ClientBegin, and after each respawn
Initializes all non-persistant parts of playerState

when called here with spectating set to true, just place yourself and init
============
*/
void CBasePlayer::SpawnToPoint(const idVec3 &spawn_origin, const idAngles &spawn_angles)
{
	idVec3 spec_origin;

	respawning = true;

	Init();

	fl.noknockback = false;

	// stop any ragdolls being used
	StopRagdoll();

	// set back the player physics
	SetPhysics(&physicsObj);

	physicsObj.SetClipModelAxis();
	physicsObj.EnableClip();

	if(!spectating)
	{
		SetCombatContents(true);
	}

	physicsObj.SetLinearVelocity(vec3_origin);

	// setup our initial view
	if(!spectating)
	{
		SetOrigin(spawn_origin);
	}
	else
	{
		spec_origin = spawn_origin;
		spec_origin[2] += pm_normalheight.GetFloat();
		spec_origin[2] += SPECTATE_RAISE;
		SetOrigin(spec_origin);
	}

	// if this is the first spawn of the map, we don't have a usercmd yet,
	// so the delta angles won't be correct.  This will be fixed on the first think.
	viewAngles = ang_zero;
	SetDeltaViewAngles(ang_zero);
	SetViewAngles(spawn_angles);
	spawnAngles = spawn_angles;
	spawnAnglesSet = false;

	legsForward = true;
	legsYaw = 0.0f;
	idealLegsYaw = 0.0f;
	oldViewYaw = viewAngles.yaw;

	if(spectating)
	{
		Hide();
	}
	else
	{
		Show();
	}

	if(mpGame->GetRules()->IsMultiplayer())
	{
		if(!spectating)
		{
			// we may be called twice in a row in some situations. avoid a double fx and 'fly to the roof'
			if(lastTeleFX < gameLocal.time - 1000)
			{
				idEntityFx::StartFx(spawnArgs.GetString("fx_spawn"), &spawn_origin, nullptr, this, true);
				lastTeleFX = gameLocal.time;
			}
		}
		AI_TELEPORT = true;
	}
	else
	{
		AI_TELEPORT = false;
	}

	// kill anything at the new position
	if(!spectating)
	{
		physicsObj.SetClipMask(MASK_PLAYERSOLID); // the clip mask is usually maintained in Move(), but KillBox requires it
		gameLocal.KillBox(this);
	}

	// don't allow full run speed for a bit
	physicsObj.SetKnockBack(100);

	// set our respawn time and buttons so that if we're killed we don't respawn immediately
	minRespawnTime = gameLocal.time;
	maxRespawnTime = gameLocal.time;
	if(!spectating)
	{
		forceRespawn = false;
	}

	Respawn_Shared();

	privateCameraView = nullptr;

	BecomeActive(TH_THINK);

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	Think();

	respawning = false;
	lastManOver = false;
	lastManPlayAgain = false;
	isTelefragged = false;
}

/*
===============
CBasePlayer::ServerSpectate
================
*/
void CBasePlayer::ServerSpectate(bool spectate)
{
	if(spectating != spectate)
	{
		Spectate(spectate);
		if(!spectate)
		{
			// When coming out of spectate, join the team with the least number of players
			if(gameLocal.mpGame.IsGametypeTeamBased())
			{
				int teamCounts[2] = { 0, 0 };
				gameLocal.mpGame.NumActualClients(false, teamCounts);
				teamCounts[team]--;
				if(teamCounts[0] < teamCounts[1])
				{
					team = 0;
				}
				else if(teamCounts[1] < teamCounts[0])
				{
					team = 1;
				}
				gameLocal.mpGame.SwitchToTeam(entityNumber, -1, team);
			};
			
			if(gameLocal.gameType == GAME_DM)
			{
				// make sure the scores are reset so you can't exploit by spectating and entering the game back
				// other game types don't matter, as you either can't join back, or it's team scores
				gameLocal.mpGame.ClearFrags(entityNumber);
			};
		};
	};
	
	if(!spectate)
		SpawnFromSpawnSpot();

	// drop the flag if player was carrying it
	if(spectate && mpGame->GetRules()->IsMultiplayer() && gameLocal.mpGame.IsGametypeFlagBased() && carryingFlag)
		DropFlag();
}

/*
===============
CBasePlayer::ShowTip
===============
*/
void CBasePlayer::ShowTip(const char *title, const char *tip, bool autoHide)
{
	if(tipUp)
	{
		return;
	}

	if(hudManager)
	{
		hudManager->ShowTip(title, tip, autoHide);
	}
	tipUp = true;
}

/*
===============
CBasePlayer::HideTip
===============
*/
void CBasePlayer::HideTip()
{
	if(hudManager)
	{
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
void CBasePlayer::ServerDealDamage(int damage, idEntity &inflictor, idEntity &attacker, const idVec3 &dir, const char *damageDefName, const int location)
{
	const idDeclEntityDef *damageDef = gameLocal.FindEntityDef(damageDefName, false);
	if(!damageDef)
	{
		gameLocal.Warning("Unknown damageDef '%s'", damageDefName);
		return;
	}

	// move the world direction vector to local coordinates
	idVec3 damage_from;
	idVec3 localDamageVector;

	damage_from = dir;
	damage_from.Normalize();

	viewAxis.ProjectVector(damage_from, localDamageVector);

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if(health > 0)
	{
		playerView.DamageImpulse(localDamageVector, &damageDef->dict);
	}

	// do the damage
	if(damage > 0)
	{
		GetAchievementManager().SetPlayerTookDamage(true);

		int oldHealth = health;
		health -= damage;

		if(health <= 0)
		{
			if(health < -999)
			{
				health = -999;
			}

			// HACK - A - LICIOUS - Check to see if we are being damaged by the frag chamber.
			if(oldHealth > 0 && strcmp(gameLocal.GetMapName(), "maps/game/mp/d3dm3.map") == 0 && strcmp(damageDefName, "damage_triggerhurt_1000_chamber") == 0)
			{
				CBasePlayer *fragChamberActivator = gameLocal.playerActivateFragChamber;
				if(fragChamberActivator != nullptr)
				{
					fragChamberActivator->GetAchievementManager().EventCompletesAchievement(ACHIEVEMENT_MP_CATCH_ENEMY_IN_ROFC);
				}
				gameLocal.playerActivateFragChamber = nullptr;
			}

			isTelefragged = damageDef->dict.GetBool("telefrag");

			lastDmgTime = gameLocal.time;
			Killed(&inflictor, &attacker, damage, dir, location);
		}
		else
		{
			if(!g_testDeath.GetBool())
			{
				lastDmgTime = gameLocal.time;
			}
		}
	}
	else
	{
		// don't accumulate impulses
		if(af.IsLoaded())
		{
			// clear impacts
			af.Rest();

			// physics is turned off by calling af.Rest()
			BecomeActive(TH_PHYSICS);
		}
	}
}

// idtech4 imports end

void CBasePlayer::PainSound()
{
	float rs;

	if(self.health < 0)
		return;

	if(damage_attacker.classname == "teledeath")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};

	// water pain sounds
	if(self.watertype == CONTENT_WATER && self.waterlevel == 3)
	{
		DeathBubbles(1);
		if(random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/drown1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/drown2.wav", 1, ATTN_NORM);
		return;
	};

	// slime pain sounds
	if(self.watertype == CONTENT_SLIME)
	{
		// FIX ME       put in some steam here
		if(random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn2.wav", 1, ATTN_NORM);
		return;
	};

	if(self.watertype == CONTENT_LAVA)
	{
		if(random() > 0.5)
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn1.wav", 1, ATTN_NORM);
		else
			gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/lburn2.wav", 1, ATTN_NORM);
		return;
	};

	if(self.pain_finished > time)
	{
		self.axhitme = 0;
		return;
	};

	self.pain_finished = time + 0.5;

	// don't make multiple pain sounds right after each other

	// ax pain sound
	if(self.axhitme == 1)
	{
		self.axhitme = 0;
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/axhit1.wav", 1, ATTN_NORM);
		return;
	};

	rs = rint((random() * 5) + 1);

	self.noise = "";
	if(rs == 1)
		self.noise = "player/pain1.wav";
	else if(rs == 2)
		self.noise = "player/pain2.wav";
	else if(rs == 3)
		self.noise = "player/pain3.wav";
	else if(rs == 4)
		self.noise = "player/pain4.wav";
	else if(rs == 5)
		self.noise = "player/pain5.wav";
	else
		self.noise = "player/pain6.wav";

	gpEngine->pfnEmitSound(self, CHAN_VOICE, self.noise, 1, ATTN_NORM);
	return;
};

void() player_pain1 = [$pain1, player_pain2] {PainSound();self.weaponframe=0; };
void() player_pain2 = [$pain2, player_pain3] {};
void() player_pain3 = [$pain3, player_pain4] {};
void() player_pain4 = [$pain4, player_pain5] {};
void() player_pain5 = [$pain5, player_pain6] {};
void() player_pain6 = [$pain6, player_run] {};

void() player_axpain1 = [$axpain1, player_axpain2] {PainSound();self.weaponframe=0; };
void() player_axpain2 = [$axpain2, player_axpain3] {};
void() player_axpain3 = [$axpain3, player_axpain4] {};
void() player_axpain4 = [$axpain4, player_axpain5] {};
void() player_axpain5 = [$axpain5, player_axpain6] {};
void() player_axpain6 = [$axpain6, player_run] {};

void player_pain()
{
	if(self.weaponframe)
		return;

	if(self.invisible_finished > time)
		return; // eyes don't have pain frames

	if(self.weapon == IT_AXE)
		player_axpain1();
	else
		player_pain1();
};

void player_diea1();
void player_dieb1();
void player_diec1();
void player_died1();
void player_diee1();
void player_die_ax1();

void DeathBubblesSpawn()
{
	entity bubble;

	if(self.owner.waterlevel != 3)
		return;

	bubble = spawn();
	
	gpEngine->pfnSetModel(bubble, "sprites/s_bubble.spr");
	gpEngine->pfnSetOrigin(bubble, self.owner.origin + '0 0 24');
	
	bubble.movetype = MOVETYPE_NOCLIP;
	bubble.solid = SOLID_NOT;
	bubble.velocity = '0 0 15';
	bubble.nextthink = time + 0.5;
	bubble.think = bubble_bob;
	bubble.classname = "bubble";
	bubble.frame = 0;
	bubble.cnt = 0;
	setsize(bubble, '-8 -8 -8', '8 8 8');
	self.nextthink = time + 0.1;
	self.think = DeathBubblesSpawn;
	self.air_finished = self.air_finished + 1;
	if(self.air_finished >= self.bubble_count)
		remove(self);
};

void CBasePlayer::DeathBubbles(float num_bubbles)
{
	entity bubble_spawner;

	bubble_spawner = spawn();
	gpEngine->pfnSetOrigin(bubble_spawner, self.origin);
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
	if(self.waterlevel == 3)
	{
		DeathBubbles(5);
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/h2odeath.wav", 1, ATTN_NONE);
		return;
	};

	rs = rint((random() * 4) + 1);
	if(rs == 1)
		self.noise = "player/death1.wav";
	if(rs == 2)
		self.noise = "player/death2.wav";
	if(rs == 3)
		self.noise = "player/death3.wav";
	if(rs == 4)
		self.noise = "player/death4.wav";
	if(rs == 5)
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

	if(vlen(damage_inflictor.velocity) > 0)
	{
		v = 0.5 * damage_inflictor.velocity;
		v = v + (25 * normalize(self.origin - damage_inflictor.origin));
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

	if(dm > -50)
	{
		//      dprint ("level 1\n");
		v = v * 0.7;
	}
	else if(dm > -200)
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
	gpEngine->pfnSetModel(new, gibname);
	gpEngine->pfnSetSize(new, '0 0 0', '0 0 0');
	new.velocity = VelocityForDamage(dm);
	new.movetype = MOVETYPE_BOUNCE;
	new.solid = SOLID_NOT;
	new.avelocity_x = random() * 600;
	new.avelocity_y = random() * 600;
	new.avelocity_z = random() * 600;
	new.think = SUB_Remove;
	new.ltime = time;
	new.nextthink = time + 10 + random() * 10;
	new.frame = 0;
	new.flags = 0;
};

void CBasePlayer::ThrowHead(string gibname, float dm)
{
	gpEngine->pfnSetModel(self, gibname);
	self.frame = 0;
	self.nextthink = -1;
	self.movetype = MOVETYPE_BOUNCE;
	self.takedamage = DAMAGE_NO;
	self.solid = SOLID_NOT;
	self.view_ofs = '0 0 8';
	gpEngine->pfnSetSize(self, '-16 -16 0', '16 16 56');
	self.velocity = VelocityForDamage(dm);
	self.origin_z = self.origin_z - 24;
	self.flags = self.flags - (self.flags & FL_ONGROUND);
	self.avelocity = crandom() * '0 600 0';
};

void CBasePlayer::GibPlayer()
{
	ThrowHead("models/h_player.mdl", self.health);
	ThrowGib("models/gib1.mdl", self.health);
	ThrowGib("models/gib2.mdl", self.health);
	ThrowGib("models/gib3.mdl", self.health);

	self.deadflag = DEAD_DEAD;

	if(damage_attacker.classname == "teledeath")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};

	if(damage_attacker.classname == "teledeath2")
	{
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/teledth1.wav", 1, ATTN_NONE);
		return;
	};

	if(random() < 0.5)
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/gib.wav", 1, ATTN_NONE);
	else
		gpEngine->pfnEmitSound(self, CHAN_VOICE, "player/udeath.wav", 1, ATTN_NONE);
};

void CBasePlayer::Die()
{
	float i;
	string s;

	self.items = self.items - (self.items & IT_INVISIBILITY);

	if((stof(infokey(world, "dq"))) != 0)
	{
		if(self.super_damage_finished > 0)
		{
			DropQuad(self.super_damage_finished - time);
			bprint(PRINT_LOW, self.netname);
			if(deathmatch == 4)
				bprint(PRINT_LOW, " lost an OctaPower with ");
			else
				bprint(PRINT_LOW, " lost a quad with ");
			s = ftos(rint(self.super_damage_finished - time));
			bprint(PRINT_LOW, s);
			bprint(PRINT_LOW, " seconds remaining\n");
		};
	};

	if((stof(infokey(world, "dr"))) != 0)
	{
		if(self.invisible_finished > 0)
		{
			bprint(PRINT_LOW, self.netname);
			bprint(PRINT_LOW, " lost a ring with ");
			s = ftos(rint(self.invisible_finished - time));
			bprint(PRINT_LOW, s);
			bprint(PRINT_LOW, " seconds remaining\n");
			DropRing(self.invisible_finished - time);
		};
	};

	self.invisible_finished = 0; // don't die as eyes
	self.invincible_finished = 0;
	self.super_damage_finished = 0;
	self.radsuit_finished = 0;
	self.modelindex = modelindex_player; // don't use eyes

	DropBackpack();

	strcpy(self.weaponmodel, "");
	
	self.view_ofs = '0 0 -8';
	self.deadflag = DEAD_DYING;
	self.solid = SOLID_NOT;
	self.flags = self.flags - (self.flags & FL_ONGROUND);
	self.movetype = MOVETYPE_TOSS;
	if(self.velocity_z < 10)
		self.velocity_z = self.velocity_z + random() * 300;

	if(self.health < -40)
	{
		GibPlayer();
		return;
	};

	DeathSound();

	self.angles_x = 0;
	self.angles_z = 0;

	if(self.weapon == IT_AXE)
	{
		player_die_ax1();
		return;
	};

	i = cvar("temp1");
	if(!i)
		i = 1 + floor(random() * 6);

	if(i == 1)
		player_diea1();
	else if(i == 2)
		player_dieb1();
	else if(i == 3)
		player_diec1();
	else if(i == 4)
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

	if(!(self.ammo_shells + self.ammo_nails + self.ammo_rockets + self.ammo_cells))
		return; // nothing in it

	item = spawn();
	item.origin = self.origin - '0 0 24';

	item.items = self.weapon;
	if(item.items == IT_AXE)
		item.netname = "Axe";
	else if(item.items == IT_SHOTGUN)
		item.netname = "Shotgun";
	else if(item.items == IT_SUPER_SHOTGUN)
		item.netname = "Double-barrelled Shotgun";
	else if(item.items == IT_NAILGUN)
		item.netname = "Nailgun";
	else if(item.items == IT_SUPER_NAILGUN)
		item.netname = "Super Nailgun";
	else if(item.items == IT_GRENADE_LAUNCHER)
		item.netname = "Grenade Launcher";
	else if(item.items == IT_ROCKET_LAUNCHER)
		item.netname = "Rocket Launcher";
	else if(item.items == IT_LIGHTNING)
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
	gpEngine->pfnSetModel(item, "models/backpack.mdl");
	gpEngine->pfnSetSize(item, '-16 -16 0', '16 16 56');
	item.SetTouchCallback(BackpackTouch);

	item.nextthink = time + 120; // remove after 2 minutes
	item.SetThinkCallback(SUB_Remove);
};

float CBasePlayer::GetBestWeapon()
{
	float it = self.items;

	if(self.waterlevel <= 1 && self.ammo_cells >= 1 && (it & IT_LIGHTNING))
		return IT_LIGHTNING;
	else if(self.ammo_nails >= 2 && (it & IT_SUPER_NAILGUN))
		return IT_SUPER_NAILGUN;
	else if(self.ammo_shells >= 2 && (it & IT_SUPER_SHOTGUN))
		return IT_SUPER_SHOTGUN;
	else if(self.ammo_nails >= 1 && (it & IT_NAILGUN))
		return IT_NAILGUN;
	else if(self.ammo_shells >= 1 && (it & IT_SHOTGUN))
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
	if(self->currentammo > 0)
		return true;

	if(self->GetCurrentWeaponID() == IT_AXE)
		return true;

	self->weapon = GetBestWeapon();

	SetCurrentAmmo();

	// drop the weapon down
	return false;
};

void set_suicide_frame(entvars_t *self)
{
	// used by kill and diconnect commands
	if(strcmp(self->model, "models/player.mdl"))
		return; // already gibbed

	self->frame = $deatha11;
	self->solid = SOLID_NOT;
	self->movetype = MOVETYPE_TOSS;
	self->deadflag = DEAD_DEAD;
	self->nextthink = -1;
};

// "animations"
/*
void() player_diea1 = [$deatha1, player_diea2] {};
void() player_diea2 = [$deatha2, player_diea3] {};
void() player_diea3 = [$deatha3, player_diea4] {};
void() player_diea4 = [$deatha4, player_diea5] {};
void() player_diea5 = [$deatha5, player_diea6] {};
void() player_diea6 = [$deatha6, player_diea7] {};
void() player_diea7 = [$deatha7, player_diea8] {};
void() player_diea8 = [$deatha8, player_diea9] {};
void() player_diea9 = [$deatha9, player_diea10] {};
void() player_diea10 = [$deatha10, player_diea11] {};
void() player_diea11 = [$deatha11, player_diea11] { PlayerDead(); };

void() player_dieb1 = [$deathb1, player_dieb2] {};
void() player_dieb2 = [$deathb2, player_dieb3] {};
void() player_dieb3 = [$deathb3, player_dieb4] {};
void() player_dieb4 = [$deathb4, player_dieb5] {};
void() player_dieb5 = [$deathb5, player_dieb6] {};
void() player_dieb6 = [$deathb6, player_dieb7] {};
void() player_dieb7 = [$deathb7, player_dieb8] {};
void() player_dieb8 = [$deathb8, player_dieb9] {};
void() player_dieb9 = [$deathb9, player_dieb9] { PlayerDead(); };

void() player_diec1 = [$deathc1, player_diec2] {};
void() player_diec2 = [$deathc2, player_diec3] {};
void() player_diec3 = [$deathc3, player_diec4] {};
void() player_diec4 = [$deathc4, player_diec5] {};
void() player_diec5 = [$deathc5, player_diec6] {};
void() player_diec6 = [$deathc6, player_diec7] {};
void() player_diec7 = [$deathc7, player_diec8] {};
void() player_diec8 = [$deathc8, player_diec9] {};
void() player_diec9 = [$deathc9, player_diec10] {};
void() player_diec10 = [$deathc10, player_diec11] {};
void() player_diec11 = [$deathc11, player_diec12] {};
void() player_diec12 = [$deathc12, player_diec13] {};
void() player_diec13 = [$deathc13, player_diec14] {};
void() player_diec14 = [$deathc14, player_diec15] {};
void() player_diec15 = [$deathc15, player_diec15] { PlayerDead(); };

void() player_died1 = [$deathd1, player_died2] {};
void() player_died2 = [$deathd2, player_died3] {};
void() player_died3 = [$deathd3, player_died4] {};
void() player_died4 = [$deathd4, player_died5] {};
void() player_died5 = [$deathd5, player_died6] {};
void() player_died6 = [$deathd6, player_died7] {};
void() player_died7 = [$deathd7, player_died8] {};
void() player_died8 = [$deathd8, player_died9] {};
void() player_died9 = [$deathd9, player_died9] { PlayerDead(); };

void() player_diee1 = [$deathe1, player_diee2] {};
void() player_diee2 = [$deathe2, player_diee3] {};
void() player_diee3 = [$deathe3, player_diee4] {};
void() player_diee4 = [$deathe4, player_diee5] {};
void() player_diee5 = [$deathe5, player_diee6] {};
void() player_diee6 = [$deathe6, player_diee7] {};
void() player_diee7 = [$deathe7, player_diee8] {};
void() player_diee8 = [$deathe8, player_diee9] {};
void() player_diee9 = [$deathe9, player_diee9] { PlayerDead(); };

void() player_die_ax1 = [$axdeth1, player_die_ax2] {};
void() player_die_ax2 = [$axdeth2, player_die_ax3] {};
void() player_die_ax3 = [$axdeth3, player_die_ax4] {};
void() player_die_ax4 = [$axdeth4, player_die_ax5] {};
void() player_die_ax5 = [$axdeth5, player_die_ax6] {};
void() player_die_ax6 = [$axdeth6, player_die_ax7] {};
void() player_die_ax7 = [$axdeth7, player_die_ax8] {};
void() player_die_ax8 = [$axdeth8, player_die_ax9] {};
void() player_die_ax9 = [$axdeth9, player_die_ax9] { PlayerDead(); };
*/

bool CBasePlayer::Pickup_Weapon(edict_t *ent, CBasePlayer *other) // TODO: this probably related to weapon class...
{
	int index;
	gitem_t *ammo;

	index = ITEM_INDEX(ent->item);

	if((((int)(dmflags->value) & DF_WEAPONS_STAY) || coop->value) && other->pers.inventory[index])
	{
		if(!(ent->spawnflags & (DROPPED_ITEM | DROPPED_PLAYER_ITEM)))
			return false; // leave the weapon for others to pickup
	}

	other->pers.inventory[index]++;

	if(!(ent->spawnflags & DROPPED_ITEM))
	{
		// give them some ammo with it
		ammo = FindItem(ent->item->ammo);
		if((int)dmflags->value & DF_INFINITE_AMMO)
			other->Add_Ammo(ammo, 1000);
		else
			other->Add_Ammo(ammo, ammo->quantity);

		if(!(ent->spawnflags & DROPPED_PLAYER_ITEM))
		{
			if(deathmatch->value)
			{
				if((int)(dmflags->value) & DF_WEAPONS_STAY)
					ent->flags |= FL_RESPAWN;
				else
					SetRespawn(ent, 30);
			}
			if(coop->value)
				ent->flags |= FL_RESPAWN;
		}
	}

	if(other->pers.weapon != ent->item &&
	   (other->pers.inventory[index] == 1) &&
	   (!deathmatch->value || other->pers.weapon == FindItem("blaster")))
		other->newweapon = ent->item;

	return true;
}

/*
===============
ChangeWeapon

The old weapon has been dropped all the way, so make the new one
current
===============
*/
void CBasePlayer::ChangeWeapon(edict_t *ent)
{
	int i;

	if(this->grenade_time)
	{
		this->grenade_time = level.time;
		this->weapon_sound = 0;
		weapon_grenade_fire(this, false);
		this->grenade_time = 0;
	};

	this->lastweapon = this->pers.weapon;
	this->weapon = this->newweapon;
	this->newweapon = nullptr;
	this->machinegun_shots = 0;

	// set visible model
	if(ent->s.modelindex == 255)
	{
		if(this->pers.weapon)
			i = ((this->pers.weapon->weapmodel & 0xff) << 8);
		else
			i = 0;
		ent->s.skinnum = (ent - g_edicts - 1) | i;
	}

	if(this->pers.weapon && this->pers.weapon->ammo)
		this->ammo_index = ITEM_INDEX(FindItem(this->pers.weapon->ammo));
	else
		this->ammo_index = 0;

	if(!this->pers.weapon)
	{ // dead
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
	if(this->pers.inventory[ITEM_INDEX(FindItem("slugs"))] && this->pers.inventory[ITEM_INDEX(FindItem("railgun"))])
	{
		this->newweapon = FindItem("railgun");
		return;
	}
	if(this->pers.inventory[ITEM_INDEX(FindItem("cells"))] && this->pers.inventory[ITEM_INDEX(FindItem("hyperblaster"))])
	{
		this->newweapon = FindItem("hyperblaster");
		return;
	}
	if(this->pers.inventory[ITEM_INDEX(FindItem("bullets"))] && this->pers.inventory[ITEM_INDEX(FindItem("chaingun"))])
	{
		this->newweapon = FindItem("chaingun");
		return;
	}
	if(this->pers.inventory[ITEM_INDEX(FindItem("bullets"))] && this->pers.inventory[ITEM_INDEX(FindItem("machinegun"))])
	{
		this->newweapon = FindItem("machinegun");
		return;
	}
	if(this->pers.inventory[ITEM_INDEX(FindItem("shells"))] > 1 && this->pers.inventory[ITEM_INDEX(FindItem("super shotgun"))])
	{
		this->newweapon = FindItem("super shotgun");
		return;
	}
	if(this->pers.inventory[ITEM_INDEX(FindItem("shells"))] && this->pers.inventory[ITEM_INDEX(FindItem("shotgun"))])
	{
		this->newweapon = FindItem("shotgun");
		return;
	}
	this->newweapon = FindItem("blaster");
}

/*
=================
Think_Weapon

Called by ClientBeginServerFrame and ClientThink
=================
*/
void CBasePlayer::Think_Weapon(edict_t *ent)
{
	// if just died, put the weapon away
	if(this->health < 1)
	{
		this->newweapon = nullptr;
		this->ChangeWeapon();
	}

	// call active weapon think routine
	if(this->pers.weapon && this->pers.weapon->weaponthink)
	{
		is_quad = (this->quad_framenum > level.framenum);
		if(this->silencer_shots)
			is_silenced = MZ_SILENCED;
		else
			is_silenced = 0;
		this->pers.weapon->weaponthink(ent);
	}
}

/*
================
Use_Weapon

Make the weapon ready if there is ammo
================
*/
void CBasePlayer::Use_Weapon(gitem_t *item)
{
	int ammo_index;
	gitem_t *ammo_item;

	// see if we're already using it
	if(item == this->GetCurrentWeapon())
		return;

	if(item->ammo && !g_select_empty->value && !(item->flags & IT_AMMO))
	{
		ammo_item = FindItem(item->ammo);
		ammo_index = ITEM_INDEX(ammo_item);

		if(!this->pers.inventory[ammo_index])
		{
			gi.cprintf(ent, PRINT_HIGH, "No %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
			return;
		}

		if(this->pers.inventory[ammo_index] < item->quantity)
		{
			gi.cprintf(ent, PRINT_HIGH, "Not enough %s for %s.\n", ammo_item->pickup_name, item->pickup_name);
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
	if((int)(dmflags->value) & DF_WEAPONS_STAY)
		return;

	int index = ITEM_INDEX(item);

	// see if we're already using it
	if(((item == this->GetCurrentWeapon()) || (item == this->newweapon)) && (this->pers.inventory[index] == 1))
	{
		gi.cprintf(this, PRINT_HIGH, "Can't drop current weapon\n");
		return;
	};

	Drop_Item(this, item);
	this->pers.inventory[index]--;
}

/*
===============
CBasePlayer::Reload
===============
*/
// from idtech4
void CBasePlayer::Reload()
{
	if(spectating || gameLocal.inCinematic || influenceActive)
		return;

	if(common->IsClient() && !IsLocallyControlled())
		return;

	if(myCurrentWeapon.GetEntity() && myCurrentWeapon.GetEntity()->IsLinked())
		myCurrentWeapon.GetEntity()->Reload();
};

/*
===============
CBasePlayer::RaiseWeapon
===============
*/
void CBasePlayer::RaiseWeapon()
{
	if(myCurrentWeapon.GetEntity() && myCurrentWeapon.GetEntity()->IsHidden())
		myCurrentWeapon.GetEntity()->RaiseWeapon();
};

/*
===============
CBasePlayer::LowerWeapon
===============
*/
void CBasePlayer::LowerWeapon()
{
	if(myCurrentWeapon.GetEntity() && !myCurrentWeapon.GetEntity()->IsHidden())
		myCurrentWeapon.GetEntity()->LowerWeapon();
};

/*
===============
CBasePlayer::FlashlightOn
===============
*/
void CBasePlayer::FlashlightOn()
{
	if(!flashlight.IsValid())
		return;

	if(flashlightBattery < idMath::Ftoi(flashlight_minActivatePercent.GetFloat() * flashlight_batteryDrainTimeMS.GetFloat()))
		return;

	if(gameLocal.inCinematic)
		return;

	if(flashlight.GetEntity()->lightOn)
		return;

	if(health <= 0)
		return;

	if(spectating)
		return;

	flashlight->FlashlightOn();
};

/*
===============
CBasePlayer::FlashlightOff
===============
*/
void CBasePlayer::FlashlightOff()
{
	if(!flashlight.IsValid())
		return;

	if(!flashlight.GetEntity()->lightOn)
		return;

	flashlight->FlashlightOff();
};

/*
===============
CBasePlayer::DropFlag()
==============
*/
void CBasePlayer::DropFlag()
{
	if(!carryingFlag || !mpGame->GetRules()->IsMultiplayer() || !gameLocal.mpGame.IsGametypeFlagBased()) /* CTF */
		return;

	CBaseEntity *entity = gameLocal.mpGame.GetTeamFlag(1 - team);
	if(entity)
	{
		CItemTeam *item = static_cast<CItemTeam *>(entity);

		if(item->carried && !item->dropped)
		{
			item->Drop(health <= 0);
			carryingFlag = false;
		};
	};
};

void CBasePlayer::ReturnFlag()
{
	if(!carryingFlag || !mpGame->GetRules()->IsMultiplayer() || !gameLocal.mpGame.IsGametypeFlagBased()) /* CTF */
		return;

	CBaseEntity *entity = gameLocal.mpGame.GetTeamFlag(1 - team);
	if(entity)
	{
		CItemTeam *item = static_cast<CItemTeam *>(entity);

		if(item->carried && !item->dropped)
		{
			item->Return();
			carryingFlag = false;
		};
	};
};

/*
===========
CBasePlayer::Save
===========
*/
void CBasePlayer::Save(idSaveGame *savefile) const
{
	int i;

	savefile->WriteUsercmd(usercmd);
	playerView.Save(savefile);

	savefile->WriteBool(noclip);
	savefile->WriteBool(godmode);

	// don't save spawnAnglesSet, since we'll have to reset them after loading the savegame
	savefile->WriteAngles(spawnAngles);
	savefile->WriteAngles(viewAngles);
	savefile->WriteAngles(cmdAngles);

	savefile->WriteInt(buttonMask);
	savefile->WriteInt(oldButtons);
	savefile->WriteInt(oldImpulseSequence);

	savefile->WriteInt(lastHitTime);
	savefile->WriteInt(lastSndHitTime);
	savefile->WriteInt(lastSavingThrowTime);

	// idBoolFields don't need to be saved, just re-linked in Restore

	savefile->WriteObject(primaryObjective);
	inventory.Save(savefile);
	weapon.Save(savefile);

	for(int i = 0; i < NUM_QUICK_SLOTS; ++i)
	{
		savefile->WriteInt(quickSlot[i]);
	}

	savefile->WriteInt(weapon_pda);
	savefile->WriteInt(weapon_fists);
	savefile->WriteInt(weapon_flashlight);
	savefile->WriteInt(weapon_chainsaw);
	savefile->WriteBool(harvest_lock);
	savefile->WriteInt(hudPowerup);
	savefile->WriteInt(lastHudPowerup);
	savefile->WriteInt(hudPowerupDuration);

	savefile->WriteInt(heartRate);

	savefile->WriteFloat(heartInfo.GetStartTime());
	savefile->WriteFloat(heartInfo.GetDuration());
	savefile->WriteFloat(heartInfo.GetStartValue());
	savefile->WriteFloat(heartInfo.GetEndValue());

	savefile->WriteInt(lastHeartAdjust);
	savefile->WriteInt(lastHeartBeat);
	savefile->WriteInt(lastDmgTime);
	savefile->WriteInt(deathClearContentsTime);
	savefile->WriteBool(doingDeathSkin);
	savefile->WriteInt(lastArmorPulse);
	savefile->WriteFloat(stamina);
	savefile->WriteFloat(healthPool);
	savefile->WriteInt(nextHealthPulse);
	savefile->WriteBool(healthPulse);
	savefile->WriteInt(nextHealthTake);
	savefile->WriteBool(healthTake);

	savefile->WriteBool(hiddenWeapon);
	soulCubeProjectile.Save(savefile);

	savefile->WriteInt(spectator);
	savefile->WriteBool(forceScoreBoard);
	savefile->WriteBool(forceRespawn);
	savefile->WriteBool(spectating);
	savefile->WriteInt(lastSpectateTeleport);
	savefile->WriteBool(lastHitToggle);
	savefile->WriteBool(wantSpectate);
	savefile->WriteBool(weaponGone);
	savefile->WriteBool(useInitialSpawns);
	savefile->WriteInt(tourneyRank);
	savefile->WriteInt(tourneyLine);

	teleportEntity.Save(savefile);
	savefile->WriteInt(teleportKiller);

	savefile->WriteInt(minRespawnTime);
	savefile->WriteInt(maxRespawnTime);

	savefile->WriteVec3(firstPersonViewOrigin);
	savefile->WriteMat3(firstPersonViewAxis);

	// don't bother saving dragEntity since it's a dev tool

	savefile->WriteJoint(hipJoint);
	savefile->WriteJoint(chestJoint);
	savefile->WriteJoint(headJoint);

	savefile->WriteStaticObject(physicsObj);

	savefile->WriteInt(aasLocation.Num());
	for(i = 0; i < aasLocation.Num(); i++)
	{
		savefile->WriteInt(aasLocation[i].areaNum);
		savefile->WriteVec3(aasLocation[i].pos);
	}

	savefile->WriteInt(bobFoot);
	savefile->WriteFloat(bobFrac);
	savefile->WriteFloat(bobfracsin);
	savefile->WriteInt(bobCycle);
	savefile->WriteFloat(xyspeed);
	savefile->WriteInt(stepUpTime);
	savefile->WriteFloat(stepUpDelta);
	savefile->WriteFloat(idealLegsYaw);
	savefile->WriteFloat(legsYaw);
	savefile->WriteBool(legsForward);
	savefile->WriteFloat(oldViewYaw);
	savefile->WriteAngles(viewBobAngles);
	savefile->WriteVec3(viewBob);
	savefile->WriteInt(landChange);
	savefile->WriteInt(landTime);

	savefile->WriteInt(currentWeapon);
	savefile->WriteInt(idealWeapon.Get());
	savefile->WriteInt(previousWeapon);
	savefile->WriteInt(weaponSwitchTime);
	savefile->WriteBool(weaponEnabled);

	savefile->WriteInt(skinIndex);
	savefile->WriteSkin(skin);
	savefile->WriteSkin(powerUpSkin);

	savefile->WriteInt(numProjectilesFired);
	savefile->WriteInt(numProjectileHits);

	savefile->WriteBool(airless);
	savefile->WriteInt(airMsec);
	savefile->WriteInt(lastAirDamage);

	savefile->WriteBool(gibDeath);
	savefile->WriteBool(gibsLaunched);
	savefile->WriteVec3(gibsDir);

	savefile->WriteFloat(zoomFov.GetStartTime());
	savefile->WriteFloat(zoomFov.GetDuration());
	savefile->WriteFloat(zoomFov.GetStartValue());
	savefile->WriteFloat(zoomFov.GetEndValue());

	savefile->WriteFloat(centerView.GetStartTime());
	savefile->WriteFloat(centerView.GetDuration());
	savefile->WriteFloat(centerView.GetStartValue());
	savefile->WriteFloat(centerView.GetEndValue());

	savefile->WriteBool(fxFov);

	savefile->WriteFloat(influenceFov);
	savefile->WriteInt(influenceActive);
	savefile->WriteFloat(influenceRadius);
	savefile->WriteObject(influenceEntity);
	savefile->WriteMaterial(influenceMaterial);
	savefile->WriteSkin(influenceSkin);

	savefile->WriteObject(privateCameraView);

	for(i = 0; i < NUM_LOGGED_VIEW_ANGLES; i++)
	{
		savefile->WriteAngles(loggedViewAngles[i]);
	}
	for(i = 0; i < NUM_LOGGED_ACCELS; i++)
	{
		savefile->WriteInt(loggedAccel[i].time);
		savefile->WriteVec3(loggedAccel[i].dir);
	}
	savefile->WriteInt(currentLoggedAccel);

	savefile->WriteObject(focusGUIent);
	// can't save focusUI
	savefile->WriteObject(focusCharacter);
	savefile->WriteInt(talkCursor);
	savefile->WriteInt(focusTime);
	savefile->WriteObject(focusVehicle);
	savefile->WriteUserInterface(cursor, false);

	savefile->WriteInt(oldMouseX);
	savefile->WriteInt(oldMouseY);

	savefile->WriteBool(tipUp);
	savefile->WriteBool(objectiveUp);

	savefile->WriteInt(lastDamageDef);
	savefile->WriteVec3(lastDamageDir);
	savefile->WriteInt(lastDamageLocation);
	savefile->WriteInt(smoothedFrame);
	savefile->WriteBool(smoothedOriginUpdated);
	savefile->WriteVec3(smoothedOrigin);
	savefile->WriteAngles(smoothedAngles);

	savefile->WriteBool(respawning);
	savefile->WriteBool(leader);
	savefile->WriteInt(lastSpectateChange);
	savefile->WriteInt(lastTeleFX);

	savefile->WriteFloat(pm_stamina.GetFloat());

	// TODO_SPARTY hook this up with new hud
	//if ( hud ) {
	//	hud->SetStateString( "message", idLocalization::GetString( "#str_02916" ) );
	//	hud->HandleNamedEvent( "Message" );
	//}

	savefile->WriteInt(weaponToggles.Num());
	for(i = 0; i < weaponToggles.Num(); i++)
	{
		WeaponToggle_t *weaponToggle = weaponToggles.GetIndex(i);
		savefile->WriteString(weaponToggle->name);
		savefile->WriteInt(weaponToggle->toggleList.Num());
		for(int j = 0; j < weaponToggle->toggleList.Num(); j++)
		{
			savefile->WriteInt(weaponToggle->toggleList[j]);
		}
	}
	savefile->WriteObject(mountedObject);
	enviroSuitLight.Save(savefile);
	savefile->WriteBool(healthRecharge);
	savefile->WriteInt(lastHealthRechargeTime);
	savefile->WriteInt(rechargeSpeed);
	savefile->WriteFloat(new_g_damageScale);

	savefile->WriteBool(bloomEnabled);
	savefile->WriteFloat(bloomSpeed);
	savefile->WriteFloat(bloomIntensity);

	savefile->WriteObject(flashlight.GetEntity());
	savefile->WriteInt(flashlightBattery);

	achievementManager.Save(savefile);

	savefile->WriteInt(playedTimeSecs);
	savefile->WriteInt(playedTimeResidual);

	for(int i = 0; i < MAX_PLAYER_PDA; i++)
	{
		savefile->WriteBool(pdaHasBeenRead[i]);
	}

	for(int i = 0; i < MAX_PLAYER_VIDEO; i++)
	{
		savefile->WriteBool(videoHasBeenViewed[i]);
	}

	for(int i = 0; i < MAX_PLAYER_AUDIO; i++)
	{
		for(int j = 0; j < MAX_PLAYER_AUDIO_ENTRIES; j++)
		{
			savefile->WriteBool(audioHasBeenHeard[i][j]);
		}
	}
}

/*
===========
CBasePlayer::Restore
===========
*/
void CBasePlayer::Restore(idRestoreGame *savefile)
{
	int i;
	int num;
	float set;

	savefile->ReadUsercmd(usercmd);
	playerView.Restore(savefile);

	savefile->ReadBool(noclip);
	savefile->ReadBool(godmode);

	savefile->ReadAngles(spawnAngles);
	savefile->ReadAngles(viewAngles);
	savefile->ReadAngles(cmdAngles);

	memset(usercmd.angles, 0, sizeof(usercmd.angles));
	SetViewAngles(viewAngles);
	spawnAnglesSet = true;

	savefile->ReadInt(buttonMask);
	savefile->ReadInt(oldButtons);
	savefile->ReadInt(oldImpulseSequence);

	usercmd.impulseSequence = 0;
	oldImpulseSequence = 0;

	savefile->ReadInt(lastHitTime);
	savefile->ReadInt(lastSndHitTime);
	savefile->ReadInt(lastSavingThrowTime);

	// Re-link idBoolFields to the scriptObject, values will be restored in scriptObject's restore
	LinkScriptVariables();

	savefile->ReadObject(reinterpret_cast<idClass *&>(primaryObjective));
	inventory.Restore(savefile);
	weapon.Restore(savefile);

	if(hudManager != nullptr)
	{
		hudManager->Initialize("hud", common->SW());
		hudManager->ActivateMenu(true);
		hud = hudManager->GetHud();
	}

	if(pdaMenu != nullptr)
	{
		pdaMenu->Initialize("pda", common->SW());
	}

	for(i = 0; i < inventory.emails.Num(); i++)
	{
		GetPDA()->AddEmail(inventory.emails[i]);
	}

	for(int i = 0; i < NUM_QUICK_SLOTS; ++i)
	{
		savefile->ReadInt(quickSlot[i]);
	}

	savefile->ReadInt(weapon_pda);
	savefile->ReadInt(weapon_fists);
	savefile->ReadInt(weapon_flashlight);
	savefile->ReadInt(weapon_chainsaw);

	savefile->ReadBool(harvest_lock);
	savefile->ReadInt(hudPowerup);
	savefile->ReadInt(lastHudPowerup);
	savefile->ReadInt(hudPowerupDuration);

	savefile->ReadInt(heartRate);

	savefile->ReadFloat(set);
	heartInfo.SetStartTime(set);
	savefile->ReadFloat(set);
	heartInfo.SetDuration(set);
	savefile->ReadFloat(set);
	heartInfo.SetStartValue(set);
	savefile->ReadFloat(set);
	heartInfo.SetEndValue(set);

	savefile->ReadInt(lastHeartAdjust);
	savefile->ReadInt(lastHeartBeat);
	savefile->ReadInt(lastDmgTime);
	savefile->ReadInt(deathClearContentsTime);
	savefile->ReadBool(doingDeathSkin);
	savefile->ReadInt(lastArmorPulse);
	savefile->ReadFloat(stamina);
	savefile->ReadFloat(healthPool);
	savefile->ReadInt(nextHealthPulse);
	savefile->ReadBool(healthPulse);
	savefile->ReadInt(nextHealthTake);
	savefile->ReadBool(healthTake);

	savefile->ReadBool(hiddenWeapon);
	soulCubeProjectile.Restore(savefile);

	savefile->ReadInt(spectator);
	savefile->ReadBool(forceScoreBoard);
	savefile->ReadBool(forceRespawn);
	savefile->ReadBool(spectating);
	savefile->ReadInt(lastSpectateTeleport);
	savefile->ReadBool(lastHitToggle);
	savefile->ReadBool(wantSpectate);
	savefile->ReadBool(weaponGone);
	savefile->ReadBool(useInitialSpawns);
	savefile->ReadInt(tourneyRank);
	savefile->ReadInt(tourneyLine);

	teleportEntity.Restore(savefile);
	savefile->ReadInt(teleportKiller);

	savefile->ReadInt(minRespawnTime);
	savefile->ReadInt(maxRespawnTime);

	savefile->ReadVec3(firstPersonViewOrigin);
	savefile->ReadMat3(firstPersonViewAxis);

	// don't bother saving dragEntity since it's a dev tool
	dragEntity.Clear();

	savefile->ReadJoint(hipJoint);
	savefile->ReadJoint(chestJoint);
	savefile->ReadJoint(headJoint);

	savefile->ReadStaticObject(physicsObj);
	RestorePhysics(&physicsObj);

	savefile->ReadInt(num);
	aasLocation.SetGranularity(1);
	aasLocation.SetNum(num);
	for(i = 0; i < num; i++)
	{
		savefile->ReadInt(aasLocation[i].areaNum);
		savefile->ReadVec3(aasLocation[i].pos);
	}

	savefile->ReadInt(bobFoot);
	savefile->ReadFloat(bobFrac);
	savefile->ReadFloat(bobfracsin);
	savefile->ReadInt(bobCycle);
	savefile->ReadFloat(xyspeed);
	savefile->ReadInt(stepUpTime);
	savefile->ReadFloat(stepUpDelta);
	savefile->ReadFloat(idealLegsYaw);
	savefile->ReadFloat(legsYaw);
	savefile->ReadBool(legsForward);
	savefile->ReadFloat(oldViewYaw);
	savefile->ReadAngles(viewBobAngles);
	savefile->ReadVec3(viewBob);
	savefile->ReadInt(landChange);
	savefile->ReadInt(landTime);

	savefile->ReadInt(currentWeapon);

	int savedIdealWeapon = -1;
	savefile->ReadInt(savedIdealWeapon);
	idealWeapon.Set(savedIdealWeapon);

	savefile->ReadInt(previousWeapon);
	savefile->ReadInt(weaponSwitchTime);
	savefile->ReadBool(weaponEnabled);

	savefile->ReadInt(skinIndex);
	savefile->ReadSkin(skin);
	savefile->ReadSkin(powerUpSkin);

	savefile->ReadInt(numProjectilesFired);
	savefile->ReadInt(numProjectileHits);

	savefile->ReadBool(airless);
	savefile->ReadInt(airMsec);
	savefile->ReadInt(lastAirDamage);

	savefile->ReadBool(gibDeath);
	savefile->ReadBool(gibsLaunched);
	savefile->ReadVec3(gibsDir);

	savefile->ReadFloat(set);
	zoomFov.SetStartTime(set);
	savefile->ReadFloat(set);
	zoomFov.SetDuration(set);
	savefile->ReadFloat(set);
	zoomFov.SetStartValue(set);
	savefile->ReadFloat(set);
	zoomFov.SetEndValue(set);

	savefile->ReadFloat(set);
	centerView.SetStartTime(set);
	savefile->ReadFloat(set);
	centerView.SetDuration(set);
	savefile->ReadFloat(set);
	centerView.SetStartValue(set);
	savefile->ReadFloat(set);
	centerView.SetEndValue(set);

	savefile->ReadBool(fxFov);

	savefile->ReadFloat(influenceFov);
	savefile->ReadInt(influenceActive);
	savefile->ReadFloat(influenceRadius);
	savefile->ReadObject(reinterpret_cast<idClass *&>(influenceEntity));
	savefile->ReadMaterial(influenceMaterial);
	savefile->ReadSkin(influenceSkin);

	savefile->ReadObject(reinterpret_cast<idClass *&>(privateCameraView));

	for(i = 0; i < NUM_LOGGED_VIEW_ANGLES; i++)
	{
		savefile->ReadAngles(loggedViewAngles[i]);
	}
	for(i = 0; i < NUM_LOGGED_ACCELS; i++)
	{
		savefile->ReadInt(loggedAccel[i].time);
		savefile->ReadVec3(loggedAccel[i].dir);
	}
	savefile->ReadInt(currentLoggedAccel);

	savefile->ReadObject(reinterpret_cast<idClass *&>(focusGUIent));
	// can't save focusUI
	focusUI = nullptr;
	savefile->ReadObject(reinterpret_cast<idClass *&>(focusCharacter));
	savefile->ReadInt(talkCursor);
	savefile->ReadInt(focusTime);
	savefile->ReadObject(reinterpret_cast<idClass *&>(focusVehicle));
	savefile->ReadUserInterface(cursor);

	savefile->ReadInt(oldMouseX);
	savefile->ReadInt(oldMouseY);

	savefile->ReadBool(tipUp);
	savefile->ReadBool(objectiveUp);

	savefile->ReadInt(lastDamageDef);
	savefile->ReadVec3(lastDamageDir);
	savefile->ReadInt(lastDamageLocation);
	savefile->ReadInt(smoothedFrame);
	savefile->ReadBool(smoothedOriginUpdated);
	savefile->ReadVec3(smoothedOrigin);
	savefile->ReadAngles(smoothedAngles);

	savefile->ReadBool(respawning);
	savefile->ReadBool(leader);
	savefile->ReadInt(lastSpectateChange);
	savefile->ReadInt(lastTeleFX);

	// set the pm_ cvars
	const idKeyValue *kv;
	kv = spawnArgs.MatchPrefix("pm_", nullptr);
	while(kv)
	{
		cvarSystem->SetCVarString(kv->GetKey(), kv->GetValue());
		kv = spawnArgs.MatchPrefix("pm_", kv);
	}

	savefile->ReadFloat(set);
	pm_stamina.SetFloat(set);

	// create combat collision hull for exact collision detection
	SetCombatModel();

	int weaponToggleCount;
	savefile->ReadInt(weaponToggleCount);
	for(i = 0; i < weaponToggleCount; i++)
	{
		WeaponToggle_t newToggle;
		memset(&newToggle, 0, sizeof(newToggle));

		idStr name;
		savefile->ReadString(name);
		strcpy(newToggle.name, name.c_str());

		int indexCount;
		savefile->ReadInt(indexCount);
		for(int j = 0; j < indexCount; j++)
		{
			int temp;
			savefile->ReadInt(temp);
			newToggle.toggleList.Append(temp);
		}
		newToggle.lastUsed = 0;
		weaponToggles.Set(newToggle.name, newToggle);
	}
	savefile->ReadObject(reinterpret_cast<idClass *&>(mountedObject));
	enviroSuitLight.Restore(savefile);
	savefile->ReadBool(healthRecharge);
	savefile->ReadInt(lastHealthRechargeTime);
	savefile->ReadInt(rechargeSpeed);
	savefile->ReadFloat(new_g_damageScale);

	savefile->ReadBool(bloomEnabled);
	savefile->ReadFloat(bloomSpeed);
	savefile->ReadFloat(bloomIntensity);

	// flashlight
	idWeapon *tempWeapon;
	savefile->ReadObject(reinterpret_cast<idClass *&>(tempWeapon));
	tempWeapon->SetIsPlayerFlashlight(true);
	flashlight = tempWeapon;
	savefile->ReadInt(flashlightBattery);

	achievementManager.Restore(savefile);

	savefile->ReadInt(playedTimeSecs);
	savefile->ReadInt(playedTimeResidual);

	aimAssist.Init(this);

	laserSightHandle = -1;

	// re-init the laser model
	memset(&laserSightRenderEntity, 0, sizeof(laserSightRenderEntity));
	laserSightRenderEntity.hModel = renderModelManager->FindModel("_BEAM");
	laserSightRenderEntity.customShader = declManager->FindMaterial("stereoRenderLaserSight");

	for(int i = 0; i < MAX_PLAYER_PDA; i++)
	{
		savefile->ReadBool(pdaHasBeenRead[i]);
	}

	for(int i = 0; i < MAX_PLAYER_VIDEO; i++)
	{
		savefile->ReadBool(videoHasBeenViewed[i]);
	}

	for(int i = 0; i < MAX_PLAYER_AUDIO; i++)
	{
		for(int j = 0; j < MAX_PLAYER_AUDIO_ENTRIES; j++)
		{
			savefile->ReadBool(audioHasBeenHeard[i][j]);
		}
	}

	// Update the soul cube HUD indicator
	if(hud)
	{
		if(weapon_soulcube > 0 && (inventory.weapons & (1 << weapon_soulcube)))
		{
		}
	}
}

/*
===============
CBasePlayer::SavePersistantInfo

Saves any inventory and player stats when changing levels.
===============
*/
void CBasePlayer::SavePersistantInfo()
{
	idDict &playerInfo = gameLocal.persistentPlayerInfo[entityNumber];

	playerInfo.Clear();
	inventory.GetPersistantData(playerInfo);
	playerInfo.SetInt("health", health);
	playerInfo.SetInt("current_weapon", currentWeapon);
	playerInfo.SetInt("playedTime", playedTimeSecs);

	achievementManager.SavePersistentData(playerInfo);
};

/*
===============
CBasePlayer::RestorePersistantInfo

Restores any inventory and player stats when changing levels.
===============
*/
void CBasePlayer::RestorePersistantInfo()
{
	if(mpGame->GetRules()->IsMultiplayer() || g_demoMode.GetBool())
		gameLocal.persistentPlayerInfo[entityNumber].Clear();

	spawnArgs.Copy(gameLocal.persistentPlayerInfo[entityNumber]);

	inventory.RestoreInventory(this, spawnArgs);
	health = spawnArgs.GetInt("health", "100");
	idealWeapon = spawnArgs.GetInt("current_weapon", "1");

	playedTimeSecs = spawnArgs.GetInt("playedTime");

	achievementManager.RestorePersistentData(spawnArgs);
};

/*
===============
CBasePlayer::Weapon_Combat
===============
*/
void CBasePlayer::Weapon_Combat()
{
	if(influenceActive || !weaponEnabled || gameLocal.inCinematic || privateCameraView)
		return;

	weapon.GetEntity()->RaiseWeapon();
	if(weapon.GetEntity()->IsReloading())
	{
		if(!AI_RELOAD)
		{
			AI_RELOAD = true;
			SetState("ReloadWeapon");
			UpdateScript();
		};
	}
	else
		AI_RELOAD = false;

	if(idealWeapon == weapon_soulcube && soulCubeProjectile.GetEntity() != nullptr)
	{
		idealWeapon = currentWeapon;
	}

	if(idealWeapon != currentWeapon && idealWeapon.Get() < MAX_WEAPONS)
	{
		if(weaponCatchup)
		{
			assert(common->IsClient());

			currentWeapon = idealWeapon.Get();
			weaponGone = false;
			animPrefix = spawnArgs.GetString(va("def_weapon%d", currentWeapon));
			weapon.GetEntity()->GetWeaponDef(animPrefix, inventory.GetClipAmmoForWeapon(currentWeapon));
			animPrefix.Strip("weapon_");

			weapon.GetEntity()->NetCatchup();
			const function_t *newstate = GetScriptFunction("NetCatchup");
			if(newstate)
			{
				SetState(newstate);
				UpdateScript();
			}
			weaponCatchup = false;
		}
		else
		{
			if(weapon.GetEntity()->IsReady())
			{
				weapon.GetEntity()->PutAway();
			}

			if(weapon.GetEntity()->IsHolstered())
			{
				assert(idealWeapon.Get() >= 0);
				assert(idealWeapon.Get() < MAX_WEAPONS);

				if(currentWeapon != weapon_pda && !spawnArgs.GetBool(va("weapon%d_toggle", currentWeapon)))
				{
					previousWeapon = currentWeapon;
				}
				currentWeapon = idealWeapon.Get();
				weaponGone = false;
				animPrefix = spawnArgs.GetString(va("def_weapon%d", currentWeapon));
				weapon.GetEntity()->GetWeaponDef(animPrefix, inventory.GetClipAmmoForWeapon(currentWeapon));
				animPrefix.Strip("weapon_");

				weapon.GetEntity()->Raise();
			}
		}
	}
	else
	{
		weaponGone = false; // if you drop and re-get weap, you may miss the = false above
		if(weapon.GetEntity()->IsHolstered())
		{
			if(!weapon.GetEntity()->AmmoAvailable())
			{
				// weapons can switch automatically if they have no more ammo
				NextBestWeapon();
			}
			else
			{
				weapon.GetEntity()->Raise();
				state = GetScriptFunction("RaiseWeapon");
				if(state)
				{
					SetState(state);
				}
			}
		}
	}

	// check for attack
	AI_WEAPON_FIRED = false;
	if(!influenceActive)
	{
		if((usercmd.buttons & BUTTON_ATTACK) && !weaponGone)
			FireWeapon();
		else if(oldButtons & BUTTON_ATTACK)
		{
			AI_ATTACK_HELD = false;
			weapon.GetEntity()->EndAttack();
		};
	};

	// update our ammo clip in our inventory
	if((currentWeapon >= 0) && (currentWeapon < MAX_WEAPONS))
		inventory.SetClipAmmoForWeapon(currentWeapon, weapon.GetEntity()->AmmoInClip());
};

/*
===============
CBasePlayer::WeaponLoweringCallback
===============
*/
void CBasePlayer::WeaponLoweringCallback()
{
	SetState("LowerWeapon");
	UpdateScript();
};

/*
===============
CBasePlayer::WeaponRisingCallback
===============
*/
void CBasePlayer::WeaponRisingCallback()
{
	SetState("RaiseWeapon");
	UpdateScript();
};

/*
===============
CBasePlayer::UpdateSpectating
===============
*/
void CBasePlayer::UpdateSpectating()
{
	assert(spectating);
	//assert(!common->IsClient());
	assert(IsHidden());
	
	if constexpr(IsServer())
	{
		if(!mpGame->GetRules()->IsMultiplayer())
			return;
		
		CBasePlayer *player = gameLocal.GetClientByNum(spectator);
		
		if(!player || (player->spectating && player != this))
			SpectateFreeFly(true);
		else if(usercmd.buttons & BUTTON_JUMP)
			SpectateFreeFly(false);
		else if(usercmd.buttons & BUTTON_USE)
			SpectateCycle();
		else if(usercmd.buttons & BUTTON_ATTACK)
			wantSpectate = false;
	};
};

/*
===============
CBasePlayer::GiveInventoryItem
===============
*/
bool CBasePlayer::GiveItem(idDict *item, unsigned int giveFlags)
{
	if(mpGame->GetRules()->IsMultiplayer() && spectating)
		return false;

	if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		inventory.items.Append(new(TAG_ENTITY) idDict(*item));

	const char *itemName = item->GetString("inv_name");

	if(giveFlags & ITEM_GIVE_FEEDBACK)
	{
		if(idStr::Cmpn(itemName, STRTABLE_ID, STRTABLE_ID_LENGTH) == 0)
			inventory.pickupItemNames.Append(idLocalization::GetString(itemName));
		else
			inventory.pickupItemNames.Append(itemName);

		const char *icon = item->GetString("inv_icon");
		if(hud != nullptr)
			hud->ShowNewItem(itemName, icon);
	};

	// D3XP added to support powercells
	if((giveFlags & ITEM_GIVE_UPDATE_STATE) && item->GetInt("inv_powercell") && focusUI)
	{
		//Reset the powercell count
		int powerCellCount = 0;
		for(int j = 0; j < inventory.items.Num(); j++)
		{
			idDict *item = inventory.items[j];
			if(item->GetInt("inv_powercell"))
			{
				powerCellCount++;
			}
		};
		focusUI->SetStateInt("powercell_count", powerCellCount);
	};

	return true;
};

/*
===============
CBasePlayer::RemoveInventoryItem
===============
*/
void CBasePlayer::RemoveItem(CDict *item)
{
	inventory.items.Remove(item);

	if(item->GetInt("inv_powercell") && focusUI)
	{
		//Reset the powercell count
		int powerCellCount = 0;
		for(int j = 0; j < inventory.items.Num(); j++)
		{
			idDict *item = inventory.items[j];
			if(item->GetInt("inv_powercell"))
			{
				powerCellCount++;
			}
		}
		focusUI->SetStateInt("powercell_count", powerCellCount);
	};

	delete item;
};

/*
==============
CBasePlayer::GiveInventoryItem
==============
*/
bool CBasePlayer::GiveItem(const char *name)
{
	CDict args;

	args.Set("classname", name);
	args.Set("owner", this->name.c_str());
	gameLocal.SpawnEntityDef(args);
	return true;
};

/*
===============
CBasePlayer::RemoveInventoryItem
===============
*/
void CBasePlayer::RemoveItem(const char *name)
{
	CDict *item = FindItem(name);
	if(item)
		RemoveItem(item);
};

/*
===============
CBasePlayer::FindInventoryItem
===============
*/
CDict *CBasePlayer::FindItem(const char *name)
{
	for(int i = 0; i < inventory.items.Num(); i++)
	{
		const char *iname = inventory.items[i]->GetString("inv_name");
		if(iname != nullptr && *iname != nullptr)
			if(idStr::Icmp(name, iname) == 0)
				return inventory.items[i];
	};
	
	return nullptr;
};

/*
===============
CBasePlayer::FindInventoryItem
===============
*/
CDict *CBasePlayer::FindItem(int index)
{
	if(index <= inventory.items.Num())
		return inventory.items[index];
	
	return nullptr;
};

/*
===============
CBasePlayer::GetNumInventoryItems
===============
*/
int CBasePlayer::GetNumInventoryItems() const
{
	return inventory.items.Num();
};

/*
===============
CBasePlayer::EnterCinematic
===============
*/
void CBasePlayer::EnterCinematic()
{
	Hide();
	
	StopSound(SND_CHANNEL_PDA_AUDIO, false);
	StopSound(SND_CHANNEL_PDA_VIDEO, false);

	if(hudManager)
		hudManager->SetRadioMessage(false);

	physicsObj.SetLinearVelocity(vec3_origin);

	SetState("EnterCinematic");
	UpdateScript();

	if(weaponEnabled && weapon.GetEntity())
		weapon.GetEntity()->EnterCinematic();

	if(flashlight.GetEntity())
		flashlight.GetEntity()->EnterCinematic();

	AI_FORWARD = false;
	AI_BACKWARD = false;
	AI_STRAFE_LEFT = false;
	AI_STRAFE_RIGHT = false;
	AI_RUN = false;
	AI_ATTACK_HELD = false;
	AI_WEAPON_FIRED = false;
	AI_JUMP = false;
	AI_CROUCH = false;
	AI_ONGROUND = true;
	AI_ONLADDER = false;
	AI_DEAD = (health <= 0);
	AI_RUN = false;
	AI_PAIN = false;
	AI_HARDLANDING = false;
	AI_SOFTLANDING = false;
	AI_RELOAD = false;
	AI_TELEPORT = false;
	AI_TURN_LEFT = false;
	AI_TURN_RIGHT = false;
};

/*
===============
CBasePlayer::ExitCinematic
===============
*/
void CBasePlayer::ExitCinematic()
{
	Show();

	if(weaponEnabled && weapon.GetEntity())
		weapon.GetEntity()->ExitCinematic();

	if(flashlight.GetEntity())
		flashlight.GetEntity()->ExitCinematic();

	// long cinematics would have surpassed the healthTakeTime, causing the player to take damage
	// immediately after the cinematic ends.  Instead we start the healthTake cooldown again once
	// the cinematic ends.
	if(g_skill.GetInteger() == 3)
		nextHealthTake = gameLocal.time + g_healthTakeTime.GetInteger() * 1000;

	SetState("ExitCinematic");
	UpdateScript();
};

/*
==================
WeaponFireFeedback

Called when a weapon fires, generates head twitches, etc
==================
*/
void CBasePlayer::WeaponFireFeedback(const CDict *weaponDef)
{
	// force a blink
	blink_time = 0;

	// play the fire animation
	AI_WEAPON_FIRED = true;

	// update view feedback
	playerView.WeaponFireFeedback(weaponDef);

	// shake controller
	float highMagnitude = weaponDef->GetFloat("controllerShakeHighMag");
	int highDuration = weaponDef->GetInt("controllerShakeHighTime");
	float lowMagnitude = weaponDef->GetFloat("controllerShakeLowMag");
	int lowDuration = weaponDef->GetInt("controllerShakeLowTime");
	//const char *name = weaponDef->GetString( "inv_name" );

	if(IsLocallyControlled())
		SetControllerShake(highMagnitude, highDuration, lowMagnitude, lowDuration);
};

/*
===============
CBasePlayer::CacheWeapons
===============
*/
void CBasePlayer::CacheWeapons()
{
	idStr weap;
	int w;

	// check if we have any weapons
	if(!inventory.weapons)
		return;

	for(w = 0; w < MAX_WEAPONS; w++)
	{
		if(inventory.weapons & (1 << w))
		{
			weap = spawnArgs.GetString(va("def_weapon%d", w));
			if(weap != "")
			{
				CBaseWeapon::CacheWeapon(weap);
			}
			else
			{
				inventory.weapons &= ~(1 << w);
			}
		}
	}
}

/*
===============
CBasePlayer::SetQuickSlot
===============
*/
void CBasePlayer::SetQuickSlot(int index, int val)
{
	if(index >= NUM_QUICK_SLOTS || index < 0)
		return;

	quickSlot[index] = val;
};

/*
===============
CBasePlayer::GetQuickSlot
===============
*/
int CBasePlayer::GetQuickSlot(int index)
{
	if(index >= NUM_QUICK_SLOTS || index < 0)
		return -1;

	return quickSlot[index];
};

/*
===============
CBasePlayer::Give
===============
*/
bool CBasePlayer::Give(const char *statname, const char *value, unsigned int giveFlags)
{
	int amount;

	if(AI_DEAD)
		return false;

	if(!CStr::Icmp(statname, "health"))
	{
		if(health >= inventory.maxHealth)
		{
			return false;
		}
		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			amount = atoi(value);
			if(amount)
			{
				health += amount;
				if(health > inventory.maxHealth)
				{
					health = inventory.maxHealth;
				}
				healthPulse = true;
			}
		}
	}
	else if(!idStr::Icmp(statname, "stamina"))
	{
		if(stamina >= 100)
		{
			return false;
		}
		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			stamina += atof(value);
			if(stamina > 100)
			{
				stamina = 100;
			}
		}
	}
	else if(!idStr::Icmp(statname, "heartRate"))
	{
		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			heartRate += atoi(value);
			if(heartRate > MAX_HEARTRATE)
			{
				heartRate = MAX_HEARTRATE;
			}
		}
	}
	else if(!idStr::Icmp(statname, "air"))
	{
		if(airMsec >= pm_airMsec.GetInteger())
		{
			return false;
		}
		if(giveFlags & ITEM_GIVE_UPDATE_STATE)
		{
			airMsec += pm_airMsec.GetInteger() * atoi(value) / 100;
			if(airMsec > pm_airMsec.GetInteger())
			{
				airMsec = pm_airMsec.GetInteger();
			}
		}
	}
	else if(!idStr::Icmp(statname, "enviroTime"))
	{
		if((giveFlags & ITEM_GIVE_UPDATE_STATE) && PowerUpActive(ENVIROTIME))
		{
			inventory.powerupEndTime[ENVIROTIME] += (atof(value) * 1000);
		}
		else
		{
			GivePowerUp(ENVIROTIME, atoi(value) * 1000, giveFlags);
		}
	}
	else
	{
		bool ret = inventory.Give(this, spawnArgs, statname, value, &idealWeapon, true, giveFlags);
		return ret;
	}
	return true;
}

/*
===============
CBasePlayer::GiveHealthPool

adds health to the player health pool
===============
*/
void CBasePlayer::GiveHealthPool(float amt)
{
	if(AI_DEAD)
		return;

	if(health > 0)
	{
		healthPool += amt;
		if(healthPool > inventory.maxHealth - health)
		{
			healthPool = inventory.maxHealth - health;
		}
		nextHealthPulse = gameLocal.time;
	};
};

/*
===============
CBasePlayer::PowerUpModifier
===============
*/
float CBasePlayer::PowerUpModifier(int type)
{
	float mod = 1.0f;

	if(PowerUpActive(BERSERK))
	{
		switch(type)
		{
		case SPEED:
			mod *= 1.7f;
			break;
		case PROJECTILE_DAMAGE:
			mod *= 2.0f;
			break;
		case MELEE_DAMAGE:
			mod *= 30.0f;
			break;
		case MELEE_DISTANCE:
			mod *= 2.0f;
			break;
		};
	};

	if constexpr(IsServer())
	{
		if(mpGame->GetRules()->IsMultiplayer())
		{
			if(PowerUpActive(MEGAHEALTH))
			{
				if(healthPool <= 0)
					GiveHealthPool(100);
			}
			else
				healthPool = 0;

			/*if( PowerUpActive( HASTE ) ) {
				switch( type ) {
				case SPEED: {
					mod = 1.7f;
					break;
							}
				}
			}*/
		};
	};

	return mod;
};

/*
===============
CBasePlayer::ClearPowerUps
===============
*/
void CBasePlayer::ClearPowerUps()
{
	for(int i = 0; i < MAX_POWERUPS; i++)
		if(PowerUpActive(i))
			ClearPowerup(i);
	
	inventory.ClearPowerUps();

	if(mpGame->GetRules()->IsMultiplayer())
		if(enviroSuitLight.IsValid())
			enviroSuitLight.GetEntity()->PostEventMS(&EV_Remove, 0);
};

void CBasePlayer::PlayerJump()
{
	vector start, end;

	if(self->GetFlags() & FL_WATERJUMP)
		return;

	if(self->v.waterlevel >= 2)
	{
		// play swiming sound
		if(self->v.swim_flag < time)
		{
			self->v.swim_flag = time + 1;
			if(random() < 0.5)
				EmitSound(CHAN_BODY, "misc/water1.wav", 1, ATTN_NORM);
			else
				EmitSound(CHAN_BODY, "misc/water2.wav", 1, ATTN_NORM);
		};

		return;
	};

	if(!(self->GetFlags() & FL_ONGROUND))
		return;

	if(!(self->GetFlags() & FL_JUMPRELEASED))
		return; // don't pogo stick

	self->v.flags = self->v.flags - (self->v.flags & FL_JUMPRELEASED);
	self->v.button2 = 0;

	// player jumping sound
	EmitSound(CHAN_BODY, "player/plyrjmp8.wav", 1, ATTN_NORM);
};

/*
===========
WaterMove

============
*/
.float dmgtime;

void CBasePlayer::WaterMove()
{
	//dprint (ftos(self.waterlevel));
	if(self->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if(self->GetHealth() < 0)
		return;

	if(self->v.waterlevel != 3)
	{
		if(self->v.air_finished < time)
			EmitSound(CHAN_VOICE, "player/gasp2.wav", 1, ATTN_NORM);
		else if(self->v.air_finished < time + 9)
			EmitSound(CHAN_VOICE, "player/gasp1.wav", 1, ATTN_NORM);
		self->v.air_finished = time + 12;
		self->v.dmg = 2;
	}
	else if(self->v.air_finished < time)
	{
		// drown!
		if(self->v.pain_finished < time)
		{
			self->v.dmg = self->v.dmg + 2;
			if(self->v.dmg > 15)
				self->v.dmg = 10;
			T_Damage(self, world, world, self->v.dmg);
			self->v.pain_finished = time + 1;
		};
	};

	if(!self->v.waterlevel)
	{
		if(self->v.flags & FL_INWATER)
		{
			// play leave water sound
			EmitSound(CHAN_BODY, "misc/outwater.wav", 1, ATTN_NORM);
			self->v.flags = self->v.flags - FL_INWATER;
		};
		return;
	};

	if(self->v.watertype == CONTENT_LAVA)
	{
		// do damage
		if(self->v.dmgtime < time)
		{
			if(self->v.radsuit_finished > time)
				self->v.dmgtime = time + 1;
			else
				self->v.dmgtime = time + 0.2;

			T_Damage(self, world, world, 10 * self->v.waterlevel);
		};
	}
	else if(self->v.watertype == CONTENT_SLIME)
	{
		// do damage
		if(self->v.dmgtime < time && self->v.radsuit_finished < time)
		{
			self->v.dmgtime = time + 1;
			T_Damage(self, world, world, 4 * self->v.waterlevel);
		};
	};

	if(!(self->GetFlags() & FL_INWATER))
	{
		// player enter water sound

		if(self->v.watertype == CONTENT_LAVA)
			EmitSound(CHAN_BODY, "player/inlava.wav", 1, ATTN_NORM);
		if(self->v.watertype == CONTENT_WATER)
			EmitSound(CHAN_BODY, "player/inh2o.wav", 1, ATTN_NORM);
		if(self->v.watertype == CONTENT_SLIME)
			EmitSound(CHAN_BODY, "player/slimbrn2.wav", 1, ATTN_NORM);

		self->v.flags += FL_INWATER;
		self->v.dmgtime = 0;
	}
};

void CBasePlayer::CheckWaterJump(edict_t *self)
{
	vector start, end;

	// check for a jump-out-of-water
	makevectors(self->v.angles);
	start = self->v.origin;
	start_z = start_z + 8;
	v_forward_z = 0;
	normalize(v_forward);
	end = start + v_forward * 24;
	traceline(start, end, TRUE, self);
	if(trace_fraction < 1)
	{ // solid at waist
		start_z = start_z + self->v.maxs_z - 8;
		end = start + v_forward * 24;
		self->v.movedir = trace_plane_normal * -50;
		traceline(start, end, TRUE, self);
		if(trace_fraction == 1)
		{ // open at eye level
			self->v.flags |= FL_WATERJUMP;
			self->v.velocity_z = 225;
			self->v.flags = self->v.flags - (self->v.flags & FL_JUMPRELEASED);
			self->v.teleport_time = time + 2; // safety net
			return;
		};
	};
};

/*
============
W_Attack

An attack impulse can be triggered now
============
*/

void player_shot1();
void player_nail1();
void player_light1();
void player_rocket1();

void CBasePlayer::Attack()
{
	if(!CheckNoAmmo())
		return;

	makevectors(this->v_angle);       // calculate forward angle for velocity
	this->show_hostile = time + 1.0f; // wake monsters up

	mpCurrentWeapon->OnAttack();
};

/*
===============================================================================

PLAYER WEAPON USE

===============================================================================
*/

void CBasePlayer::SetCurrentAmmo()
{
	player_run(); // get out of any weapon firing states

	this->inventory.items = this->inventory.items - (this->inventory.items & (IT_SHELLS | IT_NAILS | IT_ROCKETS | IT_CELLS));

	if(this->weapon == IT_AXE)
	{
		this->currentammo = 0;
		this->weaponmodel = /*mpCurrentWeapon->GetViewModel()*/"models/v_axe.mdl";
	}
	else if(this->weapon == IT_SHOTGUN)
	{
		this->currentammo = this->ammo_shells;
		this->weaponmodel = "models/v_shot.mdl";
		this->inventory.items |= IT_SHELLS;
	}
	else if(this->weapon == IT_SUPER_SHOTGUN)
	{
		this->currentammo = this->ammo_shells;
		this->weaponmodel = "models/v_shot2.mdl";
		this->inventory.items |= IT_SHELLS;
	}
	else if(this->weapon == IT_NAILGUN)
	{
		this->currentammo = this->ammo_nails;
		this->weaponmodel = "models/v_nail.mdl";
		this->inventory.items |= IT_NAILS;
	}
	else if(this->weapon == IT_SUPER_NAILGUN)
	{
		this->currentammo = this->ammo_nails;
		this->weaponmodel = "models/v_nail2.mdl";
		this->inventory.items |= IT_NAILS;
	}
	else if(this->weapon == IT_GRENADE_LAUNCHER)
	{
		this->currentammo = this->ammo_rockets;
		this->weaponmodel = "models/v_rock.mdl";
		this->inventory.items |= IT_ROCKETS;
	}
	else if(this->weapon == IT_ROCKET_LAUNCHER)
	{
		this->currentammo = this->ammo_rockets;
		this->weaponmodel = "models/v_rock2.mdl";
		this->inventory.items |= IT_ROCKETS;
	}
	else if(this->weapon == IT_LIGHTNING)
	{
		this->currentammo = this->ammo_cells;
		this->weaponmodel = "models/v_light.mdl";
		this->inventory.items |= IT_CELLS;
	}
	else
	{
		this->currentammo = 0;
		this->weaponmodel = "";
	};
	
	this->weaponframe = 0;
};

/*
============
W_ChangeWeapon

============
*/
void CBasePlayer::ChangeWeapon()
{
	float it, am, fl;

	it = this->inventory.items;
	am = 0;

	if(this->impulse == 1)
	{
		fl = IT_AXE;
	}
	else if(this->impulse == 2)
	{
		fl = IT_SHOTGUN;
		if(this->ammo_shells < 1)
			am = 1;
	}
	else if(this->impulse == 3)
	{
		fl = IT_SUPER_SHOTGUN;
		if(this->ammo_shells < 2)
			am = 1;
	}
	else if(this->impulse == 4)
	{
		fl = IT_NAILGUN;
		if(this->ammo_nails < 1)
			am = 1;
	}
	else if(this->impulse == 5)
	{
		fl = IT_SUPER_NAILGUN;
		if(this->ammo_nails < 2)
			am = 1;
	}
	else if(this->impulse == 6)
	{
		fl = IT_GRENADE_LAUNCHER;
		if(this->ammo_rockets < 1)
			am = 1;
	}
	else if(this->impulse == 7)
	{
		fl = IT_ROCKET_LAUNCHER;
		if(this->ammo_rockets < 1)
			am = 1;
	}
	else if(this->impulse == 8)
	{
		fl = IT_LIGHTNING;
		if(this->ammo_cells < 1)
			am = 1;
	};

	mnImpulse = 0;

	if(!(this->inventory.items & fl))
	{
		// don't have the weapon or the ammo
		sprint(self, PRINT_HIGH, "no weapon.\n");
		return;
	};

	if(am)
	{
		// don't have the ammo
		sprint(self, PRINT_HIGH, "not enough ammo.\n");
		return;
	};

	//
	// set weapon, set ammo
	//
	this->weapon = fl;
	SetCurrentAmmo();
};

/*
============
CheatCommand
============
*/
void CBasePlayer::CheatCommand()
{
	//if (deathmatch || coop)
		//return;

	this->ammo_rockets = 100;
	this->ammo_nails = 200;
	this->ammo_shells = 100;
	this->inventory.items |=
	IT_AXE |
	IT_SHOTGUN |
	IT_SUPER_SHOTGUN |
	IT_NAILGUN |
	IT_SUPER_NAILGUN |
	IT_GRENADE_LAUNCHER |
	IT_ROCKET_LAUNCHER |
	IT_KEY1 | IT_KEY2;

	this->ammo_cells = 200;
	this->inventory.items |= IT_LIGHTNING;

	this->weapon = IT_ROCKET_LAUNCHER;
	mnImpulse = 0;
	
	SetCurrentAmmo();
};

/*
============
CycleWeaponCommand

Go to the next weapon with ammo
============
*/
void CBasePlayer::CycleWeaponCommand()
{
	float it, am;

	it = this->inventory.items;
	mnImpulse = 0;

	while(1)
	{
		am = 0;

		if(this->weapon == IT_LIGHTNING)
			this->weapon = IT_AXE;
		else if(this->weapon == IT_AXE)
		{
			this->weapon = IT_SHOTGUN;
			if(this->ammo_shells < 1)
				am = 1;
		}
		else if(this->weapon == IT_SHOTGUN)
		{
			this->weapon = IT_SUPER_SHOTGUN;
			if(this->ammo_shells < 2)
				am = 1;
		}
		else if(this->weapon == IT_SUPER_SHOTGUN)
		{
			this->weapon = IT_NAILGUN;
			if(this->ammo_nails < 1)
				am = 1;
		}
		else if(this->weapon == IT_NAILGUN)
		{
			this->weapon = IT_SUPER_NAILGUN;
			if(this->ammo_nails < 2)
				am = 1;
		}
		else if(this->weapon == IT_SUPER_NAILGUN)
		{
			this->weapon = IT_GRENADE_LAUNCHER;
			if(this->ammo_rockets < 1)
				am = 1;
		}
		else if(this->weapon == IT_GRENADE_LAUNCHER)
		{
			this->weapon = IT_ROCKET_LAUNCHER;
			if(this->ammo_rockets < 1)
				am = 1;
		}
		else if(this->weapon == IT_ROCKET_LAUNCHER)
		{
			this->weapon = IT_LIGHTNING;
			if(this->ammo_cells < 1)
				am = 1;
		};

		if((this->inventory.items & this->weapon) && am == 0)
		{
			SetCurrentAmmo();
			return;
		};
	};
};

/*
============
CycleWeaponReverseCommand

Go to the prev weapon with ammo
============
*/
void CBasePlayer::CycleWeaponReverseCommand()
{
	float it, am;

	it = this->inventory.items;
	mnImpulse = 0;

	while(1)
	{
		am = 0;

		if(self.weapon == IT_LIGHTNING)
		{
			self.weapon = IT_ROCKET_LAUNCHER;
			if(self.ammo_rockets < 1)
				am = 1;
		}
		else if(self.weapon == IT_ROCKET_LAUNCHER)
		{
			self.weapon = IT_GRENADE_LAUNCHER;
			if(self.ammo_rockets < 1)
				am = 1;
		}
		else if(self.weapon == IT_GRENADE_LAUNCHER)
		{
			self.weapon = IT_SUPER_NAILGUN;
			if(self.ammo_nails < 2)
				am = 1;
		}
		else if(self.weapon == IT_SUPER_NAILGUN)
		{
			self.weapon = IT_NAILGUN;
			if(self.ammo_nails < 1)
				am = 1;
		}
		else if(self.weapon == IT_NAILGUN)
		{
			self.weapon = IT_SUPER_SHOTGUN;
			if(self.ammo_shells < 2)
				am = 1;
		}
		else if(self.weapon == IT_SUPER_SHOTGUN)
		{
			self.weapon = IT_SHOTGUN;
			if(self.ammo_shells < 1)
				am = 1;
		}
		else if(self.weapon == IT_SHOTGUN)
		{
			self.weapon = IT_AXE;
		}
		else if(self.weapon == IT_AXE)
		{
			self.weapon = IT_LIGHTNING;
			if(self.ammo_cells < 1)
				am = 1;
		};

		if((it & self.weapon) && am == 0)
		{
			SetCurrentAmmo();
			return;
		};
	};
};

/*
============
ServerflagsCommand

Just for development
============
*/
void CBasePlayer::ServerflagsCommand()
{
	serverflags = serverflags * 2 + 1;
};

/*
============
ImpulseCommands

============
*/
void CBasePlayer::ImpulseCommands()
{
	switch(mnImpulse)
	{
	case 1 ... 8:
		ChangeWeapon();
		break;
	case 9:
		CheatCommand();
		break;
	case 10:
		CycleWeaponCommand();
		break;
	case 11:
		ServerflagsCommand();
		break;
	case 12:
		CycleWeaponReverseCommand();
		break;
	};

	mnImpulse = 0;
};

/*
============
W_WeaponFrame

Called every frame so impulse events can be handled as well as possible
============
*/
void CBasePlayer::WeaponFrame()
{
	if(time < this->attack_finished)
		return;

	ImpulseCommands();

	// check for attack
	if(this->button0)
	{
		SuperDamageSound();
		Attack();
	};
};

/*
========
SuperDamageSound

Plays sound if needed
========
*/
void CBasePlayer::SuperDamageSound()
{
	if(this->super_damage_finished > time)
	{
		if(this->super_sound < time)
		{
			this->super_sound = time + 1;
			EmitSound(CHAN_BODY, "items/damage3.wav", 1, ATTN_NORM);
		};
	};
};


/*
===============
PlayerNoise

Each player can have two noise objects associated with it:
a personal noise (jumping, pain, weapon firing), and a weapon
target noise (bullet wall impacts)

Monsters that don't directly see the player can move
to a noise in hopes of seeing the player from there.
===============
*/
void CBasePlayer::Noise(vec3_t where, int type)
{
	edict_t *noise;

	if (type == PNOISE_WEAPON)
	{
		if (this->silencer_shots)
		{
			this->silencer_shots--;
			return;
		};
	};

	if (deathmatch->value)
		return;

	if (this->flags & FL_NOTARGET)
		return;

	if (!this->mynoise)
	{
		noise = gpEngine->pfnSpawn();
		noise->SetClassName("player_noise");
		
		VectorSet (noise->mins, -8, -8, -8);
		VectorSet (noise->maxs, 8, 8, 8);
		
		noise->SetOwner(this);
		noise->svflags = SVF_NOCLIENT;
		this->mynoise = noise;

		noise = gpEngine->pfnSpawn();
		noise->SetClassName("player_noise");
		
		VectorSet (noise->mins, -8, -8, -8);
		VectorSet (noise->maxs, 8, 8, 8);
		
		noise->SetOwner(this);
		noise->svflags = SVF_NOCLIENT;
		this->mynoise2 = noise;
	};

	if (type == PNOISE_SELF || type == PNOISE_WEAPON)
	{
		noise = this->mynoise;
		level.sound_entity = noise;
		level.sound_entity_framenum = level.framenum;
	}
	else // type == PNOISE_IMPACT
	{
		noise = this->mynoise2;
		level.sound2_entity = noise;
		level.sound2_entity_framenum = level.framenum;
	};

	VectorCopy (where, noise->s.origin);
	VectorSubtract (where, noise->maxs, noise->absmin);
	VectorAdd (where, noise->maxs, noise->absmax);
	
	noise->teleport_time = level.time;
	gi.linkentity (noise);
};

/*
================
CheckPowerups

Check for turning off powerups
================
*/
void CBasePlayer::CheckPowerups()
{
	if(self->GetHealth() <= 0)
		return;

	// invisibility
	if(self->v.invisible_finished)
	{
		// sound and screen flash when items starts to run out
		if(self->v.invisible_sound < time)
		{
			self->EmitSound(CHAN_AUTO, "items/inv3.wav", 0.5, ATTN_IDLE);
			self->v.invisible_sound = time + ((random() * 3) + 1);
		};

		if(self->v.invisible_finished < time + 3)
		{
			if(self->v.invisible_time == 1)
			{
				sprint(self, PRINT_HIGH, "Ring of Shadows magic is fading\n");
				stuffcmd(self, "bf\n");
				self->EmitSound(CHAN_AUTO, "items/inv2.wav", 1, ATTN_NORM);
				self->v.invisible_time = time + 1;
			};

			if(self->v.invisible_time < time)
			{
				self->v.invisible_time = time + 1;
				stuffcmd(self, "bf\n");
			};
		};

		if(self->v.invisible_finished < time)
		{
			// just stopped
			self->inventory.items = self->inventory.items - IT_INVISIBILITY;
			self->v.invisible_finished = 0;
			self->v.invisible_time = 0;
		};

		// use the eyes
		self->v.frame = 0;
		self->v.modelindex = modelindex_eyes;
	}
	else
		self->v.modelindex = modelindex_player; // don't use eyes

	// invincibility
	if(self->v.invincible_finished)
	{
		// sound and screen flash when items starts to run out
		if(self->v.invincible_finished < time + 3)
		{
			if(self->v.invincible_time == 1)
			{
				sprint(self, PRINT_HIGH, "Protection is almost burned out\n");
				stuffcmd(self, "bf\n");
				self->EmitSound(CHAN_AUTO, "items/protect2.wav", 1, ATTN_NORM);
				self->v.invincible_time = time + 1;
			};

			if(self->v.invincible_time < time)
			{
				self->v.invincible_time = time + 1;
				stuffcmd(self, "bf\n");
			};
		};

		if(self->v.invincible_finished < time)
		{ // just stopped
			self->v.items = self->v.items - IT_INVULNERABILITY;
			self->v.invincible_time = 0;
			self->v.invincible_finished = 0;
		};
		
		if(self->v.invincible_finished > time)
		{
			self->v.effects |= EF_DIMLIGHT;
			self->v.effects |= EF_RED;
		}
		else
		{
			self->v.effects = self->v.effects - (self->v.effects & EF_DIMLIGHT);
			self->v.effects = self->v.effects - (self->v.effects & EF_RED);
		};
	};

	// super damage
	if(self->v.super_damage_finished)
	{
		// sound and screen flash when items starts to run out

		if(self->v.super_damage_finished < time + 3)
		{
			if(self->v.super_time == 1)
			{
				if(deathmatch == 4)
					sprint(self, PRINT_HIGH, "OctaPower is wearing off\n");
				else
					sprint(self, PRINT_HIGH, "Quad Damage is wearing off\n");
				stuffcmd(self, "bf\n");
				self->EmitSound(CHAN_AUTO, "items/damage2.wav", 1, ATTN_NORM);
				self->v.super_time = time + 1;
			}

			if(self->v.super_time < time)
			{
				self->v.super_time = time + 1;
				stuffcmd(self, "bf\n");
			}
		}

		if(self->v.super_damage_finished < time)
		{ // just stopped
			self->v.items = self->v.items - IT_QUAD;
			if(deathmatch == 4)
			{
				self->v.ammo_cells = 255;
				self->v.armorvalue = 1;
				self->v.armortype = 0.8;
				self->v.health = 100;
			};

			self->v.super_damage_finished = 0;
			self->v.super_time = 0;
		}
		if(self->v.super_damage_finished > time)
		{
			self->v.effects |= EF_DIMLIGHT;
			self->v.effects |= EF_BLUE;
		}
		else
		{
			self->v.effects = self->v.effects - (self->v.effects & EF_DIMLIGHT);
			self->v.effects = self->v.effects - (self->v.effects & EF_BLUE);
		};
	};

	// suit
	if(self->v.radsuit_finished)
	{
		self->v.air_finished = time + 12; // don't drown

		// sound and screen flash when items starts to run out
		if(self->v.radsuit_finished < time + 3)
		{
			if(self->v.rad_time == 1)
			{
				sprint(self, PRINT_HIGH, "Air supply in Biosuit expiring\n");
				stuffcmd(self, "bf\n");
				self->EmitSound(CHAN_AUTO, "items/suit2.wav", 1, ATTN_NORM);
				self->v.rad_time = time + 1;
			}

			if(self->v.rad_time < time)
			{
				self->v.rad_time = time + 1;
				stuffcmd(self, "bf\n");
			};
		};

		if(self->v.radsuit_finished < time)
		{
			// just stopped
			self->v.items = self->v.items - IT_SUIT;
			self->v.rad_time = 0;
			self->v.radsuit_finished = 0;
		};
	};
};