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
/// @brief Items the player can pick up or use

#pragma once

#include "BaseEntity.hpp"

class CBaseItem : public CBaseEntity
{
public:
	CLASS_PROTOTYPE(CBaseItem);

	CBaseItem();
	virtual ~CBaseItem();

	void Save(idSaveGame *savefile) const;
	void Restore(idRestoreGame *savefile);

	void Spawn();
	virtual void Think();

	void GetAttributes(idDict &attributes) const;

	virtual bool GiveToPlayer(CBasePlayer *player, unsigned int giveFlags);
	virtual bool Pickup(CBasePlayer *player);

	virtual void Present();

	// networking
	virtual void WriteToSnapshot(idBitMsg &msg) const;
	virtual void ReadFromSnapshot(const idBitMsg &msg);

	enum
	{
		EVENT_PICKUP = CBaseEntity::EVENT_MAXEVENTS,
		EVENT_RESPAWN,
		EVENT_RESPAWNFX,
		EVENT_TAKEFLAG,
		EVENT_DROPFLAG,
		EVENT_FLAGRETURN,
		EVENT_FLAGCAPTURE,
		EVENT_MAXEVENTS
	};

private:
	idVec3 orgOrigin;
	bool spin;
	bool pulse;
	bool canPickUp;

	// for item pulse effect
	int itemShellHandle;
	const idMaterial *shellMaterial;

	// used to update the item pulse effect
	mutable bool inView;
	mutable int inViewTime;
	mutable int lastCycle;
	mutable int lastRenderViewTime;

	void Event_DropToFloor();
	void Event_Touch(idEntity *other, trace_t *trace);
	void Event_Trigger(idEntity *activator);
	void Event_Respawn();
	void Event_RespawnFx();
};