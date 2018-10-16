/// @file
/// @brief base entity client-side methods implementations

/*
================
UpdateGuiParms
================
*/
void UpdateGuiParms(idUserInterface *gui, const idDict *args)
{
	if(gui == NULL || args == NULL)
	{
		return;
	}
	const idKeyValue *kv = args->MatchPrefix("gui_parm", NULL);
	while(kv)
	{
		gui->SetStateString(kv->GetKey(), kv->GetValue());
		kv = args->MatchPrefix("gui_parm", kv);
	}
	gui->SetStateBool("noninteractive", args->GetBool("gui_noninteractive"));
	gui->StateChanged(gameLocal.time);
}

/*
================
AddRenderGui
================
*/
void AddRenderGui(const char *name, idUserInterface **gui, const idDict *args)
{
	const idKeyValue *kv = args->MatchPrefix("gui_parm", NULL);
	*gui = uiManager->FindGui(name, true, (kv != NULL));
	UpdateGuiParms(*gui, args);
}

/*
================
CBaseEntity::ClientThink
================
*/
void CBaseEntity::ClientThink(const int curTime, const float fraction, const bool predict)
{
	InterpolatePhysics(fraction);
	Present();
};

/*
================
CBaseEntity::ClientPredictionThink
================
*/
void CBaseEntity::ClientPredictionThink()
{
	RunPhysics();
	Present();
};

/*
================
CBaseEntity::TriggerGuis
================
*/
void CBaseEntity::TriggerGuis()
{
	for(int i = 0; i < MAX_RENDERENTITY_GUI; ++i)
		if(renderEntity.gui[i])
			renderEntity.gui[i]->Trigger(gameLocal.time);
};

/*
================
CBaseEntity::GetListenerId
================
*/
int CBaseEntity::GetListenerId() const
{
	return refSound.listenerId;
}

/*
================
CBaseEntity::GetSoundEmitter
================
*/
idSoundEmitter *CBaseEntity::GetSoundEmitter() const
{
	return refSound.referenceSound;
}

/*
================
CBaseEntity::FreeSoundEmitter
================
*/
void CBaseEntity::FreeSoundEmitter(bool immediate)
{
	if(refSound.referenceSound)
	{
		refSound.referenceSound->Free(immediate);
		refSound.referenceSound = NULL;
	}
}

/*
================
idEntity::WriteGUIToSnapshot
================
*/
void idEntity::WriteGUIToSnapshot(idBitMsg &msg) const
{
	// no need to loop over MAX_RENDERENTITY_GUI at this time
	if(renderEntity.gui[0])
	{
		msg.WriteByte(renderEntity.gui[0]->State().GetInt("networkState"));
	}
	else
	{
		msg.WriteByte(0);
	}
}

/*
================
idEntity::Event_SetGui
================
* BSM Nerve: Allows guis to be changed at runtime. Guis that are
* loaded after the level loads should be precahced using PrecacheGui.
*/
void idEntity::Event_SetGui(int guiNum, const char *guiName)
{
	idUserInterface **gui = NULL;

	if(guiNum >= 1 && guiNum <= MAX_RENDERENTITY_GUI)
	{
		gui = &renderEntity.gui[guiNum - 1];
	}

	if(gui)
	{
		*gui = uiManager->FindGui(guiName, true, false);
		UpdateGuiParms(*gui, &spawnArgs);
		UpdateChangeableSpawnArgs(NULL);
		gameRenderWorld->UpdateEntityDef(modelDefHandle, &renderEntity);
	}
	else
	{
		gpSystem->Error("Entity '%s' doesn't have a GUI %d", name.c_str(), guiNum);
	}
}

/*
================
idEntity::Event_PrecacheGui
================
* BSM Nerve: Forces the engine to initialize a gui even if it is not specified as used in a level.
* This is useful for preventing load hitches when switching guis during the game using "setGui"
*/
void idEntity::Event_PrecacheGui(const char *guiName)
{
	uiManager->FindGui(guiName, true, true);
}

void idEntity::Event_GetGuiParm(int guiNum, const char *key)
{
	if(renderEntity.gui[guiNum - 1])
	{
		idThread::ReturnString(renderEntity.gui[guiNum - 1]->GetStateString(key));
		return;
	}
	idThread::ReturnString("");
}

void idEntity::Event_GetGuiParmFloat(int guiNum, const char *key)
{
	if(renderEntity.gui[guiNum - 1])
	{
		idThread::ReturnFloat(renderEntity.gui[guiNum - 1]->GetStateFloat(key));
		return;
	}
	idThread::ReturnFloat(0.0f);
}

void idEntity::Event_GuiNamedEvent(int guiNum, const char *event)
{
	if(renderEntity.gui[guiNum - 1])
	{
		renderEntity.gui[guiNum - 1]->HandleNamedEvent(event);
	}
}

/*
================
idEntity::WriteColorToSnapshot
================
*/
void idEntity::WriteColorToSnapshot(idBitMsg &msg) const
{
	idVec4 color;

	color[0] = renderEntity.shaderParms[SHADERPARM_RED];
	color[1] = renderEntity.shaderParms[SHADERPARM_GREEN];
	color[2] = renderEntity.shaderParms[SHADERPARM_BLUE];
	color[3] = renderEntity.shaderParms[SHADERPARM_ALPHA];
	msg.WriteLong(PackColor(color));
}

/*
================
idEntity::ReadColorFromSnapshot
================
*/
void idEntity::ReadColorFromSnapshot(const idBitMsg &msg)
{
	idVec4 color;

	UnpackColor(msg.ReadLong(), color);
	renderEntity.shaderParms[SHADERPARM_RED] = color[0];
	renderEntity.shaderParms[SHADERPARM_GREEN] = color[1];
	renderEntity.shaderParms[SHADERPARM_BLUE] = color[2];
	renderEntity.shaderParms[SHADERPARM_ALPHA] = color[3];
}

/*
================
idEntity::ReadGUIFromSnapshot
================
*/
void idEntity::ReadGUIFromSnapshot(const CBitMsg &msg)
{
	int state;
	idUserInterface *gui;
	state = msg.ReadByte();
	gui = renderEntity.gui[0];
	if(gui && state != mpGUIState)
	{
		mpGUIState = state;
		gui->SetStateInt("networkState", state);
		gui->HandleNamedEvent("networkState");
	}
}

/*
================
CBaseEntity::ClientSendEvent
================
*/
void CBaseEntity::ClientSendEvent(int eventId, const idBitMsg *msg) const
{
	CBitMsg outMsg;
	byte msgBuf[MAX_GAME_MESSAGE_SIZE];

	// prevent dupe events caused by frame re-runs
	if(!gameLocal.isNewFrame)
		return;

	outMsg.InitWrite(msgBuf, sizeof(msgBuf));
	outMsg.BeginWriting();
	outMsg.WriteBits(gameLocal.GetSpawnId(this), 32);
	outMsg.WriteByte(eventId);
	outMsg.WriteLong(gameLocal.serverTime);
	
	if(msg)
	{
		outMsg.WriteBits(msg->GetSize(), idMath::BitsForInteger(MAX_EVENT_PARAM_SIZE));
		outMsg.WriteData(msg->GetReadData(), msg->GetSize());
	}
	else
		outMsg.WriteBits(0, idMath::BitsForInteger(MAX_EVENT_PARAM_SIZE));

	session->GetActingGameStateLobbyBase().SendReliableToHost(GAME_RELIABLE_MESSAGE_EVENT, outMsg);
};

/*
================
CBaseEntity::ClientReceiveEvent
================
*/
bool CBaseEntity::ClientReceiveEvent(int event, int time, const idBitMsg &msg)
{
	int index;
	const idSoundShader *shader;
	s_channelType channel;

	switch(event)
	{
	case EVENT_STARTSOUNDSHADER:
	{
		// the sound stuff would early out
		assert(gameLocal.isNewFrame);
		if(time < gameLocal.realClientTime - 1000)
		{
			// too old, skip it ( reliable messages don't need to be parsed in full )
			gpSystem->DevPrintf("ent 0x%x: start sound shader too old (%d ms)\n", entityNumber, gameLocal.realClientTime - time);
			return true;
		}
		index = gameLocal.ClientRemapDecl(DECL_SOUND, msg.ReadLong());
		if(index >= 0 && index < declManager->GetNumDecls(DECL_SOUND))
		{
			shader = declManager->SoundByIndex(index, false);
			channel = (s_channelType)msg.ReadByte();
			StartSoundShader(shader, channel, 0, false, NULL);
		}
		return true;
	}
	case EVENT_STOPSOUNDSHADER:
	{
		// the sound stuff would early out
		assert(gameLocal.isNewFrame);
		channel = (s_channelType)msg.ReadByte();
		StopSound(channel, false);
		return true;
	}
	default:
	{
		return false;
	}
	}
}