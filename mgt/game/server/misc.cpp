/*
	misc.qc

	pretty much everything else

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

/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for spotlights, etc.
*/
C_EXPORT void info_null(entvars_t *self)
{
	remove(self);
};

/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for lightning.
*/
C_EXPORT void info_notnull(entvars_t *self)
{
};

//============================================================================

const float START_OFF = 1;

void CLight::Use()
{
	if (self->spawnflags & START_OFF)
	{
		gpEngine->pfnLightStyle(self->style, "m");
		self->spawnflags = self->spawnflags - START_OFF;
	}
	else
	{
		gpEngine->pfnLightStyle(self->style, "a");
		self->spawnflags = self->spawnflags + START_OFF;
	};
};

/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
*/
C_EXPORT void light(entvars_t *self)
{
	CLight::OnSpawn();
};

void CLight::OnSpawn()
{
	if(!self->targetname)
	{
		// inert light
		remove(self);
		return;
	};
	
	if (self->style >= 32)
	{
		self->use = light_use;
		if (self->spawnflags & START_OFF)
			gpEngine->pfnLightStyle(self->style, "a");
		else
			gpEngine->pfnLightStyle(self->style, "m");
	};
};

/*QUAKED light_fluoro (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Non-displayed light.
Default light value is 300
Default style is 0
If targeted, it will toggle between on or off.
Makes steady fluorescent humming sound
*/
C_EXPORT void light_fluoro(entvars_t *self)
{
	if (self->style >= 32)
	{
		self->use = light_use;
		
		if (self->spawnflags & START_OFF)
			gpEngine->pfnLightStyle(self->style, "a");
		else
			gpEngine->pfnLightStyle(self->style, "m");
	};
	
	gpEngine->pfnPrecacheSound ("ambience/fl_hum1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/fl_hum1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_fluorospark (0 1 0) (-8 -8 -8) (8 8 8)
Non-displayed light.
Default light value is 300
Default style is 10
Makes sparking, broken fluorescent sound
*/
C_EXPORT void light_fluorospark(entvars_t *self)
{
	if (!self->style)
		self->style = 10;

	gpEngine->pfnPrecacheSound ("ambience/buzz1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/buzz1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_globe (0 1 0) (-8 -8 -8) (8 8 8)
Sphere globe light.
Default light value is 300
Default style is 0
*/
C_EXPORT void light_globe(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("sprites/s_light.spr");
	gpEngine->pfnSetModel (self, "sprites/s_light.spr");
	gpEngine->pfnMakeStatic (self);
};

void FireAmbient()
{
	gpEngine->pfnPrecacheSound ("ambience/fire1.wav");
// attenuate fast
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/fire1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED light_torch_small_walltorch (0 .5 0) (-10 -10 -20) (10 10 20)
Short wall torch
Default light value is 200
Default style is 0
*/
C_EXPORT void light_torch_small_walltorch(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame.mdl");
	gpEngine->pfnSetModel (self, "models/flame.mdl");
	FireAmbient ();
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_large_yellow (0 1 0) (-10 -10 -12) (12 12 18)
Large yellow flame ball
*/
C_EXPORT void light_flame_large_yellow(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	self->frame = 1;
	FireAmbient ();
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_small_yellow (0 1 0) (-8 -8 -8) (8 8 8) START_OFF
Small yellow flame ball
*/
C_EXPORT void light_flame_small_yellow(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	FireAmbient ();
	gpEngine->pfnMakeStatic (self);
};

/*QUAKED light_flame_small_white (0 1 0) (-10 -10 -40) (10 10 40) START_OFF
Small white flame ball
*/
C_EXPORT void light_flame_small_white(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/flame2.mdl");
	gpEngine->pfnSetModel (self, "models/flame2.mdl");
	FireAmbient ();
	gpEngine->pfnMakeStatic (self);
};

//============================================================================


/*QUAKED misc_fireball (0 .5 .8) (-8 -8 -8) (8 8 8)
Lava Balls
*/

void fire_fly();
void fire_touch();

C_EXPORT void misc_fireball(entvars_t *self)
{
	gpEngine->pfnPrecacheModel ("models/lavaball.mdl");
	
	self->classname = "fireball";
	self->nextthink = time + (random() * 5);
	self->think = fire_fly;
	
	if (!self->speed)
		self->speed == 1000;
};

void fire_fly()
{
	entvars_t fireball = spawn();
	
	fireball.solid = SOLID_TRIGGER;
	fireball.movetype = MOVETYPE_TOSS;
	fireball.velocity = '0 0 1000';
	fireball.velocity_x = (random() * 100) - 50;
	fireball.velocity_y = (random() * 100) - 50;
	fireball.velocity_z = self->speed + (random() * 200);
	fireball.classname = "fireball";
	
	gpEngine->pfnSetModel (fireball, "models/lavaball.mdl");
	gpEngine->pfnSetSize (fireball, '0 0 0', '0 0 0');
	gpEngine->pfnSetOrigin (fireball, self->origin);
	
	fireball.nextthink = time + 5;
	fireball.think = SUB_Remove;
	fireball.touch = fire_touch;
	
	self->nextthink = time + (random() * 5) + 3;
	self->think = fire_fly;
};

void fire_touch()
{
	T_Damage (other, self, self, 20);
	remove(self);
};

//============================================================================

void barrel_explode()
{
	self->takedamage = DAMAGE_NO;
	self->classname = "explo_box";
	
	// did say self->owner
	T_RadiusDamage (self, self, 160, world, "");
	
	gpEngine->pfnWriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	gpEngine->pfnWriteByte (MSG_MULTICAST, TE_EXPLOSION);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->origin_x);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->origin_y);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->origin_z+32);
	
	multicast (self->origin, MULTICAST_PHS);
	remove (self);
};

/*QUAKED misc_explobox (0 .5 .8) (0 0 0) (32 32 64)
TESTING THING
*/
C_EXPORT void misc_explobox(entvars_t *self)
{
	float     oldz;
	
	self->solid = SOLID_BBOX;
	self->movetype = MOVETYPE_NONE;
	
	gpEngine->pfnPrecacheModel ("maps/b_explob.bsp");
	gpEngine->pfnSetModel (self, "maps/b_explob.bsp");
	gpEngine->pfnSetSize (self, '0 0 0', '32 32 64');
	gpEngine->pfnPrecacheSound ("weapons/r_exp3.wav");
	
	self->health = 20;
	self->th_die = barrel_explode;
	self->takedamage = DAMAGE_AIM;

	self->origin_z += 2;
	oldz = self->origin_z;
	
	gpEngine->pfnDropToFloor(self);
	
	if (oldz - self->origin_z > 250)
	{
		dprint ("item fell out of level at ");
		dprint (vtos(self->origin));
		dprint ("\n");
		remove(self);
	};
};

/*QUAKED misc_explobox2 (0 .5 .8) (0 0 0) (32 32 64)
Smaller exploding box, REGISTERED ONLY
*/
C_EXPORT void misc_explobox2(entvars_t *self)
{
	float     oldz;
	
	self->solid = SOLID_BBOX;
	self->movetype = MOVETYPE_NONE;
	
	gpEngine->pfnPrecacheModel ("maps/b_exbox2.bsp");
	gpEngine->pfnSetModel (self, "maps/b_exbox2.bsp");
	gpEngine->pfnSetSize (self, '0 0 0', '32 32 32');
	gpEngine->pfnPrecacheSound ("weapons/r_exp3.wav");
	
	self->health = 20;
	self->th_die = barrel_explode;
	self->takedamage = DAMAGE_AIM;

	self->origin_z += 2;
	oldz = self->origin_z;
	
	gpEngine->pfnDropToFloor(self);
	
	if (oldz - self->origin_z > 250)
	{
		dprint ("item fell out of level at ");
		dprint (vtos(self->origin));
		dprint ("\n");
		remove(self);
	};
};

//============================================================================

const float SPAWNFLAG_SUPERSPIKE      = 1;
const float SPAWNFLAG_LASER = 2;

void Laser_Touch()
{
	vector org;
	
	if (other == self->owner)
		return;         // don't explode on owner

	if (pointcontents(self->origin) == CONTENT_SKY)
	{
		remove(self);
		return;
	};
	
	gpEngine->pfnEmitSound (self, CHAN_WEAPON, "enforcer/enfstop.wav", 1, ATTN_STATIC);
	org = self->origin - 8*normalize(self->velocity);

	if (other.health)
	{
		SpawnBlood (org, 15);
		other.deathtype = "laser";
		T_Damage (other, self, self->owner, 15);
	}
	else
	{
		gpEngine->pfnWriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		gpEngine->pfnWriteByte (MSG_MULTICAST, TE_GUNSHOT);
		gpEngine->pfnWriteByte (MSG_MULTICAST, 5);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, org_x);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, org_y);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, org_z);
		multicast (org, MULTICAST_PVS);
	};
	
	remove(self);   
};

void LaunchLaser(vector org, vector vec)
{
	vector  vec;
		
	if (self->classname == "monster_enforcer")
		gpEngine->pfnEmitSound (self, CHAN_WEAPON, "enforcer/enfire.wav", 1, ATTN_NORM);

	vec = normalize(vec);
	
	newmis = spawn();
	newmis.owner = self;
	newmis.movetype = MOVETYPE_FLY;
	newmis.solid = SOLID_BBOX;
	newmis.effects = EF_DIMLIGHT;

	gpEngine->pfnSetModel (newmis, "models/laser.mdl");
	gpEngine->pfnSetSize (newmis, '0 0 0', '0 0 0');             

	gpEngine->pfnSetOrigin (newmis, org);

	newmis.velocity = vec * 600;
	newmis.angles = vectoangles(newmis.velocity);

	newmis.nextthink = time + 5;
	newmis.think = SUB_Remove;
	newmis.touch = Laser_Touch;
};

void CTrapShooter::use()
{
	if (self->spawnflags & SPAWNFLAG_LASER)
	{
		gpEngine->pfnEmitSound (self, CHAN_VOICE, "enforcer/enfire.wav", 1, ATTN_NORM);
		LaunchLaser (self->origin, self->movedir);
	}
	else
	{
		gpEngine->pfnEmitSound (self, CHAN_VOICE, "weapons/spike2.wav", 1, ATTN_NORM);
		launch_spike (self->origin, self->movedir);
		newmis.velocity = self->movedir * 500;
		if (self->spawnflags & SPAWNFLAG_SUPERSPIKE)
			newmis.touch = superspike_touch;
	};
};

void CTrapShooter::Think()
{
	Use();
	
	self->nextthink = time + self->wait;
	newmis.velocity = self->movedir * 500;
};

/*QUAKED trap_spikeshooter (0 .5 .8) (-8 -8 -8) (8 8 8) superspike laser
When triggered, fires a spike in the direction set in QuakeEd.
Laser is only for REGISTERED.
*/
C_EXPORT void trap_spikeshooter(entvars_t *self)
{
	SetMovedir ();
	
	self->use = this->Use;
	
	if (self->spawnflags & SPAWNFLAG_LASER)
	{
		gpEngine->pfnPrecacheModel ("models/laser.mdl");
		
		gpEngine->pfnPrecacheSound ("enforcer/enfire.wav");
		gpEngine->pfnPrecacheSound ("enforcer/enfstop.wav");
	}
	else
		gpEngine->pfnPrecacheSound ("weapons/spike2.wav");
};

/*QUAKED trap_shooter (0 .5 .8) (-8 -8 -8) (8 8 8) superspike laser
Continuously fires spikes.
"wait" time between spike (1.0 default)
"nextthink" delay before firing first spike, so multiple shooters can be stagered.
*/
C_EXPORT void trap_shooter(entvars_t *self)
{
	CTrapShooter::Spawn();
};

void CTrapShooter::Spawn() // CSpikeShooter
{
	trap_spikeshooter();
	
	if (self->wait == 0)
		self->wait = 1;
	
	self->nextthink = self->nextthink + self->wait + self->ltime;
	self->think = this->Think;
};

/*
===============================================================================


===============================================================================
*/

void make_bubbles();
void bubble_remove();
void bubble_bob();

/*QUAKED air_bubbles (0 .5 .8) (-8 -8 -8) (8 8 8)

testing air bubbles
*/
C_EXPORT void air_bubbles(entvars_t *self)
{
	remove(self);
};

void make_bubbles()
{
	entvars_t bubble = spawn();
	
	gpEngine->pfnSetModel (bubble, "sprites/s_bubble.spr");
	gpEngine->pfnSetOrigin (bubble, self->origin);
	
	bubble->v.movetype = MOVETYPE_NOCLIP;
	bubble->v.solid = SOLID_NOT;
	bubble->v.velocity = '0 0 15';
	bubble->v.nextthink = time + 0.5;
	bubble->v.think = bubble_bob;
	bubble->v.touch = bubble_remove;
	bubble->v.classname = "bubble";
	bubble->v.frame = 0;
	bubble->v.cnt = 0;
	
	gpEngine->pfnSetSize (bubble, '-8 -8 -8', '8 8 8');
	
	self->nextthink = time + random() + 0.5;
	self->think = make_bubbles;
};

void bubble_split()
{
	entvars_t bubble = spawn();
	
	gpEngine->pfnSetModel (bubble, "sprites/s_bubble.spr");
	gpEngine->pfnSetOrigin (bubble, self->origin);
	
	bubble.movetype = MOVETYPE_NOCLIP;
	bubble.solid = SOLID_NOT;
	bubble.velocity = self->velocity;
	bubble.nextthink = time + 0.5;
	bubble.think = bubble_bob;
	bubble.touch = bubble_remove;
	bubble.classname = "bubble";
	bubble.frame = 1;
	bubble.cnt = 10;
	
	gpEngine->pfnSetSize (bubble, '-8 -8 -8', '8 8 8');
	
	self->frame = 1;
	self->cnt = 10;
	
	if (self->waterlevel != 3)
		remove (self);
};

void bubble_remove()
{
	if (other.classname == self->classname)
	{
//              dprint ("bump");
		return;
	};
	
	remove(self);
};

void bubble_bob()
{
	float             rnd1, rnd2, rnd3;
	vector    vtmp1, modi;

	self->cnt += 1;
	
	if (self->cnt == 4)
		bubble_split();
	
	if (self->cnt == 20)
		remove(self);

	rnd1 = self->velocity_x + (-10 + (random() * 20));
	rnd2 = self->velocity_y + (-10 + (random() * 20));
	rnd3 = self->velocity_z + 10 + random() * 10;

	if (rnd1 > 10)
		rnd1 = 5;
	if (rnd1 < -10)
		rnd1 = -5;
		
	if (rnd2 > 10)
		rnd2 = 5;
	if (rnd2 < -10)
		rnd2 = -5;
		
	if (rnd3 < 10)
		rnd3 = 15;
	if (rnd3 > 30)
		rnd3 = 25;
	
	self->velocity_x = rnd1;
	self->velocity_y = rnd2;
	self->velocity_z = rnd3;
		
	self->nextthink = time + 0.5;
	self->think = bubble_bob;
};

/*~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>
~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~<~>~*/

/*QUAKED viewthing (0 .5 .8) (-8 -8 -8) (8 8 8)

Just for the debugging level.  Don't use
*/
C_EXPORT void viewthing(entvars_t *self)
{
	self->movetype = MOVETYPE_NONE;
	self->solid = SOLID_NOT;
	
	gpEngine->pfnPrecacheModel ("models/player.mdl");
	gpEngine->pfnSetModel (self, "models/player.mdl");
};


/*
==============================================================================

SIMPLE BMODELS

==============================================================================
*/

class CWall : public CGameEntity
{
public:
	void Spawn();
	void Use();
};

void CWall::Use()
{
	// change to alternate textures
	self->frame = 1 - self->frame;
};

/*QUAKED func_wall (0 .5 .8) ?
This is just a solid wall if not inhibitted
*/
C_EXPORT void func_wall(entvars_t *self)
{
	CWall::Spawn();
};

void CWall::Spawn()
{
	self->angles = '0 0 0';
	self->movetype = MOVETYPE_PUSH;  // so it doesn't get pushed by anything
	self->solid = SOLID_BSP;
	self->use = CWall::Use;
	
	gpEngine->pfnSetModel(self, self->model);
};

/*QUAKED func_illusionary (0 .5 .8) ?
A simple entity that looks solid but lets you walk through it.
*/
C_EXPORT void func_illusionary(entvars_t *self)
{
	self->angles = '0 0 0';
	self->movetype = MOVETYPE_NONE;
	self->solid = SOLID_NOT;
	
	gpEngine->pfnSetModel(self, self->model);
	
	gpEngine->pfnMakeStatic(self);
};

/*QUAKED func_episodegate (0 .5 .8) ? E1 E2 E3 E4
This bmodel will appear if the episode has allready been completed, so players can't reenter it.
*/
C_EXPORT void func_episodegate(entvars_t *self)
{
	if (!(serverflags & self->spawnflags))
		return;                 // can still enter episode

	self->angles = '0 0 0';
	self->movetype = MOVETYPE_PUSH;  // so it doesn't get pushed by anything
	self->solid = SOLID_BSP;
	self->use = CWall::Use;
	
	gpEngine->pfnSetModel (self, self->model);
};

/*QUAKED func_bossgate (0 .5 .8) ?
This bmodel appears unless players have all of the episode sigils.
*/
C_EXPORT void func_bossgate(entvars_t *self)
{
	if ( (serverflags & 15) == 15)
		return;         // all episodes completed
	
	self->angles = '0 0 0';
	self->movetype = MOVETYPE_PUSH;  // so it doesn't get pushed by anything
	self->solid = SOLID_BSP;
	self->use = CWall::Use;
	
	gpEngine->pfnSetModel (self, self->model);
};

//============================================================================

/*QUAKED ambient_suck_wind (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_suck_wind(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/suck1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/suck1.wav", 1, ATTN_STATIC);
};

/*QUAKED ambient_drone (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_drone(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/drone6.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/drone6.wav", 0.5, ATTN_STATIC);
};

/*QUAKED ambient_flouro_buzz (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_flouro_buzz(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/buzz1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/buzz1.wav", 1, ATTN_STATIC);
};

/*QUAKED ambient_drip (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_drip(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/drip1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/drip1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED ambient_comp_hum (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_comp_hum(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/comp1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/comp1.wav", 1, ATTN_STATIC);
};

/*QUAKED ambient_thunder (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_thunder(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/thunder1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/thunder1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED ambient_light_buzz (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_light_buzz(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/fl_hum1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/fl_hum1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED ambient_swamp1 (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_swamp1(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/swamp1.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/swamp1.wav", 0.5, ATTN_STATIC);
};

/*QUAKED ambient_swamp2 (0.3 0.1 0.6) (-10 -10 -8) (10 10 8)
*/
C_EXPORT void ambient_swamp2(entvars_t *self)
{
	gpEngine->pfnPrecacheSound ("ambience/swamp2.wav");
	gpEngine->pfnEmitAmbientSound (self->origin, "ambience/swamp2.wav", 0.5, ATTN_STATIC);
};

//============================================================================

class CNoiseMaker : public CGameEntity
{
public:
	void Spawn() override;
	void Think() override;
};

/*QUAKED misc_noisemaker (1 0.5 0) (-10 -10 -10) (10 10 10)

For optimzation testing, starts a lot of sounds.
*/
C_EXPORT void misc_noisemaker(entvars_t *self)
{
	CNoiseMaker::Spawn();
};

void CNoiseMaker::Spawn()
{
	gpEngine->pfnPrecacheSound ("enforcer/enfire.wav");
	gpEngine->pfnPrecacheSound ("enforcer/enfstop.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight2.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight3.wav");
	gpEngine->pfnPrecacheSound ("enforcer/sight4.wav");
	gpEngine->pfnPrecacheSound ("enforcer/pain1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/pain2.wav");
	gpEngine->pfnPrecacheSound ("enforcer/death1.wav");
	gpEngine->pfnPrecacheSound ("enforcer/idle1.wav");

	self->nextthink = time + 0.1 + random();
	self->think = this->Think;
};

void CNoiseMaker::Think()
{
	self->nextthink = time + 0.5;
	
	gpEngine->pfnEmitSound (self, 1, "enforcer/enfire.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 2, "enforcer/enfstop.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 3, "enforcer/sight1.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 4, "enforcer/sight2.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 5, "enforcer/sight3.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 6, "enforcer/sight4.wav", 1, ATTN_NORM);
	gpEngine->pfnEmitSound (self, 7, "enforcer/pain1.wav", 1, ATTN_NORM);
};