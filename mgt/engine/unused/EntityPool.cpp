/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2016-2018 Headcrab Garage
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief entity pool

#include "EntityPool.hpp"
//#include "debug/Debug.hpp"

CEntityPool::CEntityPool(int anMaxEntities) // -1 for default or uint
{
	//LogMsg("Constructing the entity pool component...");

	int nEntitiesToReserve = MAX_EDICTS;

	if(anMaxEntities > 0)
		nEntitiesToReserve = anMaxEntities;

	mvEntities.reserve(nEntitiesToReserve);
};

CEntityPool::~CEntityPool()
{
	//LogMsg("Destructing the entity pool component...");
};

/*
edict_t *ED_Alloc (void)
{
	int			i;
	edict_t		*e;

	for ( i=svs.maxclients+1 ; i<sv.num_edicts ; i++)
	//for ( i=MAX_CLIENTS+1 ; i<sv.num_edicts ; i++)
	{
		e = EDICT_NUM(i);
		// the first couple seconds of server time can involve a lot of
		// freeing and allocating, so relax the replacement policy
		if (e->free && ( e->freetime < 2 || sv.time - e->freetime > 0.5 ) )
		{
			ED_ClearEdict (e);
			return e;
		}
	}
	
	if (i == MAX_EDICTS)
		Sys_Error ("ED_Alloc: no free edicts");
		
	sv.num_edicts++;
	
	/*
	if (i == MAX_EDICTS)
	{
		Con_Printf ("WARNING: ED_Alloc: no free edicts\n");
		i--;	// step on whatever is the last edict
		e = EDICT_NUM(i);
		SV_UnlinkEdict(e);
	}
	else
		sv.num_edicts++;
	*/

e = EDICT_NUM(i);
ED_ClearEdict(e);

return e;
}
* /

/*
=================
ED_Alloc

Either finds a free edict, or allocates a new one.
Try to avoid reusing an entity that was recently freed, because it
can cause the client to think the entity morphed into something else
instead of being removed and recreated, which can cause interpolated
angles and bad trails.
=================
*/
IEntity *
CEntityPool::AllocEntity()
{
	for(auto It : mvEntities)
	{
		// The first couple seconds of server time
		// can involve a lot of freeing and allocating,
		// so relax the replacement policy
		if(It->IsFree() && (It->GetFreeTime() < 2.0f || (mfTime - It->GetFreeTime()) > 0.5f))
		{
			It->Init();
			return It;
		};
	};

	Sys_Error("ED_AllocEdict: no free edicts\n");
	return nullptr;
};

void CGameWorld::FreeEdicts()
{
	for(auto &&It : mvEdicts)
	{
		if(It->IsFree())
			continue;

		It->Free();
	};
};

void CEntityPool::FreeAllEntities()
{
	// Mark all entities as free
	for(auto &&It : mvEntities)
		It->SetFree(true);
};