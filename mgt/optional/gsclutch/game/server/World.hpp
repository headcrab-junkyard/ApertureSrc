/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-2005 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
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
*/

/// @file

#pragma once

/// Needs to be called any time an entity changes origin, mins, maxs, or solid
/// flags ent->v.modified
/// sets ent->v.absmin and ent->v.absmax
/// if touchtriggers, calls prog functions for the intersected triggers
void SV_LinkEdict(edict_t *ent, bool touch_triggers); // TODO: sharedEntity_t in q3

/// call before removing an entity, and before trying to move one,
/// so it doesn't clip against itself
/// flags ent->v.modified
void SV_UnlinkEdict(edict_t *ent); // TODO: sharedEntity_t in q3

edict_t *SV_TestEntityPosition(edict_t *ent);

/// mins and maxs are reletive
///
/// if the entire move stays in a solid volume, trace.allsolid will be set
///
/// if the starting point is in a solid, it will be allowed to move out
/// to an open area
///
/// nomonsters is used for line of sight or edge testing, where monsters
/// shouldn't be considered solid objects
///
/// passedict is explicitly excluded from clipping checks (normally nullptr)
trace_t SV_Move(vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int type, edict_t *passedict);

edict_t *SV_TestPlayerPosition(edict_t *ent, vec3_t origin);