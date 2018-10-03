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

#include "BaseItem.hpp"

CBaseItem::CBaseItem() = default;
CBaseItem::~CBaseItem() = default;

/*
================
idItem::Spawn
================
*/
void CBaseItem::Spawn()
{
	idStr giveTo;
	idEntity *ent;
	float tsize;

	if(spawnArgs.GetBool("dropToFloor"))
	{
		PostEventMS(&EV_DropToFloor, 0);
	}

	if(spawnArgs.GetFloat("triggersize", "0", tsize))
	{
		GetPhysics()->GetClipModel()->LoadModel(idTraceModel(idBounds(vec3_origin).Expand(tsize)));
		GetPhysics()->GetClipModel()->Link(gameLocal.clip);
	}

	if(spawnArgs.GetBool("start_off"))
	{
		GetPhysics()->SetContents(0);
		Hide();
	}
	else
	{
		GetPhysics()->SetContents(CONTENTS_TRIGGER);
	}

	giveTo = spawnArgs.GetString("owner");
	if(giveTo.Length())
	{
		ent = gameLocal.FindEntity(giveTo);
		if(!ent)
		{
			gameLocal.Error("Item couldn't find owner '%s'", giveTo.c_str());
		}
		PostEventMS(&EV_Touch, 0, ent, NULL);
	}

	// idItemTeam does not rotate and bob
	if(spawnArgs.GetBool("spin") || (common->IsMultiplayer() && !this->IsType(idItemTeam::Type)))
	{
		spin = true;
		BecomeActive(TH_THINK);
	}

	//pulse = !spawnArgs.GetBool( "nopulse" );
	//temp hack for tim
	pulse = false;
	orgOrigin = GetPhysics()->GetOrigin();

	canPickUp = !(spawnArgs.GetBool("triggerFirst") || spawnArgs.GetBool("no_touch"));

	inViewTime = -1000;
	lastCycle = -1;
	itemShellHandle = -1;
	shellMaterial = declManager->FindMaterial("itemHighlightShell");
}

/*
================
idItem::Think
================
*/
void CBaseItem::Think()
{
	if(thinkFlags & TH_THINK)
	{
		if(spin)
		{
			idAngles ang;
			idVec3 org;

			ang.pitch = ang.roll = 0.0f;
			ang.yaw = (gameLocal.time & 4095) * 360.0f / -4096.0f;
			SetAngles(ang);

			float scale = 0.005f + entityNumber * 0.00001f;

			org = orgOrigin;
			org.z += 4.0f + cos((gameLocal.time + 2000) * scale) * 4.0f;
			SetOrigin(org);
		}
	}

	Present();
}

/*
================
idItem::GiveToPlayer
================
*/
bool CBaseItem::GiveToPlayer(CBasePlayer *player, unsigned int giveFlags)
{
	if(player == NULL)
	{
		return false;
	}

	if(spawnArgs.GetBool("inv_carry"))
	{
		return player->GiveInventoryItem(&spawnArgs, giveFlags);
	}

	return player->GiveItem(this, giveFlags);
}

/*
================
idItem::Pickup
================
*/
bool CBaseItem::Pickup(CBasePlayer *player)
{
	const bool didGiveSucceed = GiveToPlayer(player, ITEM_GIVE_FEEDBACK);
	if(!didGiveSucceed)
	{
		return false;
	}

	// Store the time so clients know when to stop predicting and let snapshots overwrite.
	if(player->IsLocallyControlled())
	{
		clientPredictPickupMilliseconds = gameLocal.time;
	}
	else
	{
		clientPredictPickupMilliseconds = 0;
	}

	// play pickup sound
	StartSound("snd_acquire", SND_CHANNEL_ITEM, 0, false, NULL);

	// clear our contents so the object isn't picked up twice
	GetPhysics()->SetContents(0);

	// hide the model
	Hide();

	// remove the highlight shell
	if(itemShellHandle != -1)
	{
		gameRenderWorld->FreeEntityDef(itemShellHandle);
		itemShellHandle = -1;
	}

	// Clients need to bail out after some feedback, but
	// before actually changing any values. The values
	// will be updated in the next snapshot.
	if(common->IsClient())
	{
		return didGiveSucceed;
	}

	if(!GiveToPlayer(player, ITEM_GIVE_UPDATE_STATE))
	{
		return false;
	}

	// trigger our targets
	ActivateTargets(player);

	float respawn = spawnArgs.GetFloat("respawn");
	bool dropped = spawnArgs.GetBool("dropped");
	bool no_respawn = spawnArgs.GetBool("no_respawn");

	if(common->IsMultiplayer() && respawn == 0.0f)
	{
		respawn = 20.0f;
	}

	if(respawn && !dropped && !no_respawn)
	{
		const char *sfx = spawnArgs.GetString("fxRespawn");
		if(sfx != NULL && *sfx != NULL)
		{
			PostEventSec(&EV_RespawnFx, respawn - 0.5f);
		}
		PostEventSec(&EV_RespawnItem, respawn);
	}
	else if(!spawnArgs.GetBool("inv_objective") && !no_respawn)
	{
		// give some time for the pickup sound to play
		// FIXME: Play on the owner
		if(!spawnArgs.GetBool("inv_carry"))
		{
			PostEventMS(&EV_Remove, 5000);
		}
	}

	BecomeInactive(TH_THINK);
	return true;
}

/*
================
idItem::Event_Respawn
================
*/
void CBaseItem::Event_Respawn()
{
	ServerSendEvent(EVENT_RESPAWN, NULL, false);
}

/*
================
idItem::Event_RespawnFx
================
*/
void CBaseItem::Event_RespawnFx()
{
	ServerSendEvent(EVENT_RESPAWNFX, NULL, false);
}