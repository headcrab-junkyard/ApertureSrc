/// @file

#include "BaseItem.hpp"

//CBaseItem::CBaseItem() = default;
//CBaseItem::~CBaseItem() = default;

/* 
================
CBaseItem::ClientThink
================
*/
void CBaseItem::ClientThink( int curTime, float fraction, bool predict )
{
	// only think forward because the state is not synced through snapshots
	if ( !gameLocal.isNewFrame )
		return;

	Think();
};

/*
================
CBaseItem::ClientPredictionThink
================
*/
void CBaseItem::ClientPredictionThink()
{
	// only think forward because the state is not synced through snapshots
	if ( !gameLocal.isNewFrame )
		return;

	Think();
};

/*
================
CBaseItem::UpdateRenderEntity
================
*/
bool CBaseItem::UpdateRenderEntity(renderEntity_s *renderEntity, const renderView_t *renderView) const
{
	if(lastRenderViewTime == renderView->time[timeGroup])
		return false;

	lastRenderViewTime = renderView->time[timeGroup];

	// check for glow highlighting if near the center of the view
	idVec3 dir = renderEntity->origin - renderView->vieworg;
	dir.Normalize();
	float d = dir * renderView->viewaxis[0];

	// two second pulse cycle
	float cycle = (renderView->time[timeGroup] - inViewTime) / 2000.0f;

	if(d > 0.94f)
	{
		if(!inView)
		{
			inView = true;
			if(cycle > lastCycle)
			{
				// restart at the beginning
				inViewTime = renderView->time[timeGroup];
				cycle = 0.0f;
			}
		}
	}
	else
	{
		if(inView)
		{
			inView = false;
			lastCycle = ceil(cycle);
		}
	}

	// fade down after the last pulse finishes
	if(!inView && cycle > lastCycle)
	{
		renderEntity->shaderParms[4] = 0.0f;
	}
	else
	{
		// pulse up in 1/4 second
		cycle -= (int)cycle;
		if(cycle < 0.1f)
		{
			renderEntity->shaderParms[4] = cycle * 10.0f;
		}
		else if(cycle < 0.2f)
		{
			renderEntity->shaderParms[4] = 1.0f;
		}
		else if(cycle < 0.3f)
		{
			renderEntity->shaderParms[4] = 1.0f - (cycle - 0.2f) * 10.0f;
		}
		else
		{
			// stay off between pulses
			renderEntity->shaderParms[4] = 0.0f;
		}
	}

	// update every single time this is in view
	return true;
}

/*
================
CBaseItem::ModelCallback
================
*/
bool CBaseItem::ModelCallback(renderEntity_t *renderEntity, const renderView_t *renderView)
{
	const CBaseItem *ent;

	// this may be triggered by a model trace or other non-view related source
	if(!renderView)
		return false;

	ent = static_cast<CBaseItem *>(gameLocal.entities[renderEntity->entityNum]);
	if(ent == nullptr)
	{
		mpSystem->Error("idItem::ModelCallback: callback with NULL game entity");
		return false;
	};

	return ent->UpdateRenderEntity(renderEntity, renderView);
}

/*
================
CBaseItem::Present
================
*/
void CBaseItem::Present()
{
	CBaseEntity::Present();

	if(!fl.hidden && pulse)
	{
		// also add a highlight shell model
		renderEntity_t shell;

		shell = renderEntity;

		// we will mess with shader parms when the item is in view
		// to give the "item pulse" effect
		shell.callback = CBaseItem::ModelCallback;
		shell.entityNum = entityNumber;
		shell.customShader = shellMaterial;
		
		if(itemShellHandle == -1)
			itemShellHandle = gameRenderWorld->AddEntityDef(&shell);
		else
			gameRenderWorld->UpdateEntityDef(itemShellHandle, &shell);
	};
};

/*
================
CBaseItem::ClientReceiveEvent
================
*/
bool CBaseItem::ClientReceiveEvent( int event, int time, const idBitMsg &msg ) {

	switch( event ) {
		case EVENT_RESPAWN: {
			Event_Respawn();
			return true;
		}
		case EVENT_RESPAWNFX: {
			Event_RespawnFx();
			return true;
		}
		default: {
			return CBaseEntity::ClientReceiveEvent( event, time, msg );
		}
	}
}

/*
================
CBaseItem::Event_Respawn
================
*/
void CBaseItem::Event_Respawn()
{
	BecomeActive( TH_THINK );
	Show();
	inViewTime = -1000;
	lastCycle = -1;
	GetPhysics()->SetContents( CONTENTS_TRIGGER );
	SetOrigin( orgOrigin );
	StartSound( "snd_respawn", SND_CHANNEL_ITEM, 0, false, NULL );
	CancelEvents( &EV_RespawnItem ); // don't double respawn
}

/*
================
CBaseItem::Event_RespawnFx
================
*/
void CBaseItem::Event_RespawnFx()
{
	const char *sfx = spawnArgs.GetString( "fxRespawn" );
	if ( sfx != NULL && *sfx != NULL ) {
		idEntityFx::StartFx( sfx, NULL, NULL, this, true );
	}
}