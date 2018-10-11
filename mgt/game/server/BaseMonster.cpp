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

#include "BaseMonster.hpp"

CBaseMonster::CBaseMonster() = default;
CBaseMonster::~CBaseMonster() = default;

/*
================
monster_use

Using a monster makes it angry at the current activator
================
*/
void CBaseMonster::Use(CBaseEntity *activator)
{
	// No enemy?
	if(self->enemy)
		return;

	// I'm dead
	if(self->health <= 0)
		return;

	// He's invisible!
	if(activator->items & IT_INVISIBILITY) // TODO: not present in q2
		return;

	// He's a cheater!!
	if(activator->flags & FL_NOTARGET)
		return;

	// TODO: q2
	//if(!(activator->client) && !(activator->monsterinfo.aiflags & AI_GOOD_GUY))
		//return;
	
	if(activator->classname != "player")
		return;

	// delay reaction so if the monster is teleported, its sound is still heard
	self->enemy = activator;
	self->nextthink = time + 0.1; // TODO: not present in q2
	self->SetThinkCallback(FoundTarget);
};

void CBaseMonster::Think()
{
	MoveFrame();
	
	if (self->linkcount != self->linkcount)
	{
		self->linkcount = self->linkcount;
		CheckGround();
	};
	
	CatagorizePosition ();
	WorldEffects();
	M_SetEffects (self);
};

void CBaseMonster::MoveFrame()
{
	mmove_t	*move;
	int		index;

	move = self->currentmove;
	self->nextthink = level.time + FRAMETIME;

	if ((self->nextframe) && (self->nextframe >= move->firstframe) && (self->nextframe <= move->lastframe))
	{
		self->s.frame = self->nextframe;
		self->nextframe = 0;
	}
	else
	{
		if (self->s.frame == move->lastframe)
		{
			if (move->endfunc)
			{
				move->endfunc (self);

				// regrab move, endfunc is very likely to change it
				move = self->currentmove;

				// check for death
				if (self->svflags & SVF_DEADMONSTER)
					return;
			};
		};

		if (self->s.frame < move->firstframe || self->s.frame > move->lastframe)
		{
			self->aiflags &= ~AI_HOLD_FRAME;
			self->s.frame = move->firstframe;
		}
		else
		{
			if (!(self->aiflags & AI_HOLD_FRAME))
			{
				self->s.frame++;
				if (self->s.frame > move->lastframe)
					self->s.frame = move->firstframe;
			};
		};
	};

	index = self->s.frame - move->firstframe;
	if (move->frame[index].aifunc)
		if (!(self->aiflags & AI_HOLD_FRAME))
			move->frame[index].aifunc (self, move->frame[index].dist * self->monsterinfo.scale);
		else
			move->frame[index].aifunc (self, 0);

	if (move->frame[index].thinkfunc)
		move->frame[index].thinkfunc (self);
};

void CBaseMonster::CheckGround()
{
	vec3_t		point;
	trace_t		trace;

	if (self->flags & (FL_SWIM|FL_FLY))
		return;

	if (self->velocity[2] > 100)
	{
		self->groundentity = nullptr;
		return;
	};

// if the hull point one-quarter unit down is solid the entity is on ground
	point[0] = self->s.origin[0];
	point[1] = self->s.origin[1];
	point[2] = self->s.origin[2] - 0.25;

	trace = gi.trace (self->s.origin, self->mins, self->maxs, point, self, MASK_MONSTERSOLID);

	// check steepness
	if ( trace.plane.normal[2] < 0.7 && !trace.startsolid)
	{
		self->groundentity = nullptr;
		return;
	};

//	self->groundentity = trace.ent;
//	self->groundentity_linkcount = trace.ent->linkcount;
//	if (!trace.startsolid && !trace.allsolid)
//		VectorCopy (trace.endpos, self->s.origin);
	if (!trace.startsolid && !trace.allsolid)
	{
		VectorCopy (trace.endpos, self->s.origin);
		self->groundentity = trace.ent;
		self->groundentity_linkcount = trace.ent->linkcount;
		self->velocity[2] = 0;
	};
};

void CBaseMonster::CatagorizePosition()
{
	vec3_t		point;
	int			cont;

	//
	// get waterlevel
	//
	point[0] = self->s.origin[0];
	point[1] = self->s.origin[1];
	point[2] = self->s.origin[2] + self->mins[2] + 1;	
	cont = gi.pointcontents (point);

	if (!(cont & MASK_WATER))
	{
		self->waterlevel = 0;
		self->watertype = 0;
		return;
	};

	self->watertype = cont;
	self->waterlevel = 1;
	point[2] += 26;
	cont = gi.pointcontents (point);
	
	if (!(cont & MASK_WATER))
		return;

	self->waterlevel = 2;
	point[2] += 22;
	cont = gi.pointcontents (point);
	
	if (cont & MASK_WATER)
		self->waterlevel = 3;
};

void CBaseMonster::WorldEffects()
{
	int		dmg;

	if (self->health > 0)
	{
		if (!(self->flags & FL_SWIM))
		{
			if (self->waterlevel < 3)
				self->air_finished = level.time + 12;
			else if (self->air_finished < level.time)
			{	// drown!
				if (self->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floor(level.time - self->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage (self, world, world, vec3_origin, self->s.origin, vec3_origin, dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
					self->pain_debounce_time = level.time + 1;
				};
			};
		}
		else
		{
			if (self->waterlevel > 0)
				self->air_finished = level.time + 9;
			else if (self->air_finished < level.time)
			{	// suffocate!
				if (self->pain_debounce_time < level.time)
				{
					dmg = 2 + 2 * floor(level.time - self->air_finished);
					if (dmg > 15)
						dmg = 15;
					T_Damage (self, world, world, vec3_origin, self->s.origin, vec3_origin, dmg, 0, DAMAGE_NO_ARMOR, MOD_WATER);
					self->pain_debounce_time = level.time + 1;
				};
			};
		};
	};
	
	if (self->waterlevel == 0)
	{
		if (self->flags & FL_INWATER)
		{	
			gi.sound (self, CHAN_BODY, gi.soundindex("player/watr_out.wav"), 1, ATTN_NORM, 0);
			self->flags &= ~FL_INWATER;
		};
		return;
	};

	if ((self->watertype & CONTENTS_LAVA) && !(self->flags & FL_IMMUNE_LAVA))
	{
		if (self->damage_debounce_time < level.time)
		{
			self->damage_debounce_time = level.time + 0.2;
			T_Damage (self, world, world, vec3_origin, self->s.origin, vec3_origin, 10*self->waterlevel, 0, 0, MOD_LAVA);
		};
	};
	
	if ((self->watertype & CONTENTS_SLIME) && !(self->flags & FL_IMMUNE_SLIME))
	{
		if (self->damage_debounce_time < level.time)
		{
			self->damage_debounce_time = level.time + 1;
			T_Damage (self, world, world, vec3_origin, self->s.origin, vec3_origin, 4*self->waterlevel, 0, 0, MOD_SLIME);
		};
	};
	
	if ( !(self->flags & FL_INWATER) )
	{	
		if (!(self->svflags & SVF_DEADMONSTER))
		{
			if (self->watertype & CONTENTS_LAVA)
				if (random() <= 0.5)
					gi.sound (self, CHAN_BODY, gi.soundindex("player/lava1.wav"), 1, ATTN_NORM, 0);
				else
					gi.sound (self, CHAN_BODY, gi.soundindex("player/lava2.wav"), 1, ATTN_NORM, 0);
			else if (self->watertype & CONTENTS_SLIME)
				gi.sound (self, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
			else if (self->watertype & CONTENTS_WATER)
				gi.sound (self, CHAN_BODY, gi.soundindex("player/watr_in.wav"), 1, ATTN_NORM, 0);
		};

		self->flags |= FL_INWATER;
		self->damage_debounce_time = 0;
	};
};

void CBaseMonster::SetEffects()
{
	self->s.effects &= ~(EF_COLOR_SHELL|EF_POWERSCREEN);
	self->s.renderfx &= ~(RF_SHELL_RED|RF_SHELL_GREEN|RF_SHELL_BLUE);

	if (self->aiflags & AI_RESURRECTING)
	{
		self->s.effects |= EF_COLOR_SHELL;
		self->s.renderfx |= RF_SHELL_RED;
	};

	if (self->health <= 0)
		return;

	if (self->powerarmor_time > level.time)
	{
		if (self->power_armor_type == POWER_ARMOR_SCREEN)
			self->s.effects |= EF_POWERSCREEN;
		else if (self->power_armor_type == POWER_ARMOR_SHIELD)
		{
			self->s.effects |= EF_COLOR_SHELL;
			self->s.renderfx |= RF_SHELL_GREEN;
		};
	};
};

void CBaseMonster::ReactToDamage (edict_t *targ, edict_t *attacker)
{
	if (!(attacker->client) && !(attacker->svflags & SVF_MONSTER))
		return;

	if (attacker == targ || attacker == targ->enemy)
		return;

	// if we are a good guy monster and our attacker is a player
	// or another good guy, do not get mad at them
	if (targ->monsterinfo.aiflags & AI_GOOD_GUY)
	{
		if (attacker->client || (attacker->monsterinfo.aiflags & AI_GOOD_GUY))
			return;
	}

	// we now know that we are not both good guys

	// if attacker is a client, get mad at them because he's good and we're not
	if (attacker->client)
	{
		targ->monsterinfo.aiflags &= ~AI_SOUND_TARGET;

		// this can only happen in coop (both new and old enemies are clients)
		// only switch if can't see the current enemy
		if (targ->enemy && targ->enemy->client)
		{
			if (visible(targ, targ->enemy))
			{
				targ->oldenemy = attacker;
				return;
			}
			targ->oldenemy = targ->enemy;
		}
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
		return;
	}

	// it's the same base (walk/swim/fly) type and a different classname and it's not a tank
	// (they spray too much), get mad at them
	if (((targ->flags & (FL_FLY|FL_SWIM)) == (attacker->flags & (FL_FLY|FL_SWIM))) &&
		 (strcmp (targ->classname, attacker->classname) != 0) &&
		 (strcmp(attacker->classname, "monster_tank") != 0) &&
		 (strcmp(attacker->classname, "monster_supertank") != 0) &&
		 (strcmp(attacker->classname, "monster_makron") != 0) &&
		 (strcmp(attacker->classname, "monster_jorg") != 0) )
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	}
	// if they *meant* to shoot us, then shoot back
	else if (attacker->enemy == targ)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	}
	// otherwise get mad at whoever they are mad at (help our buddy) unless it is us!
	else if (attacker->enemy && attacker->enemy != targ)
	{
		if (targ->enemy && targ->enemy->client)
			targ->oldenemy = targ->enemy;
		targ->enemy = attacker->enemy;
		if (!(targ->monsterinfo.aiflags & AI_DUCKED))
			FoundTarget (targ);
	};
};