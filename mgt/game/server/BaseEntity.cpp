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

#include "BaseEntity.hpp"

/*
================
CBaseEntity::Save
================
*/
void CBaseEntity::Save(idSaveGame *savefile) const
{
	int i, j;

	savefile->WriteInt(entityNumber);
	savefile->WriteInt(entityDefNumber);

	// spawnNode and activeNode are restored by gameLocal
	savefile->WriteDict(&spawnArgs);
	savefile->WriteString(name);
	scriptObject.Save(savefile);

	savefile->WriteInt(thinkFlags);
	savefile->WriteInt(dormantStart);
	savefile->WriteBool(cinematic);

	savefile->WriteObject(cameraTarget);

	savefile->WriteInt(health);

	savefile->WriteInt(targets.Num());
	for(i = 0; i < targets.Num(); i++)
	{
		targets[i].Save(savefile);
	}

	entityFlags_s flags = fl;
	LittleBitField(&flags, sizeof(flags));
	savefile->Write(&flags, sizeof(flags));

	savefile->WriteInt(timeGroup);
	savefile->WriteBool(noGrab);
	savefile->WriteRenderEntity(xrayEntity);
	savefile->WriteInt(xrayEntityHandle);
	savefile->WriteSkin(xraySkin);

	savefile->WriteRenderEntity(renderEntity);
	savefile->WriteInt(modelDefHandle);
	savefile->WriteRefSound(refSound);

	savefile->WriteObject(bindMaster);
	savefile->WriteJoint(bindJoint);
	savefile->WriteInt(bindBody);
	savefile->WriteObject(teamMaster);
	savefile->WriteObject(teamChain);

	savefile->WriteStaticObject(defaultPhysicsObj);

	savefile->WriteInt(numPVSAreas);
	for(i = 0; i < MAX_PVS_AREAS; i++)
	{
		savefile->WriteInt(PVSAreas[i]);
	}

	if(!signals)
	{
		savefile->WriteBool(false);
	}
	else
	{
		savefile->WriteBool(true);
		for(i = 0; i < NUM_SIGNALS; i++)
		{
			savefile->WriteInt(signals->signal[i].Num());
			for(j = 0; j < signals->signal[i].Num(); j++)
			{
				savefile->WriteInt(signals->signal[i][j].threadnum);
				savefile->WriteString(signals->signal[i][j].function->Name());
			}
		}
	}

	savefile->WriteInt(mpGUIState);
}

/*
================
CBaseEntity::Restore
================
*/
void CBaseEntity::Restore(idRestoreGame *savefile)
{
	int i, j;
	int num;
	idStr funcname;

	savefile->ReadInt(entityNumber);
	savefile->ReadInt(entityDefNumber);

	// spawnNode and activeNode are restored by gameLocal
	savefile->ReadDict(&spawnArgs);
	savefile->ReadString(name);
	SetName(name);

	scriptObject.Restore(savefile);

	savefile->ReadInt(thinkFlags);
	savefile->ReadInt(dormantStart);
	savefile->ReadBool(cinematic);

	savefile->ReadObject(reinterpret_cast<idClass *&>(cameraTarget));

	savefile->ReadInt(health);

	targets.Clear();
	savefile->ReadInt(num);
	targets.SetNum(num);
	for(i = 0; i < num; i++)
	{
		targets[i].Restore(savefile);
	}

	savefile->Read(&fl, sizeof(fl));
	LittleBitField(&fl, sizeof(fl));

	savefile->ReadInt(timeGroup);
	savefile->ReadBool(noGrab);
	savefile->ReadRenderEntity(xrayEntity);
	savefile->ReadInt(xrayEntityHandle);
	if(xrayEntityHandle != -1)
	{
		xrayEntityHandle = gameRenderWorld->AddEntityDef(&xrayEntity);
	}
	savefile->ReadSkin(xraySkin);

	savefile->ReadRenderEntity(renderEntity);
	savefile->ReadInt(modelDefHandle);
	savefile->ReadRefSound(refSound);

	savefile->ReadObject(reinterpret_cast<idClass *&>(bindMaster));
	savefile->ReadJoint(bindJoint);
	savefile->ReadInt(bindBody);
	savefile->ReadObject(reinterpret_cast<idClass *&>(teamMaster));
	savefile->ReadObject(reinterpret_cast<idClass *&>(teamChain));

	savefile->ReadStaticObject(defaultPhysicsObj);
	RestorePhysics(&defaultPhysicsObj);

	savefile->ReadInt(numPVSAreas);
	for(i = 0; i < MAX_PVS_AREAS; i++)
	{
		savefile->ReadInt(PVSAreas[i]);
	}

	bool readsignals;
	savefile->ReadBool(readsignals);
	if(readsignals)
	{
		signals = new(TAG_ENTITY) signalList_t;
		for(i = 0; i < NUM_SIGNALS; i++)
		{
			savefile->ReadInt(num);
			signals->signal[i].SetNum(num);
			for(j = 0; j < num; j++)
			{
				savefile->ReadInt(signals->signal[i][j].threadnum);
				savefile->ReadString(funcname);
				signals->signal[i][j].function = gameLocal.program.FindFunction(funcname);
				if(!signals->signal[i][j].function)
				{
					savefile->Error("Function '%s' not found", funcname.c_str());
				}
			}
		}
	}

	savefile->ReadInt(mpGUIState);

	// restore must retrieve modelDefHandle from the renderer
	if(modelDefHandle != -1)
		modelDefHandle = gameRenderWorld->AddEntityDef(&renderEntity);
};

/*
============
CBaseEntity::Killed

Called whenever an entity's health is reduced to 0 or less.
This is a virtual function that subclasses are expected to implement.
============
*/
void CBaseEntity::Killed(CBaseEntity *inflictor, CBaseEntity *attacker, int damage, const idVec3 &dir, int location)
{
};

/*
================
CBaseEntity::ServerSendEvent

   Saved events are also sent to any client that connects late so all clients
   always receive the events nomatter what time they join the game.
================
*/
void CBaseEntity::ServerSendEvent(int eventId, const idBitMsg *msg, bool saveEvent, lobbyUserID_t excluding) const
{
	CBitMsg outMsg;
	byte msgBuf[MAX_GAME_MESSAGE_SIZE];

	// prevent dupe events caused by frame re-runs
	if(!gameLocal.isNewFrame)
		return;

	outMsg.InitWrite(msgBuf, sizeof(msgBuf));
	outMsg.BeginWriting();
	outMsg.WriteBits(gameLocal.GetSpawnId(this), 32);
	outMsg.WriteByte(eventId);
	outMsg.WriteLong(gameLocal.time);
	
	if(msg)
	{
		outMsg.WriteBits(msg->GetSize(), idMath::BitsForInteger(MAX_EVENT_PARAM_SIZE));
		outMsg.WriteData(msg->GetReadData(), msg->GetSize());
	}
	else
		outMsg.WriteBits(0, idMath::BitsForInteger(MAX_EVENT_PARAM_SIZE));

	idLobbyBase &lobby = session->GetActingGameStateLobbyBase();
	peerMask_t peerMask = MAX_UNSIGNED_TYPE(peerMask_t);
	if(excluding.IsValid())
	{
		peerMask = ~(peerMask_t)lobby.PeerIndexFromLobbyUser(excluding);
	}
	lobby.SendReliable(GAME_RELIABLE_MESSAGE_EVENT, outMsg, false, peerMask);

	if(saveEvent)
	{
		gameLocal.SaveEntityNetworkEvent(this, eventId, msg);
	}
}

/*
================
CBaseEntity::ServerReceiveEvent
================
*/
bool CBaseEntity::ServerReceiveEvent(int event, int time, const idBitMsg &msg)
{
	switch(event)
	{
	case 0:
	{
	}
	default:
	{
		return false;
	}
	}
}

/***********************************************************************

  Script functions
	
***********************************************************************/

/*
================
CBaseEntity::ShouldConstructScriptObjectAtSpawn

Called during CBaseEntity::Spawn to see if it should construct the script object or not.
Overridden by subclasses that need to spawn the script object themselves.
================
*/
bool CBaseEntity::ShouldConstructScriptObjectAtSpawn() const
{
	return true;
}

/*
================
CBaseEntity::ConstructScriptObject

Called during CBaseEntity::Spawn.  Calls the constructor on the script object.
Can be overridden by subclasses when a thread doesn't need to be allocated.
================
*/
idThread *CBaseEntity::ConstructScriptObject()
{
	idThread *thread;
	const function_t *constructor;

	// init the script object's data
	scriptObject.ClearObject();

	// call script object's constructor
	constructor = scriptObject.GetConstructor();
	if(constructor)
	{
		// start a thread that will initialize after Spawn is done being called
		thread = new idThread();
		thread->SetThreadName(name.c_str());
		thread->CallFunction(this, constructor, true);
		thread->DelayedStart(0);
	}
	else
	{
		thread = NULL;
	}

	// clear out the object's memory
	scriptObject.ClearObject();

	return thread;
}

/*
================
CBaseEntity::DeconstructScriptObject

Called during CBaseEntity::~CBaseEntity.  Calls the destructor on the script object.
Can be overridden by subclasses when a thread doesn't need to be allocated.
Not called during idGameLocal::MapShutdown.
================
*/
void CBaseEntity::DeconstructScriptObject()
{
	idThread *thread;
	const function_t *destructor;

	// don't bother calling the script object's destructor on map shutdown
	if(gameLocal.GameState() == GAMESTATE_SHUTDOWN)
	{
		return;
	}

	// call script object's destructor
	destructor = scriptObject.GetDestructor();
	if(destructor)
	{
		// start a thread that will run immediately and be destroyed
		thread = new idThread();
		thread->SetThreadName(name.c_str());
		thread->CallFunction(this, destructor, true);
		thread->Execute();
		delete thread;
	}
}

/*
============
T_Damage

The damage is coming from inflictor, but get mad at attacker
This should be the only function that ever reduces health.
============
*/
void CBaseEntity::TakeDamage(entity targ, entity inflictor, entity attacker, float damage)
{
	vector dir;
	entity oldself;
	float save;
	float take;
	string s;
	string attackerteam, targteam;

	if(!targ->v.takedamage)
		return;

	// used by buttons and triggers to set activator for target firing
	damage_attacker = attacker;

	// check for quad damage powerup on the attacker
	if(attacker->v.super_damage_finished > time && inflictor->v.classname != "door")
		if(deathmatch == 4)
			damage = damage * 8;
		else
			damage = damage * 4;

	// save damage based on the target's armor level

	save = ceil(targ->v.armortype * damage);
	if(save >= targ->v.armorvalue)
	{
		save = targ->v.armorvalue;
		targ->v.armortype = 0; // lost all armor
		targ->v.items = targ->v.items - (targ->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3));
	}

	targ->v.armorvalue = targ->v.armorvalue - save;
	take = ceil(damage - save);

	// add to the damage total for clients, which will be sent as a single
	// message at the end of the frame
	// FIXME: remove after combining shotgun blasts?
	if(targ->v.flags & FL_CLIENT)
	{
		targ->v.dmg_take = targ->v.dmg_take + take;
		targ->v.dmg_save = targ->v.dmg_save + save;
		targ->v.dmg_inflictor = inflictor;
	}

	damage_inflictor = inflictor;

	// figure momentum add
	if((inflictor != world) && (targ->v.movetype == MOVETYPE_WALK))
	{
		dir = targ->v.origin - (inflictor->v.absmin + inflictor->v.absmax) * 0.5;
		dir = normalize(dir);
		// Set kickback for smaller weapons
		//Zoid -- use normal NQ kickback
		//		// Read: only if it's not yourself doing the damage
		//		if ( (damage < 60) & ((attacker->v.classname == "player") & (targ->v.classname == "player")) & ( attacker->v.netname != targ->v.netname))
		//			targ->v.velocity = targ->v.velocity + dir * damage * 11;
		//		else
		// Otherwise, these rules apply to rockets and grenades
		// for blast velocity
		targ->v.velocity = targ->v.velocity + dir * damage * 8;

		// Rocket Jump modifiers
		if((rj > 1) & ((attacker->v.classname == "player") & (targ->v.classname == "player")) & (attacker->v.netname == targ->v.netname))
			targ->v.velocity = targ->v.velocity + dir * damage * rj;
	}

	// check for godmode or invincibility
	if(targ->v.flags & FL_GODMODE)
		return;
	if(targ->v.invincible_finished >= time)
	{
		if(self->v.invincible_sound < time)
		{
			sound(targ, CHAN_ITEM, "items/protect3.wav", 1, ATTN_NORM);
			self->v.invincible_sound = time + 2;
		}
		return;
	}

	// team play damage avoidance
	//ZOID 12-13-96: self.team doesn't work in QW.  Use keys
	attackerteam = infokey(attacker, "team");
	targteam = infokey(targ, "team");

	if((teamplay == 1) && (targteam == attackerteam) &&
	   (attacker->v.classname == "player") && (attackerteam != "") &&
	   inflictor->v.classname != "door")
		return;

	if((teamplay == 3) && (targteam == attackerteam) &&
	   (attacker->v.classname == "player") && (attackerteam != "") &&
	   (targ != attacker) && inflictor->v.classname != "door")
		return;

	// do the damage
	targ->v.health = targ->v.health - take;

	if(targ->v.health <= 0)
	{
		Killed(targ, attacker);
		return;
	}

	// react to the damage
	oldself = self;
	self = targ;

	/*SERVER
	if ( (self.flags & FL_MONSTER) && attacker != world)
	{
	// get mad unless of the same class (except for soldiers)
		if (self != attacker && attacker != self.enemy)
		{
			if ( (self.classname != attacker.classname) 
			|| (self.classname == "monster_army" ) )
			{
				if (self.enemy.classname == "player")
					self.oldenemy = self.enemy;
				self.enemy = attacker;
				FoundTarget ();
			}
		}
	}
*/
	if(self->v.th_pain)
		self->v.th_pain(attacker, take);

	self = oldself;
};

/*
============
Killed
============
*/
void CBaseEntity::Killed(CBaseEntity *inflictor, CBaseEntity *attacker int damage, vec3_t point)
{
	if(self->GetHealth() < -999)
		self->SetHealth(-999); // don't let sbar look bad if a player

	// TODO: not present in q2
	if(self->GetMoveType() == MOVETYPE_PUSH || self->GetMoveType() == MOVETYPE_NONE)
	{
		// doors, triggers, etc
		self->v.th_die();
		return;
	};
	//

	self->SetEnemy(attacker);

	// bump the monster counter
	if(self->v.flags & FL_MONSTER)
	{
		killed_monsters++;
		WriteByte(MSG_ALL, SVC_KILLEDMONSTER);
	};

	ClientObituary(self, attacker);

	self->v.takedamage = DAMAGE_NO;
	self->SetTouchCallback(SUB_Null);
	self->SetEffects(0);

	/*SERVER
	monster_death_use();
*/
	self->v.th_die();
};

/*
============
CanDamage

Returns true if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
bool CanDamage(entity targ, entity inflictor)
{
	// bmodels need special checking because their origin is 0,0,0
	if(targ.movetype == MOVETYPE_PUSH)
	{
		traceline(inflictor.origin, 0.5 * (targ.absmin + targ.absmax), TRUE, self);
		if(trace_fraction == 1)
			return true;
		if(trace_ent == targ)
			return true;
		return false;
	}

	traceline(inflictor.origin, targ.origin, TRUE, self);
	if(trace_fraction == 1)
		return true;
	traceline(inflictor.origin, targ.origin + '15 15 0', TRUE, self);
	if(trace_fraction == 1)
		return true;
	traceline(inflictor.origin, targ.origin + '-15 -15 0', TRUE, self);
	if(trace_fraction == 1)
		return true;
	traceline(inflictor.origin, targ.origin + '-15 15 0', TRUE, self);
	if(trace_fraction == 1)
		return true;
	traceline(inflictor.origin, targ.origin + '15 -15 0', TRUE, self);
	if(trace_fraction == 1)
		return true;

	return false;
};

void CBaseEntity::SetModel(const char *name)
{
	gpEngine->pfnSetModel(self, name);
};

void CBaseEntity::SetSize(float *mins, float *maxs)
{
	gpEngine->pfnSetSize(self, mins, maxs);
};

/*
============
Killed
============
*/
// q2
void CBaseEntity::Killed(edict_t *targ, edict_t *inflictor, edict_t *attacker, int damage, vec3_t point)
{

	if((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		//		targ->svflags |= SVF_DEADMONSTER;	// now treat as a different content type
		if(!(targ->monsterinfo.aiflags & AI_GOOD_GUY))
		{
			level.killed_monsters++;
			if(coop->value && attacker->client)
				attacker->client->resp.score++;
			// medics won't heal monsters that they kill themselves
			if(strcmp(attacker->classname, "monster_medic") == 0)
				targ->owner = attacker;
		};
	};

	if(targ->movetype == MOVETYPE_PUSH || targ->movetype == MOVETYPE_STOP || targ->movetype == MOVETYPE_NONE)
	{ // doors, triggers, etc
		targ->die(targ, inflictor, attacker, damage, point);
		return;
	};

	if((targ->svflags & SVF_MONSTER) && (targ->deadflag != DEAD_DEAD))
	{
		targ->touch = NULL;
		monster_death_use(targ);
	};

	targ->die(targ, inflictor, attacker, damage, point);
};