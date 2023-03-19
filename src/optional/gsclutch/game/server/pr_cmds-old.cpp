



/*
===============================================================================

						BUILT-IN FUNCTIONS

===============================================================================
*/

char *PF_VarString(int first)
{
	int i;
	static char out[256];

	out[0] = 0;
	for(i = first; i < pr_argc; i++)
	{
		strcat(out, G_STRING((OFS_PARM0 + i * 3)));
	}
	return out;
}

/*
=================
PF_errror

This is a TERMINAL error, which will kill off the entire server.
Dumps self.

error(value)
=================
*/
void PF_error(const char *s)
{
	edict_t *ed;

	Con_Printf("======SERVER ERROR in %s:\n%s\n", pr_strings + pr_xfunction->s_name, s);
	ed = PROG_TO_EDICT(gGlobalVariables.self);
	ED_Print(ed);

	Host_Error("Program error");
}

/*
=================
PF_objerror

Dumps out self, then an error message.  The program is aborted and self is
removed, but the level can continue.

objerror(value)
=================
*/
void PF_objerror(const char *s)
{
	edict_t *ed;

	Con_Printf("======OBJECT ERROR in %s:\n%s\n", pr_strings + pr_xfunction->s_name, s);
	ed = PROG_TO_EDICT(gGlobalVariables.self);
	ED_Print(ed);
	ED_Free(ed);

	Host_Error("Program error");
}

/*
==============
PF_makevectors

Writes new values for v_forward, v_up, and v_right based on angles
makevectors(vector)
==============
*/
void PF_makevectors(float *v)
{
	AngleVectors(v, gGlobalVariables.v_forward, gGlobalVariables.v_right, gGlobalVariables.v_up);
}





/*
=================
PF_bprint

broadcast print to everyone on server

bprint(value)
=================
*/
void PF_bprint(const char *s)
{
	SV_BroadcastPrintf("%s", s);
}

/*
=================
PF_normalize

vector normalize(vector)
=================
*/
void PF_normalize(float *value1)
{
	vec3_t newvalue;
	float fnew;

	fnew = value1[0] * value1[0] + value1[1] * value1[1] + value1[2] * value1[2];
	fnew = sqrt(fnew);

	if(fnew == 0)
		newvalue[0] = newvalue[1] = newvalue[2] = 0;
	else
	{
		fnew = 1 / fnew;
		newvalue[0] = value1[0] * fnew;
		newvalue[1] = value1[1] * fnew;
		newvalue[2] = value1[2] * fnew;
	}

	VectorCopy(newvalue, G_VECTOR(OFS_RETURN));
}

/*
=================
PF_vlen

scalar vlen(vector)
=================
*/
float PF_vlen(float *value1)
{
	float fnew;

	fnew = value1[0] * value1[0] + value1[1] * value1[1] + value1[2] * value1[2];
	fnew = sqrt(fnew);

	return fnew;
}

/*
=================
PF_vectoyaw

float vectoyaw(vector)
=================
*/
float PF_vectoyaw(float *value1)
{
	float yaw;

	if(value1[1] == 0 && value1[0] == 0)
		yaw = 0;
	else
	{
		yaw = (int)(atan2(value1[1], value1[0]) * 180 / M_PI);
		if(yaw < 0)
			yaw += 360;
	}

	return yaw;
}

/*
=================
PF_vectoangles

vector vectoangles(vector)
=================
*/
float *PF_vectoangles(float *value1)
{
	float forward;
	float yaw, pitch;

	if(value1[1] == 0 && value1[0] == 0)
	{
		yaw = 0;
		if(value1[2] > 0)
			pitch = 90;
		else
			pitch = 270;
	}
	else
	{
		yaw = (int)(atan2(value1[1], value1[0]) * 180 / M_PI);
		if(yaw < 0)
			yaw += 360;

		forward = sqrt(value1[0] * value1[0] + value1[1] * value1[1]);
		pitch = (int)(atan2(value1[2], forward) * 180 / M_PI);
		if(pitch < 0)
			pitch += 360;
	}

	G_FLOAT(OFS_RETURN + 0) = pitch;
	G_FLOAT(OFS_RETURN + 1) = yaw;
	G_FLOAT(OFS_RETURN + 2) = 0;
}

/*
=================
PF_Random

Returns a number from 0<= num < 1

random()
=================
*/
float PF_random()
{
	float num = (rand() & 0x7fff) / ((float)0x7fff);

	return num;
}





/*
=================
PF_sound

Each entity can have eight independant sound sources, like voice,
weapon, feet, etc.

Channel 0 is an auto-allocate channel, the others override anything
allready running on that entity/channel pair.

An attenuation of 0 will play full volume everywhere in the level.
Larger attenuations will drop off.

=================
*/
void PF_sound(edict_t *entity, int channel, const char *sample, int volume, float attenuation)
{
	volume *= 255;

	if(volume < 0 || volume > 255)
		Sys_Error("SV_StartSound: volume = %i", volume);

	if(attenuation < 0 || attenuation > 4)
		Sys_Error("SV_StartSound: attenuation = %f", attenuation);

	if(channel < 0 || channel > 7)
		Sys_Error("SV_StartSound: channel = %i", channel);

	SV_StartSound(entity, channel, sample, volume, attenuation);
}

/*
=================
PF_break

break()
=================
*/
void PF_break()
{
	Con_Printf("break statement\n");
	*(int *)-4 = 0; // dump to debugger
	                //	Host_Error ("break statement");
}

/*
=================
PF_traceline

Used for use tracing and shot targeting
Traces are blocked by bbox and exact bsp entityes, and also slide box entities
if the tryents flag is set.

traceline (vector1, vector2, tryents)
=================
*/
void PF_traceline(float *v1, float *v2, int nomonsters, edict_t *ent)
{
	trace_t trace;

	trace = SV_Move(v1, vec3_origin, vec3_origin, v2, nomonsters, ent);

	gGlobalVariables.trace_allsolid = trace.allsolid;
	gGlobalVariables.trace_startsolid = trace.startsolid;
	gGlobalVariables.trace_fraction = trace.fraction;
	gGlobalVariables.trace_inwater = trace.inwater;
	gGlobalVariables.trace_inopen = trace.inopen;

	VectorCopy(trace.endpos, gGlobalVariables.trace_endpos);
	VectorCopy(trace.plane.normal, gGlobalVariables.trace_plane_normal);

	gGlobalVariables.trace_plane_dist = trace.plane.dist;

	if(trace.ent)
		gGlobalVariables.trace_ent = EDICT_TO_PROG(trace.ent);
	else
		gGlobalVariables.trace_ent = EDICT_TO_PROG(sv.edicts);
}

#ifdef QUAKE2
extern trace_t SV_Trace_Toss(edict_t *ent, edict_t *ignore);

void PF_TraceToss(edict_t *ent, edict_t *ignore)
{
	trace_t trace;

	trace = SV_Trace_Toss(ent, ignore);

	gGlobalVariables.trace_allsolid = trace.allsolid;
	gGlobalVariables.trace_startsolid = trace.startsolid;
	gGlobalVariables.trace_fraction = trace.fraction;
	gGlobalVariables.trace_inwater = trace.inwater;
	gGlobalVariables.trace_inopen = trace.inopen;

	VectorCopy(trace.endpos, gGlobalVariables.trace_endpos);
	VectorCopy(trace.plane.normal, gGlobalVariables.trace_plane_normal);

	gGlobalVariables.trace_plane_dist = trace.plane.dist;

	if(trace.ent)
		gGlobalVariables.trace_ent = EDICT_TO_PROG(trace.ent);
	else
		gGlobalVariables.trace_ent = EDICT_TO_PROG(sv.edicts);
}
#endif

/*
=================
PF_checkpos

Returns true if the given entity can move to the given position from it's
current position by walking or rolling.
FIXME: make work...
scalar checkpos (entity, vector)
=================
*/
qboolean PF_checkpos(edict_t *ed, float *v)
{
	return false;
}

//============================================================================

byte checkpvs[MAX_MAP_LEAFS / 8];

int PF_newcheckclient(int check)
{
	int i;
	byte *pvs;
	edict_t *ent;
	mleaf_t *leaf;
	vec3_t org;

	// cycle to the next one

	if(check < 1)
		check = 1;
	if(check > svs.maxclients)
		check = svs.maxclients;

	if(check == svs.maxclients)
		i = 1;
	else
		i = check + 1;

	for(;; i++)
	{
		if(i == svs.maxclients + 1)
			i = 1;

		ent = EDICT_NUM(i);

		if(i == check)
			break; // didn't find anything else

		if(ent->free)
			continue;
		if(ent->v.health <= 0)
			continue;
		if((int)ent->v.flags & FL_NOTARGET)
			continue;

		// anything that is a client, or has a client as an enemy
		break;
	}

	// get the PVS for the entity
	VectorAdd(ent->v.origin, ent->v.view_ofs, org);
	leaf = Mod_PointInLeaf(org, sv.worldmodel);
	pvs = Mod_LeafPVS(leaf, sv.worldmodel);
	memcpy(checkpvs, pvs, (sv.worldmodel->numleafs + 7) >> 3);

	return i;
}

/*
=================
PF_checkclient

Returns a client (or object that has a client enemy) that would be a
valid target.

If there are more than one valid options, they are cycled each frame

If (self.origin + self.viewofs) is not in the PVS of the current target,
it is not returned at all.

name checkclient ()
=================
*/
#define MAX_CHECK 16
int c_invis, c_notvis;
edict_t *PF_checkclient()
{
	edict_t *ent, *self;
	mleaf_t *leaf;
	int l;
	vec3_t view;

	// find a new check if on a new frame
	if(sv.time - sv.lastchecktime >= 0.1)
	{
		sv.lastcheck = PF_newcheckclient(sv.lastcheck);
		sv.lastchecktime = sv.time;
	}

	// return check if it might be visible
	ent = EDICT_NUM(sv.lastcheck);
	if(ent->free || ent->v.health <= 0)
	{
		return sv.edicts;
	}

	// if current entity can't possibly see the check entity, return 0
	self = PROG_TO_EDICT(gGlobalVariables.self);
	VectorAdd(self->v.origin, self->v.view_ofs, view);
	leaf = Mod_PointInLeaf(view, sv.worldmodel);
	l = (leaf - sv.worldmodel->leafs) - 1;
	if((l < 0) || !(checkpvs[l >> 3] & (1 << (l & 7))))
	{
		c_notvis++;
		return sv.edicts;
	}

	// might be able to see it
	c_invis++;
	return ent;
}

//============================================================================

/*
=================
PF_localcmd

Sends text over to the client's execution buffer

localcmd (string)
=================
*/
void PF_localcmd(const char *str)
{
	Cbuf_AddText(str);
}

/*
=================
PF_cvar

float cvar (string)
=================
*/
float PF_cvar(const char *str)
{
	return Cvar_VariableValue(str);
}

/*
=================
PF_cvar_set

float cvar (string)
=================
*/
void PF_cvar_set(const char *var, const char *val)
{
	Cvar_Set(var, val);
}

/*
=========
PF_dprint
=========
*/
void PF_dprint(const char *s)
{
	Con_DPrintf("%s", s);
}

char pr_string_temp[128];

int PF_ftos(float v)
{
	if(v == (int)v)
		sprintf(pr_string_temp, "%d", (int)v);
	else
		sprintf(pr_string_temp, "%5.1f", v);
	return /*PR_GetString()*/ pr_string_temp - pr_strings;
}

int PF_vtos(vec3_t vec)
{
	sprintf(pr_string_temp, "'%5.1f %5.1f %5.1f'", vec[0], vec[1], vec[2]);
	return pr_string_temp - pr_strings;
}

#ifdef QUAKE2
int PF_etos(edict_t *ent)
{
	sprintf(pr_string_temp, "entity %i", EDICT_NUM(ent));
	return pr_string_temp - pr_strings;
}
#endif

// entity (entity start, .string field, string match) find = #5;
void PF_Find(edict_t *start, int f, const char *s)
#ifdef QUAKE2
{
	int e;
	char *t;
	edict_t *ed;
	edict_t *first;
	edict_t *second;
	edict_t *last;

	first = second = last = (edict_t *)sv.edicts;
	e = EDICT_NUM(start);
	if(!s)
		Host_Error("PF_Find: bad search string");

	for(e++; e < sv.num_edicts; e++)
	{
		ed = EDICT_NUM(e);
		if(ed->free)
			continue;
		t = E_STRING(ed, f);
		if(!t)
			continue;
		if(!strcmp(t, s))
		{
			if(first == (edict_t *)sv.edicts)
				first = ed;
			else if(second == (edict_t *)sv.edicts)
				second = ed;
			ed->v.chain = EDICT_TO_PROG(last);
			last = ed;
		}
	}

	if(first != last)
	{
		if(last != second)
			first->v.chain = last->v.chain;
		else
			first->v.chain = EDICT_TO_PROG(last);
		last->v.chain = EDICT_TO_PROG((edict_t *)sv.edicts);
		if(second && second != last)
			second->v.chain = EDICT_TO_PROG(last);
	}
	return first;
}
#else
{
	int e;
	int f;
	char *s, *t;
	edict_t *ed;

	e = EDICT_NUM(OFS_PARM0);
	f = G_INT(OFS_PARM1);
	s = G_STRING(OFS_PARM2);
	if(!s)
		Host_Error("PF_Find: bad search string");

	for(e++; e < sv.num_edicts; e++)
	{
		ed = EDICT_NUM(e);
		if(ed->free)
			continue;
		t = E_STRING(ed, f);
		if(!t)
			continue;
		if(!strcmp(t, s))
		{
			return ed;
		}
	}

	return sv.edicts;
}
#endif

void PR_CheckEmptyString(const char *s)
{
	if(s[0] <= ' ')
		Host_Error("Bad string");
}

int PF_precache_file(int file)
{
	// precache_file is only used to copy files with qcc, it does nothing
	return file;
}

int PF_precache_sound(const char *s)
{
	int i;

	if(sv.state != ss_loading)
		Host_Error("PF_Precache_*: Precache can only be done in spawn functions");

	G_INT(OFS_RETURN) = G_INT(OFS_PARM0);
	PR_CheckEmptyString(s);

	for(i = 0; i < MAX_SOUNDS; i++)
	{
		if(!sv.sound_precache[i])
		{
			sv.sound_precache[i] = s;
			return;
		}
		if(!strcmp(sv.sound_precache[i], s))
			return;
	}
	Host_Error("PF_precache_sound: overflow");
}

void PF_precache_model(const char *s)
{
	int i;

	if(sv.state != ss_loading)
		Host_Error("PF_Precache_*: Precache can only be done in spawn functions");

	G_INT(OFS_RETURN) = G_INT(OFS_PARM0);
	PR_CheckEmptyString(s);

	for(i = 0; i < MAX_MODELS; i++)
	{
		if(!sv.model_precache[i])
		{
			sv.model_precache[i] = s;
			sv.models[i] = Mod_ForName(s, true);
			return;
		}
		if(!strcmp(sv.model_precache[i], s))
			return;
	}
	Host_Error("PF_precache_model: overflow");
}

void PF_coredump()
{
	ED_PrintEdicts();
}

void PF_eprint(edict_t *ent)
{
	ED_PrintNum(EDICT_NUM(ent));
}

int PF_rint(float f)
{
	if(f > 0)
		return (int)(f + 0.5);
	else
		return (int)(f - 0.5);
}

/*
=============
PF_pointcontents
=============
*/
float PF_pointcontents(float *v)
{
	return SV_PointContents(v);
}

/*
=============
PF_nextent

entity nextent(entity)
=============
*/
edict_t *PF_nextent(edict_t *ent)
{
	int i;
	edict_t *ent;

	i = EDICT_NUM(ent);
	while(1)
	{
		i++;
		if(i == sv.num_edicts)
		{
			return sv.edicts;
			return;
		}
		ent = EDICT_NUM(i);
		if(!ent->free)
		{
			return ent;
		}
	}
}

/*
=============
PF_aim

Pick a vector for the player to shoot along
vector aim(entity, missilespeed)
=============
*/
void PF_aim(edict_t *ent, float speed)
{
	edict_t *check, *bestent;
	vec3_t start, dir, end, bestdir;
	int i, j;
	trace_t tr;
	float dist, bestdist;

	VectorCopy(ent->v.origin, start);
	start[2] += 20;

	// try sending a trace straight
	VectorCopy(gGlobalVariables.v_forward, dir);
	VectorMA(start, 2048, dir, end);
	tr = SV_Move(start, vec3_origin, vec3_origin, end, false, ent);
	if(tr.ent && tr.ent->v.takedamage == DAMAGE_AIM && (!teamplay.value || ent->v.team <= 0 || ent->v.team != tr.ent->v.team))
	{
		VectorCopy(gGlobalVariables.v_forward, G_VECTOR(OFS_RETURN));
		return;
	}

	// try all possible entities
	VectorCopy(dir, bestdir);
	bestdist = sv_aim.value;
	bestent = nullptr;

	check = NEXT_EDICT(sv.edicts);
	for(i = 1; i < sv.num_edicts; i++, check = NEXT_EDICT(check))
	{
		if(check->v.takedamage != DAMAGE_AIM)
			continue;
		if(check == ent)
			continue;
		if(teamplay.value && ent->v.team > 0 && ent->v.team == check->v.team)
			continue; // don't aim at teammate
		for(j = 0; j < 3; j++)
			end[j] = check->v.origin[j] + 0.5 * (check->v.mins[j] + check->v.maxs[j]);
		VectorSubtract(end, start, dir);
		VectorNormalize(dir);
		dist = DotProduct(dir, gGlobalVariables.v_forward);
		if(dist < bestdist)
			continue; // to far to turn
		tr = SV_Move(start, vec3_origin, vec3_origin, end, false, ent);
		if(tr.ent == check)
		{ // can shoot at this one
			bestdist = dist;
			bestent = check;
		}
	}

	if(bestent)
	{
		VectorSubtract(bestent->v.origin, ent->v.origin, dir);
		dist = DotProduct(dir, gGlobalVariables.v_forward);
		VectorScale(gGlobalVariables.v_forward, dist, end);
		end[2] = dir[2];
		VectorNormalize(end);
		VectorCopy(end, G_VECTOR(OFS_RETURN));
	}
	else
	{
		VectorCopy(bestdir, G_VECTOR(OFS_RETURN));
	}
}

/*
===============================================================================

MESSAGE WRITING

===============================================================================
*/

sizebuf_t *WriteDest(int dest)
{
	int entnum;
	edict_t *ent;

	switch(dest)
	{
	case MSG_BROADCAST:
		return &sv.datagram;

	case MSG_ONE:
		ent = PROG_TO_EDICT(gGlobalVariables.msg_entity);
		entnum = NUM_FOR_EDICT(ent);
		if(entnum < 1 || entnum > svs.maxclients)
			Host_Error("WriteDest: not a client");
		return &svs.clients[entnum - 1].message;

	case MSG_ALL:
		return &sv.reliable_datagram;

	case MSG_INIT:
		return &sv.signon;

	default:
		Host_Error("WriteDest: bad destination");
		break;
	}

	return nullptr;
}

void PF_WriteByte(int val)
{
	MSG_WriteByte(WriteDest(), val);
}

void PF_WriteChar(int val)
{
	MSG_WriteChar(WriteDest(), val);
}

void PF_WriteShort(int val)
{
	MSG_WriteShort(WriteDest(), val);
}

void PF_WriteLong(int val)
{
	MSG_WriteLong(WriteDest(), val);
}

void PF_WriteAngle(float val)
{
	MSG_WriteAngle(WriteDest(), val);
}

void PF_WriteCoord(float val)
{
	MSG_WriteCoord(WriteDest(), val);
}

void PF_WriteString(const char *s)
{
	MSG_WriteString(WriteDest(), s);
}

void PF_WriteEntity(edict_t *ent)
{
	MSG_WriteShort(WriteDest(), EDICT_NUM(ent));
}

//=============================================================================

/*
==============
PF_setspawnparms
==============
*/
void PF_setspawnparms(edict_t *ent)
{
	int i;
	client_t *client;

	i = NUM_FOR_EDICT(ent);
	if(i < 1 || i > svs.maxclients)
		Host_Error("Entity is not a client");

	// copy spawn parms out of the client_t
	client = svs.clients + (i - 1);

	for(i = 0; i < NUM_SPAWN_PARMS; i++)
		(&gGlobalVariables.parm1)[i] = client->spawn_parms[i];
}



#ifdef QUAKE2

float PF_WaterMove(edict_t *self)
{
	int flags;
	int waterlevel;
	int watertype;
	float drownlevel;
	float damage = 0.0;

	//self = PROG_TO_EDICT(gGlobalVariables.self);

	if(self->v.movetype == MOVETYPE_NOCLIP)
	{
		self->v.air_finished = sv.time + 12;
		return damage;
	}

	if(self->v.health < 0)
	{
		return damage;
	}

	if(self->v.deadflag == DEAD_NO)
		drownlevel = 3;
	else
		drownlevel = 1;

	flags = (int)self->v.flags;
	waterlevel = (int)self->v.waterlevel;
	watertype = (int)self->v.watertype;

	if(!(flags & (FL_IMMUNE_WATER + FL_GODMODE)))
		if(((flags & FL_SWIM) && (waterlevel < drownlevel)) || (waterlevel >= drownlevel))
		{
			if(self->v.air_finished < sv.time)
				if(self->v.pain_finished < sv.time)
				{
					self->v.dmg = self->v.dmg + 2;
					if(self->v.dmg > 15)
						self->v.dmg = 10;
					//					T_Damage (self, world, world, self.dmg, 0, FALSE);
					damage = self->v.dmg;
					self->v.pain_finished = sv.time + 1.0;
				}
		}
		else
		{
			if(self->v.air_finished < sv.time)
				//				sound (self, CHAN_VOICE, "player/gasp2.wav", 1, ATTN_NORM);
				SV_StartSound(self, CHAN_VOICE, "player/gasp2.wav", 255, ATTN_NORM);
			else if(self->v.air_finished < sv.time + 9)
				//				sound (self, CHAN_VOICE, "player/gasp1.wav", 1, ATTN_NORM);
				SV_StartSound(self, CHAN_VOICE, "player/gasp1.wav", 255, ATTN_NORM);
			self->v.air_finished = sv.time + 12.0;
			self->v.dmg = 2;
		}

	if(!waterlevel)
	{
		if(flags & FL_INWATER)
		{
			// play leave water sound
			//			sound (self, CHAN_BODY, "misc/outwater.wav", 1, ATTN_NORM);
			SV_StartSound(self, CHAN_BODY, "misc/outwater.wav", 255, ATTN_NORM);
			self->v.flags = (float)(flags & ~FL_INWATER);
		}
		self->v.air_finished = sv.time + 12.0;
		return damage;
	}

	if(watertype == CONTENT_LAVA)
	{ // do damage
		if(!(flags & (FL_IMMUNE_LAVA + FL_GODMODE)))
			if(self->v.dmgtime < sv.time)
			{
				if(self->v.radsuit_finished < sv.time)
					self->v.dmgtime = sv.time + 0.2;
				else
					self->v.dmgtime = sv.time + 1.0;
				//				T_Damage (self, world, world, 10*self.waterlevel, 0, TRUE);
				damage = (float)(10 * waterlevel);
			}
	}
	else if(watertype == CONTENT_SLIME)
	{ // do damage
		if(!(flags & (FL_IMMUNE_SLIME + FL_GODMODE)))
			if(self->v.dmgtime < sv.time && self->v.radsuit_finished < sv.time)
			{
				self->v.dmgtime = sv.time + 1.0;
				//				T_Damage (self, world, world, 4*self.waterlevel, 0, TRUE);
				damage = (float)(4 * waterlevel);
			}
	}

	if(!(flags & FL_INWATER))
	{
		// player enter water sound
		if(watertype == CONTENT_LAVA)
			//			sound (self, CHAN_BODY, "player/inlava.wav", 1, ATTN_NORM);
			SV_StartSound(self, CHAN_BODY, "player/inlava.wav", 255, ATTN_NORM);
		if(watertype == CONTENT_WATER)
			//			sound (self, CHAN_BODY, "player/inh2o.wav", 1, ATTN_NORM);
			SV_StartSound(self, CHAN_BODY, "player/inh2o.wav", 255, ATTN_NORM);
		if(watertype == CONTENT_SLIME)
			//			sound (self, CHAN_BODY, "player/slimbrn2.wav", 1, ATTN_NORM);
			SV_StartSound(self, CHAN_BODY, "player/slimbrn2.wav", 255, ATTN_NORM);

		self->v.flags = (float)(flags | FL_INWATER);
		self->v.dmgtime = 0;
	}

	if(!(flags & FL_WATERJUMP))
	{
		//		self.velocity = self.velocity - 0.8*self.waterlevel*frametime*self.velocity;
		VectorMA(self->v.velocity, -0.8 * self->v.waterlevel * host_frametime, self->v.velocity, self->v.velocity);
	}

	return damage;
}
#endif

enginefuncs_t gEngineFuncs[] =
{
  PF_Fixme,
  PF_makevectors,    // void(entity e)	makevectors 		= #1;
  PF_setorigin,      // void(entity e, vector o) setorigin	= #2;
  PF_setmodel,       // void(entity e, string m) setmodel	= #3;
  PF_setsize,        // void(entity e, vector min, vector max) setsize = #4;
  PF_Fixme,          // void(entity e, vector min, vector max) setabssize = #5;
  PF_break,          // void() break						= #6;
  PF_random,         // float() random						= #7;
  PF_sound,          // void(entity e, float chan, string samp) sound = #8;
  PF_normalize,      // vector(vector v) normalize			= #9;
  PF_error,          // void(string e) error				= #10;
  PF_objerror,       // void(string e) objerror				= #11;
  PF_vlen,           // float(vector v) vlen				= #12;
  PF_vectoyaw,       // float(vector v) vectoyaw		= #13;
  PF_Spawn,          // entity() spawn						= #14;
  PF_Remove,         // void(entity e) remove				= #15;
  PF_traceline,      // float(vector v1, vector v2, float tryents) traceline = #16;
  PF_checkclient,    // entity() clientlist					= #17;
  PF_Find,           // entity(entity start, .string fld, string match) find = #18;
  PF_precache_sound, // void(string s) precache_sound		= #19;
  PF_precache_model, // void(string s) precache_model		= #20;
  PF_stuffcmd,       // void(entity client, string s)stuffcmd = #21;
  PF_findradius,     // entity(vector org, float rad) findradius = #22;
  PF_bprint,         // void(string s) bprint				= #23;
  PF_sprint,         // void(entity client, string s) sprint = #24;
  PF_dprint,         // void(string s) dprint				= #25;
  PF_ftos,           // void(string s) ftos				= #26;
  PF_vtos,           // void(string s) vtos				= #27;
  PF_coredump,
  PF_traceon,
  PF_traceoff,
  PF_eprint,   // void(entity e) debug print an entire entity
  PF_walkmove, // float(float yaw, float dist) walkmove
  PF_Fixme,    // float(float yaw, float dist) walkmove
  PF_droptofloor,
  PF_lightstyle,
  PF_rint,
  PF_floor,
  PF_ceil,
  PF_Fixme,
  PF_checkbottom,
  PF_pointcontents,
  PF_Fixme,
  PF_fabs,
  PF_aim,
  PF_cvar,
  PF_localcmd,
  PF_nextent,
  PF_particle,
  PF_changeyaw,
  PF_Fixme,
  PF_vectoangles,

  PF_WriteByte,
  PF_WriteChar,
  PF_WriteShort,
  PF_WriteLong,
  PF_WriteCoord,
  PF_WriteAngle,
  PF_WriteString,
  PF_WriteEntity,

  PF_changepitch,
  PF_TraceToss,
  PF_etos,
  PF_WaterMove,

  SV_MoveToGoal,
  PF_precache_file,
  PF_makestatic,

  PF_changelevel,
  PF_Fixme,

  PF_cvar_set,
  PF_centerprint,

  PF_ambientsound,

  PF_precache_model,
  PF_precache_sound, // precache_sound2 is different only for qcc
  PF_precache_file,

  PF_setspawnparms
};