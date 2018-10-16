/// @file

#include "BasePlayer.hpp"

/*
================
idPlayer::ClientThink
================
*/
void CBasePlayer::ClientThink( const int curTime, const float fraction, const bool predict )
{
	if ( IsLocallyControlled() )
		aimAssist.Update();

	UpdateSkinSetup();

	if ( !IsLocallyControlled() )
	{
		// ignore attack button of other clients. that's no good for predictions
		usercmd.buttons &= ~BUTTON_ATTACK;
	}

	buttonMask &= usercmd.buttons;
	usercmd.buttons &= ~buttonMask;

	buttonMask &= usercmd.buttons;
	usercmd.buttons &= ~buttonMask;

	if ( mountedObject ) {
		usercmd.forwardmove = 0;
		usercmd.rightmove = 0;
		usercmd.buttons &= ~(BUTTON_JUMP|BUTTON_CROUCH);
	}

	if ( objectiveSystemOpen ) {
		usercmd.forwardmove = 0;
		usercmd.rightmove = 0;
		usercmd.buttons &= ~(BUTTON_JUMP|BUTTON_CROUCH);
	}

	if ( IsLocallyControlled() ) {
		// zooming
		if ( ( usercmd.buttons ^ oldCmd.buttons ) & BUTTON_ZOOM ) {
			if ( ( usercmd.buttons & BUTTON_ZOOM ) && weapon.GetEntity() ) {
				zoomFov.Init( gameLocal.time, 200.0f, CalcFov( false ), weapon.GetEntity()->GetZoomFov() );
			} else {
				zoomFov.Init( gameLocal.time, 200.0f, zoomFov.GetCurrentValue( gameLocal.time ), DefaultFov() );
			}
		}
	}

	// clear the ik before we do anything else so the skeleton doesn't get updated twice
	walkIK.ClearJointMods();

	if ( gameLocal.isNewFrame ) {
		if ( usercmd.impulseSequence != oldImpulseSequence ) {
			PerformImpulse( usercmd.impulse );
		}
	}

	if ( forceScoreBoard ) {
		gameLocal.mpGame.SetScoreboardActive( true );
	}

	AdjustSpeed();

	if ( IsLocallyControlled() ) {
		UpdateViewAngles();
	} else {
		idQuat interpolatedAngles = Slerp( previousViewQuat, nextViewQuat, fraction );
		viewAngles = interpolatedAngles.ToAngles();
	}

	smoothedOriginUpdated = false;

	if ( !af.IsActive() ) {
		AdjustBodyAngles();
	}

	if ( !isLagged ) {
		// don't allow client to move when lagged
		if ( IsLocallyControlled() ) {
			// Locally-controlled clients are authoritative on their positions, so they can move normally.			
			Move();
			usercmd.pos = physicsObj.GetOrigin();
		} else {
			// Non-locally controlled players are interpolated.
			Move_Interpolated(fraction);
		}
	}

	if ( !g_stopTime.GetBool() ) {
		if ( !noclip && !spectating && ( health > 0 ) && !IsHidden() ) {
			TouchTriggers();
		}
	}

	// update GUIs, Items, and character interactions
	UpdateFocus();

	// service animations
	if ( !spectating && !af.IsActive() ) {
		UpdateConditions();
		UpdateAnimState();
		CheckBlink();
	}

	// clear out our pain flag so we can tell if we recieve any damage between now and the next time we think
	AI_PAIN = false;

	UpdateLocation();

	// calculate the exact bobbed view position, which is used to
	// position the view weapon, among other things
	CalculateFirstPersonView();

	// this may use firstPersonView, or a thirdPerson / camera view
	CalculateRenderView();

	if ( !gameLocal.inCinematic && weapon.GetEntity() && ( health > 0 ) && !( common->IsMultiplayer() && spectating ) ) {
		UpdateWeapon();
	}

	UpdateFlashlight();

	UpdateHud();

	if ( gameLocal.isNewFrame ) {
		UpdatePowerUps();
	}

	UpdateDeathSkin( false );

	renderEntity_t * headRenderEnt = NULL;
	if ( head.GetEntity() ) {
		headRenderEnt = head.GetEntity()->GetRenderEntity();
	}

	if ( headRenderEnt ) {
		if ( influenceSkin ) {
			headRenderEnt->customSkin = influenceSkin;
		} else {
			headRenderEnt->customSkin = NULL;
		}
	}

	if ( common->IsMultiplayer() || g_showPlayerShadow.GetBool() ) {
		renderEntity.suppressShadowInViewID	= 0;
		if ( headRenderEnt ) {
			headRenderEnt->suppressShadowInViewID = 0;
		}
	} else {
		renderEntity.suppressShadowInViewID	= entityNumber+1;
		if ( headRenderEnt ) {
			headRenderEnt->suppressShadowInViewID = entityNumber+1;
		}
	}
	// never cast shadows from our first-person muzzle flashes
	renderEntity.suppressShadowInLightID = LIGHTID_VIEW_MUZZLE_FLASH + entityNumber;
	if ( headRenderEnt ) {
		headRenderEnt->suppressShadowInLightID = LIGHTID_VIEW_MUZZLE_FLASH + entityNumber;
	}

	if ( !gameLocal.inCinematic ) {
		UpdateAnimation();
	}

	if ( enviroSuitLight.IsValid() ) {
		idAngles lightAng = firstPersonViewAxis.ToAngles();
		idVec3 lightOrg = firstPersonViewOrigin;
		const idDict *lightDef = gameLocal.FindEntityDefDict( "envirosuit_light", false );

		idVec3 enviroOffset = lightDef->GetVector( "enviro_offset" );
		idVec3 enviroAngleOffset = lightDef->GetVector( "enviro_angle_offset" );

		lightOrg += (enviroOffset.x * firstPersonViewAxis[0]);
		lightOrg += (enviroOffset.y * firstPersonViewAxis[1]);
		lightOrg += (enviroOffset.z * firstPersonViewAxis[2]);
		lightAng.pitch += enviroAngleOffset.x;
		lightAng.yaw += enviroAngleOffset.y;
		lightAng.roll += enviroAngleOffset.z;

		enviroSuitLight.GetEntity()->GetPhysics()->SetOrigin( lightOrg );
		enviroSuitLight.GetEntity()->GetPhysics()->SetAxis( lightAng.ToMat3() );
		enviroSuitLight.GetEntity()->UpdateVisuals();
		enviroSuitLight.GetEntity()->Present();
	}

	if ( common->IsMultiplayer() ) {
		DrawPlayerIcons();
	}

	Present();

	UpdateDamageEffects();

	LinkCombat();

	// stereo rendering laser sight that replaces the crosshair
	UpdateLaserSight();

	if ( gameLocal.isNewFrame && IsLocallyControlled() ) {
		playerView.CalculateShake();
	}

	// determine if portal sky is in pvs
	pvsHandle_t	clientPVS = gameLocal.pvs.SetupCurrentPVS( GetPVSAreas(), GetNumPVSAreas() );
	gameLocal.portalSkyActive = gameLocal.pvs.CheckAreasForPortalSky( clientPVS, GetPhysics()->GetOrigin() );
	gameLocal.pvs.FreeCurrentPVS( clientPVS );

	//InterpolatePhysics( fraction );

	// Make sure voice groups are set to the right team
	if ( common->IsMultiplayer() && session->GetState() >= idSession::INGAME && entityNumber < MAX_CLIENTS ) {		// The entityNumber < MAX_CLIENTS seems to quiet the static analyzer
		// Make sure we're on the right team (at the lobby level)
		const int voiceTeam = spectating ? LOBBY_SPECTATE_TEAM_FOR_VOICE_CHAT : team;

		//idLib::Printf( "CLIENT: Sending voice %i / %i\n", entityNumber, voiceTeam );

		// Update lobby team
		session->GetActingGameStateLobbyBase().SetLobbyUserTeam( gameLocal.lobbyUserIDs[ entityNumber ], voiceTeam );
		
		// Update voice groups to match in case something changed
		session->SetVoiceGroupsToTeams();
	}
}

/*
==============
CBasePlayer::UpdateSkinSetup
==============
*/
void CBasePlayer::UpdateSkinSetup()
{
	if(!common->IsMultiplayer())
	{
		return;
	}

	if(gameLocal.mpGame.IsGametypeTeamBased())
	{ /* CTF */
		skinIndex = team + 1;
	}
	else
	{
		// Each player will now have their Skin Index Reflect their entity number  ( host = 0, client 1 = 1, client 2 = 2 etc )
		skinIndex = entityNumber; // session->GetActingGameStateLobbyBase().GetLobbyUserSkinIndex( gameLocal.lobbyUserIDs[entityNumber] );
	}
	const char *baseSkinName = gameLocal.mpGame.GetSkinName(skinIndex);
	skin = declManager->FindSkin(baseSkinName, false);
	if(PowerUpActive(BERSERK))
	{
		idStr powerSkinName = baseSkinName;
		powerSkinName.Append("_berserk");
		powerUpSkin = declManager->FindSkin(powerSkinName);
	}
	else if(PowerUpActive(INVULNERABILITY))
	{
		idStr powerSkinName = baseSkinName;
		powerSkinName.Append("_invuln");
		powerUpSkin = declManager->FindSkin(powerSkinName);
	}
	else if(PowerUpActive(INVISIBILITY))
	{
		const char *invisibleSkin = "";
		spawnArgs.GetString("skin_invisibility", "", &invisibleSkin);
		powerUpSkin = declManager->FindSkin(invisibleSkin);
	}
}

/*
===============
CBasePlayer::UpdateMpMessages
===============
*/
void CBasePlayer::AddChatMessage(int index, int alpha, const idStr &message)
{
	if(mpMessages == NULL || !mpMessages->IsActive())
	{
		return;
	}

	idSWFScriptObject *mpChat = mpMessages->GetRootObject().GetNestedObj("_left", "mpChat");

	idSWFSpriteInstance *info = mpChat->GetNestedSprite(va("info%i", index));
	idSWFTextInstance *txtVal = mpChat->GetNestedText(va("info%i", index), "txtVal");
	if(info)
	{
		info->SetVisible(true);
		if(alpha >= 4)
		{
			info->SetAlpha(1.0f);
		}
		else if(alpha == 3)
		{
			info->SetAlpha(0.875f);
		}
		else if(alpha == 2)
		{
			info->SetAlpha(0.75f);
		}
		else if(alpha == 1)
		{
			info->SetAlpha(0.625f);
		}
		else
		{
			info->SetAlpha(0.5f);
		}
	}

	if(txtVal)
	{
		txtVal->SetIgnoreColor(false);
		txtVal->SetText(message);
		txtVal->SetStrokeInfo(true, 0.9f, 1.75f);
	}
}

/*
===============
CBasePlayer::UpdateMpMessages
===============
*/
void CBasePlayer::ClearChatMessage(int index)
{
	if(mpMessages == NULL || !mpMessages->IsActive())
	{
		return;
	}

	idSWFScriptObject *mpChat = mpMessages->GetRootObject().GetNestedObj("_left", "mpChat");

	idSWFSpriteInstance *info = mpChat->GetNestedSprite(va("info%i", index));
	idSWFTextInstance *txtVal = mpChat->GetNestedText(va("info%i", index), "txtVal");
	if(info)
	{
		info->SetVisible(false);
	}

	if(txtVal)
	{
		txtVal->SetText("");
	}
}

/*
===============
CBasePlayer::DrawHUD
===============
*/
void CBasePlayer::DrawHUD(idMenuHandler_HUD *_hudManager)
{
	SCOPED_PROFILE_EVENT("CBasePlayer::DrawHUD");

	if(!weapon.GetEntity() || influenceActive != INFLUENCE_NONE || privateCameraView || gameLocal.GetCamera() || !g_showHud.GetBool())
	{
		return;
	}

	if(common->IsMultiplayer())
	{
		UpdateChattingHud();
		UpdateSpectatingText();
	}

	// Always draw the local client's messages so that chat works correctly while spectating another player.
	CBasePlayer *localPlayer = static_cast<CBasePlayer *>(gameLocal.entities[gameLocal.GetLocalClientNum()]);

	if(localPlayer != NULL && localPlayer->mpMessages != NULL)
	{
		localPlayer->mpMessages->Render(renderSystem, Sys_Milliseconds());
	}

	UpdateHudStats(_hudManager);

	if(spectating)
	{
		return;
	}

	if(_hudManager)
	{
		_hudManager->Update();
	}

	weapon.GetEntity()->UpdateGUI();

	// weapon targeting crosshair
	if(!GuiActive())
	{
		// don't show the 2D crosshair in stereo rendering, use the
		// laser sight model instead
		if(_hudManager && _hudManager->GetHud())
		{
			idMenuScreen_HUD *hud = _hudManager->GetHud();

			if(weapon.GetEntity()->ShowCrosshair() && !IsGameStereoRendered())
			{
				if(weapon.GetEntity()->GetGrabberState() == 1 || weapon.GetEntity()->GetGrabberState() == 2)
				{
					hud->SetCursorState(this, CURSOR_GRABBER, 1);
					hud->SetCursorState(this, CURSOR_IN_COMBAT, 0);
				}
				else
				{
					hud->SetCursorState(this, CURSOR_GRABBER, 0);
					hud->SetCursorState(this, CURSOR_IN_COMBAT, 1);
				}
			}
			else
			{
				hud->SetCursorState(this, CURSOR_NONE, 1);
			}

			hud->UpdateCursorState();
		}
	}
	else if(_hudManager && _hudManager->GetHud())
	{
		idMenuScreen_HUD *hud = _hudManager->GetHud();

		hud->SetCursorState(this, CURSOR_NONE, 1);
		hud->UpdateCursorState();
	}
}

/*
===============
CBasePlayer::UpdateHudWeapon
===============
*/
void CBasePlayer::UpdateHudWeapon(bool flashWeapon)
{
	idMenuScreen_HUD *curDisplay = hud;
	CBasePlayer *p = this;
	if(gameLocal.GetLocalClientNum() >= 0 && gameLocal.entities[gameLocal.GetLocalClientNum()] && gameLocal.entities[gameLocal.GetLocalClientNum()]->IsType(CBasePlayer::Type))
	{
		p = static_cast<CBasePlayer *>(gameLocal.entities[gameLocal.GetLocalClientNum()]);
		if(p->spectating && p->spectator == entityNumber)
		{
			assert(p->hud);
			curDisplay = p->hud;
		}
	}

	if(!curDisplay)
	{
		return;
	}

	curDisplay->UpdateWeaponStates(p, flashWeapon);
}

/*
===============
CBasePlayer::UpdateChattingHud
===============
*/
void CBasePlayer::UpdateChattingHud()
{
	idMenuScreen_HUD *curDisplay = hud;
	CBasePlayer *p = this;
	if(gameLocal.GetLocalClientNum() >= 0 && gameLocal.entities[gameLocal.GetLocalClientNum()] && gameLocal.entities[gameLocal.GetLocalClientNum()]->IsType(CBasePlayer::Type))
	{
		p = static_cast<CBasePlayer *>(gameLocal.entities[gameLocal.GetLocalClientNum()]);
		if(p->spectating && p->spectator == entityNumber)
		{
			assert(p->hud);
			curDisplay = p->hud;
		}
	}

	if(!curDisplay)
	{
		return;
	}

	curDisplay->UpdateChattingHud(p);
}

/*
========================
idMenuScreen_Scoreboard::UpdateSpectating
========================
*/
void CBasePlayer::UpdateSpectatingText()
{
	idSWF *spectatorMessages = mpMessages;
	CBasePlayer *p = this;
	if(gameLocal.GetLocalClientNum() >= 0 && gameLocal.entities[gameLocal.GetLocalClientNum()] && gameLocal.entities[gameLocal.GetLocalClientNum()]->IsType(CBasePlayer::Type))
	{
		p = static_cast<CBasePlayer *>(gameLocal.entities[gameLocal.GetLocalClientNum()]);
		if(p && p->spectating)
		{
			spectatorMessages = p->mpMessages;
		}
	}

	if(!spectatorMessages || !spectatorMessages->IsActive())
	{
		return;
	}

	CBasePlayer *viewPlayer = static_cast<CBasePlayer *>(gameLocal.entities[p->spectator]);
	if(viewPlayer == NULL)
	{
		return;
	}

	idStr spectatetext[2];
	if(!gameLocal.mpGame.IsScoreboardActive())
	{
		gameLocal.mpGame.GetSpectateText(p, spectatetext, false);
	}

	idSWFScriptObject &root = spectatorMessages->GetRootObject();
	idSWFTextInstance *txtVal = root.GetNestedText("txtSpectating");
	if(txtVal != NULL)
	{
		txtVal->tooltip = true;
		txtVal->SetText(spectatetext[0]);
		txtVal->SetStrokeInfo(true, 0.75f, 1.75f);
	}

	txtVal = root.GetNestedText("txtFollow");
	if(txtVal != NULL)
	{
		txtVal->SetText(spectatetext[1]);
		txtVal->SetStrokeInfo(true, 0.75f, 1.75f);
	}
}

/*
===============
CBasePlayer::UpdateHudStats
===============
*/
void CBasePlayer::UpdateHudStats(idMenuHandler_HUD *_hudManager)
{
	if(_hudManager && _hudManager->GetHud())
	{
		idMenuScreen_HUD *hud = _hudManager->GetHud();
		hud->UpdateHealthArmor(this);
		hud->UpdateStamina(this);
		hud->UpdateWeaponInfo(this);

		if(inventory.weaponPulse)
		{
			UpdateHudWeapon();
			inventory.weaponPulse = false;
		}

		if(gameLocal.mpGame.IsGametypeFlagBased())
		{
			hud->SetFlagState(0, gameLocal.mpGame.GetFlagStatus(0));
			hud->SetFlagState(1, gameLocal.mpGame.GetFlagStatus(1));

			hud->SetTeamScore(0, gameLocal.mpGame.GetFlagPoints(0));
			hud->SetTeamScore(1, gameLocal.mpGame.GetFlagPoints(1));

			hud->SetTeam(team);
		}
	}
}

/*
================
idPlayer::ClientReceiveEvent
================
*/
bool CBasePlayer::ClientReceiveEvent( int event, int time, const idBitMsg &msg ) {
	switch ( event ) {
		case EVENT_EXIT_TELEPORTER:
			Event_ExitTeleporter();
			return true;
		case EVENT_ABORT_TELEPORTER:
			SetPrivateCameraView( nullptr );
			return true;
		case EVENT_POWERUP: {
			int powerup = msg.ReadShort();
			int powertime = msg.ReadShort();
			if ( powertime > 0 ) {
				GivePowerUp( powerup, powertime, ITEM_GIVE_UPDATE_STATE );
			} else {
				ClearPowerup( powerup );
			}	
			return true;
		}
		case EVENT_PICKUPNAME: {
			char buf[MAX_EVENT_PARAM_SIZE];
			msg.ReadString(buf, MAX_EVENT_PARAM_SIZE);
			inventory.AddPickupName(buf, this); //_D3XP
			return true;
		}
		case EVENT_SPECTATE: {
			bool spectate = ( msg.ReadBits( 1 ) != 0 );
			Spectate( spectate, true );
			return true;
		}
		case EVENT_ADD_DAMAGE_EFFECT: {
			if ( spectating ) {
				// if we're spectating, ignore
				// happens if the event and the spectate change are written on the server during the same frame (fraglimit)
				return true;
			}
			return CBaseActor::ClientReceiveEvent( event, time, msg );
		}
		case EVENT_FORCE_ORIGIN: {

			idVec3 forceOrigin =  ReadFloatArray< idVec3 >( msg );
			idAngles forceAngles;
			forceAngles[0] = msg.ReadFloat();
			forceAngles[1] = msg.ReadFloat();
			forceAngles[2] = msg.ReadFloat();

			Event_ForceOrigin( forceOrigin, forceAngles );
			return true;
								 }
		case EVENT_KNOCKBACK: {
			idVec3 linearVelocity =  ReadFloatArray< idVec3 >( msg );
			int knockbacktime = msg.ReadByte();
			physicsObj.SetLinearVelocity( linearVelocity );
			physicsObj.SetKnockBack( knockbacktime );
			return true;
		}
		default: {
			return CBaseActor::ClientReceiveEvent( event, time, msg );
		}
	}
//	return false;
}

/*
==================
idPlayer::Event_ExitTeleporter
==================
*/
void CBasePlayer::Event_ExitTeleporter()
{
	idEntity	*exitEnt;
	float		pushVel;

	// verify and setup
	exitEnt = teleportEntity.GetEntity();
	if ( !exitEnt ) {
		common->DPrintf( "Event_ExitTeleporter player %d while not being teleported\n", entityNumber );
		return;
	}

	pushVel = exitEnt->spawnArgs.GetFloat( "push", "300" );

	if ( common->IsServer() )
		ServerSendEvent( EVENT_EXIT_TELEPORTER, NULL, false );

	SetPrivateCameraView( nullptr );
	
	// setup origin and push according to the exit target
	SetOrigin( exitEnt->GetPhysics()->GetOrigin() + idVec3( 0, 0, CM_CLIP_EPSILON ) );
	SetViewAngles( exitEnt->GetPhysics()->GetAxis().ToAngles() );
	physicsObj.SetLinearVelocity( exitEnt->GetPhysics()->GetAxis()[ 0 ] * pushVel );
	physicsObj.ClearPushedVelocity();
	// teleport fx
	playerView.Flash( colorWhite, 120 );

	// clear the ik heights so model doesn't appear in the wrong place
	walkIK.EnableAll();

	UpdateVisuals();

	StartSound( "snd_teleport_exit", SND_CHANNEL_ANY, 0, false, NULL );

	if ( teleportKiller != -1 ) {
		// we got killed while being teleported
		Damage( gameLocal.entities[ teleportKiller ], gameLocal.entities[ teleportKiller ], vec3_origin, "damage_telefrag", 1.0f, INVALID_JOINT );
		teleportKiller = -1;
	} else {
		// kill anything that would have waited at teleport exit
		gameLocal.KillBox( this );
	}
	teleportEntity = NULL;
}

/*
====================
idPlayer::SetPrivateCameraView
====================
*/
void CBasePlayer::SetPrivateCameraView( idCamera *camView )
{
	privateCameraView = camView;
	
	if ( camView )
	{
		StopFiring();
		Hide();
	}
	else
		if ( !spectating )
			Show();
};

/*
==================
idPlayer::Event_ExitTeleporter
==================
*/
void CBasePlayer::Event_ForceOrigin( CVec3 & origin, CAngles & angles )
{
	SetOrigin( origin + idVec3( 0, 0, CM_CLIP_EPSILON ) );
	//SetViewAngles( angles );

	UpdateVisuals();
};