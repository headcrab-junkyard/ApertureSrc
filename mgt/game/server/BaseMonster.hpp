/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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
*/

/// @file

#pragma once

#include "BaseEntity.hpp"

class CBaseMonster : public CBaseEntity
{
public:
	CBaseMonster();
	~CBaseMonster();
private:
	typedef struct
	{
		void	(*aifunc)(edict_t *self, float dist);
		float	dist;
		void	(*thinkfunc)(edict_t *self);
	} mframe_t;

	typedef struct
	{
		int			firstframe;
		int			lastframe;
		mframe_t	*frame;
		void		(*endfunc)(edict_t *self);
	} mmove_t;

	mmove_t		*currentmove{nullptr};
	int			aiflags{0};
	int			nextframe{0};
	float		scale{0.0f};

	void		(*stand)(edict_t *self);
	void		(*idle)(edict_t *self);
	void		(*search)(edict_t *self);
	void		(*walk)(edict_t *self);
	void		(*run)(edict_t *self);
	void		(*dodge)(edict_t *self, edict_t *other, float eta);
	void		(*attack)(edict_t *self);
	void		(*melee)(edict_t *self);
	void		(*sight)(edict_t *self, edict_t *other);
	qboolean	(*checkattack)(edict_t *self);

	float		pausetime{0.0f};
	float		attack_finished{0.0f};

	vec3_t		saved_goal;
	float		search_time{0.0f};
	float		trail_time{0.0f};
	vec3_t		last_sighting;
	int			attack_state{0};
	int			lefty{0};
	float		idle_time{0.0f};
	int			linkcount{0};

	int			power_armor_type{0};
	int			power_armor_power{0};
};