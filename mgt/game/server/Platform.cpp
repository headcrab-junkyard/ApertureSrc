/*
	plats.qc

	platform functions

	Copyright (C) 1996-1997  Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

/// @file

#include "GameEntity.hpp"

class CPlatform : public CGameEntity
{
public:
	void center_touch();
	void outside_touch();

	void trigger_use();

	void GoUp();
	void GoDown();

	void crush();
};

const float PLAT_LOW_TRIGGER = 1;

void plat_spawn_inside_trigger()
{
	entity trigger;
	vector tmin, tmax;

	//
	// middle trigger
	//
	trigger = spawn();
	trigger->SetTouchCallback(plat_center_touch);
	trigger->v.movetype = MOVETYPE_NONE;
	trigger->v.solid = SOLID_TRIGGER;
	trigger->v.enemy = self;

	tmin = self->v.mins + '25 25 0';
	tmax = self->v.maxs - '25 25 -8';

	tmin_z = tmax_z - (self->pos1_z - self->pos2_z + 8);

	if(self->v.spawnflags & PLAT_LOW_TRIGGER)
		tmax_z = tmin_z + 8;

	if(self->v.size_x <= 50)
	{
		tmin_x = (self->mins_x + self->maxs_x) / 2;
		tmax_x = tmin_x + 1;
	};

	if(self->size_y <= 50)
	{
		tmin_y = (self->mins_y + self->maxs_y) / 2;
		tmax_y = tmin_y + 1;
	};

	pEngine->pfnSetSize(trigger, tmin, tmax);
};

void CPlatform::hit_top()
{
	gpEngine->pfnEmitSound(self, CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	self->state = STATE_TOP;
	self->SetThinkCallback(CPlatform::GoDown);
	self->nextthink = self->ltime + 3;
};

void CPlatform::hit_bottom()
{
	pEngine->pfnEmitSound(self, CHAN_NO_PHS_ADD + CHAN_VOICE, self->noise1, 1, ATTN_NORM);
	self->state = STATE_BOTTOM;
};

void CPlatform::GoDown()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self->noise, 1, ATTN_NORM);
	self->state = STATE_DOWN;
	SUB_CalcMove(self->pos2, self->speed, hit_bottom);
};

void CPlatform::GoUp()
{
	gpEngine->pfnEmitSound(self, CHAN_VOICE, self->noise, 1, ATTN_NORM);
	self->state = STATE_UP;
	SUB_CalcMove(self->pos1, self->speed, hit_top);
};

void CPlatform::center_touch(CBaseEntity *other)
{
	if(other->GetClassName() != "player")
		return;

	if(other->GetHealth() <= 0)
		return;

	self = self->enemy;

	if(self->state == STATE_BOTTOM)
		GoUp();
	else if(self->state == STATE_TOP)
		self->nextthink = self->ltime + 1; // delay going down
};

void CPlatform::outside_touch(CBaseEntity *other)
{
	if(other->GetClassName() != "player")
		return;

	if(other->GetHealth() <= 0)
		return;

	//dprint ("plat_outside_touch\n");
	self = self->enemy;

	if(self->v.state == STATE_TOP)
		GoDown();
};

void CPlatform::trigger_use()
{
	// already activated
	if(self->think)
		return;

	GoDown();
};

void CPlatform::crush(CBaseEntity *other)
{
	//dprint ("plat_crush\n");

	other->v.deathtype = "squish";
	T_Damage(other, self, self, 1);

	if(self->state == STATE_UP)
		GoDown();
	else if(self->state == STATE_DOWN)
		GoUp();
	else
		objerror("plat_crush: bad self.state\n");
};

void CPlatform::Use()
{
	self->SetUseCallback(SUB_Null);
	if(self->state != STATE_UP)
		objerror("plat_use: not in up state");
	GoDown();
};

/*QUAKED func_plat (0 .5 .8) ? PLAT_LOW_TRIGGER
speed	default 150

Plats are always drawn in the extended position, so they will light correctly.

If the plat is the target of another trigger or button, it will start out disabled in the extended position until it is trigger, when it will lower and become a normal plat.

If the "height" key is set, that will determine the amount the plat moves, instead of being implicitly determined by the model's height.
Set "sounds" to one of the following:
1) base fast
2) chain slow
*/
C_EXPORT void func_plat(entvars_t *self)
{
	entity t;

	if(!self->t_length)
		self->t_length = 80;
	if(!self->t_width)
		self->t_width = 10;

	if(self->sounds == 0)
		self->sounds = 2;

	// FIX THIS TO LOAD A GENERIC PLAT SOUND

	if(self->sounds == 1)
	{
		gpEngine->pfnPrecacheSound("plats/plat1.wav");
		gpEngine->pfnPrecacheSound("plats/plat2.wav");
		self->noise = "plats/plat1.wav";
		self->noise1 = "plats/plat2.wav";
	};

	if(self->sounds == 2)
	{
		gpEngine->pfnPrecacheSound("plats/medplat1.wav");
		gpEngine->pfnPrecacheSound("plats/medplat2.wav");
		self->noise = "plats/medplat1.wav";
		self->noise1 = "plats/medplat2.wav";
	};

	self->mangle = self->angles;
	self->angles = '0 0 0';

	self->classname = "plat";
	self->solid = SOLID_BSP;
	self->movetype = MOVETYPE_PUSH;

	gpEngine->pfnSetOrigin(self, self->origin);
	gpEngine->pfnSetModel(self, self->model);
	gpEngine->pfnSetSize(self, self->mins, self->maxs);

	self->blocked = plat_crush;

	if(!self->speed)
		self->speed = 150;

	// pos1 is the top position, pos2 is the bottom
	self->pos1 = self->origin;
	self->pos2 = self->origin;

	if(self->height)
		self->pos2_z = self->origin_z - self->height;
	else
		self->pos2_z = self->origin_z - self->size_z + 8;

	self->use = plat_trigger_use;

	plat_spawn_inside_trigger(self); // the "start moving" trigger

	if(self->targetname)
	{
		self->state = STATE_UP;
		self->use = plat_use;
	}
	else
	{
		gpEngine->pfnSetOrigin(self, self->pos2);
		self->state = STATE_BOTTOM;
	};
};