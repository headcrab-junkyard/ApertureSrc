/// @file

#include "BaseWeapon.hpp"

/*
================
CBaseWeapon::MuzzleFlashLight
================
*/
void CBaseWeapon::MuzzleFlashLight()
{
	if(!lightOn && (!g_muzzleFlash.GetBool() || !muzzleFlash.lightRadius[0]))
		return;

	if(flashJointView == INVALID_JOINT)
		return;

	UpdateFlashPosition();

	// these will be different each fire
	muzzleFlash.shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC(gameLocal.time);
	muzzleFlash.shaderParms[SHADERPARM_DIVERSITY] = renderEntity.shaderParms[SHADERPARM_DIVERSITY];

	worldMuzzleFlash.shaderParms[SHADERPARM_TIMEOFFSET] = -MS2SEC(gameLocal.time);
	worldMuzzleFlash.shaderParms[SHADERPARM_DIVERSITY] = renderEntity.shaderParms[SHADERPARM_DIVERSITY];

	// the light will be removed at this time
	muzzleFlashEnd = gameLocal.time + flashTime;

	if(muzzleFlashHandle != -1)
	{
		gameRenderWorld->UpdateLightDef(muzzleFlashHandle, &muzzleFlash);
		gameRenderWorld->UpdateLightDef(worldMuzzleFlashHandle, &worldMuzzleFlash);
	}
	else
	{
		muzzleFlashHandle = gameRenderWorld->AddLightDef(&muzzleFlash);
		worldMuzzleFlashHandle = gameRenderWorld->AddLightDef(&worldMuzzleFlash);
	};
};

/*
================
CBaseWeapon::RemoveMuzzleFlashlight
================
*/
void CBaseWeapon::RemoveMuzzleFlashlight()
{
	if(muzzleFlashHandle != -1)
	{
		gameRenderWorld->FreeLightDef(muzzleFlashHandle);
		muzzleFlashHandle = -1;
	}
	if(worldMuzzleFlashHandle != -1)
	{
		gameRenderWorld->FreeLightDef(worldMuzzleFlashHandle);
		worldMuzzleFlashHandle = -1;
	}
}

/*
================
CBaseWeapon::ClientReceiveEvent
================
*/
bool CBaseWeapon::ClientReceiveEvent( int event, int time, const idBitMsg &msg )
{
	switch( event )
	{
		case EVENT_RELOAD: {
			// Local clients predict reloads, only process this event for remote clients.
			if ( owner != NULL && !owner->IsLocallyControlled() && ( gameLocal.time - time < 1000 ) ) {
				if ( WEAPON_NETRELOAD.IsLinked() ) {
					WEAPON_NETRELOAD = true;
					WEAPON_NETENDRELOAD = false;
				}
			}
			return true;
		}
		case EVENT_ENDRELOAD: {
			// Local clients predict reloads, only process this event for remote clients.
			if ( owner != NULL && !owner->IsLocallyControlled() && WEAPON_NETENDRELOAD.IsLinked() ) {
				WEAPON_NETENDRELOAD = true;
			}
			return true;
		}
		case EVENT_CHANGESKIN: {
			int index = gameLocal.ClientRemapDecl( DECL_SKIN, msg.ReadLong() );
			renderEntity.customSkin = ( index != -1 ) ? static_cast<const idDeclSkin *>( declManager->DeclByIndex( DECL_SKIN, index ) ) : NULL;
			UpdateVisuals();
			if ( worldModel.GetEntity() ) {
				worldModel.GetEntity()->SetSkin( renderEntity.customSkin );
			}
			return true;
		}
		default: {
			return idEntity::ClientReceiveEvent( event, time, msg );
		}
	}
};

/*
===============
CBaseWeapon::ClientThink
===============
*/
void CBaseWeapon::ClientThink( const int curTime, const float fraction, const bool predict )
{
	UpdateAnimation();	
};

/*
===============
CBaseWeapon::ClientPredictionThink
===============
*/
void CBaseWeapon::ClientPredictionThink()
{
	UpdateAnimation();	
};