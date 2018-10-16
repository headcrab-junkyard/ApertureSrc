/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1993-2012 id Software LLC, a ZeniMax Media company.
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
 *
 * In addition, the Doom 3 BFG Edition Source Code is also subject to certain additional terms. 
 * You should have received a copy of these additional terms immediately 
 * following the terms and conditions of the GNU General Public License 
 * which accompanied the Doom 3 BFG Edition Source Code. If not, please request a copy 
 * in writing from id Software at the address below.
 *
 * If you have questions concerning this license or the applicable 
 * additional terms, you may contact in writing id Software LLC, 
 * c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.
*/

/// @file

#include "BaseItem.hpp"

//CBaseItem::CBaseItem() = default;
//CBaseItem::~CBaseItem() = default;

/*
================
CBaseItem::Save
================
*/
void CBaseItem::Save(idSaveGame *savefile) const
{
	savefile->WriteVec3(orgOrigin);
	savefile->WriteBool(spin);
	savefile->WriteBool(pulse);
	savefile->WriteBool(canPickUp);

	savefile->WriteMaterial(shellMaterial);

	savefile->WriteBool(inView);
	savefile->WriteInt(inViewTime);
	savefile->WriteInt(lastCycle);
	savefile->WriteInt(lastRenderViewTime);
};

/*
================
CBaseItem::Restore
================
*/
void CBaseItem::Restore(idRestoreGame *savefile)
{
	savefile->ReadVec3(orgOrigin);
	savefile->ReadBool(spin);
	savefile->ReadBool(pulse);
	savefile->ReadBool(canPickUp);

	savefile->ReadMaterial(shellMaterial);

	savefile->ReadBool(inView);
	savefile->ReadInt(inViewTime);
	savefile->ReadInt(lastCycle);
	savefile->ReadInt(lastRenderViewTime);

	//itemShellHandle = -1;
};

/*
================
idItem::Event_Respawn
================
*/
void CBaseItem::Event_Respawn()
{
	ServerSendEvent(EVENT_RESPAWN, nullptr, false);
};

/*
================
idItem::Event_RespawnFx
================
*/
void CBaseItem::Event_RespawnFx()
{
	ServerSendEvent(EVENT_RESPAWNFX, nullptr, false);
};