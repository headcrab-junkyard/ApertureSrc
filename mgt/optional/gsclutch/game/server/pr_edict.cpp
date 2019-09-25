/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2017-2019 BlackPhrase
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
/// @brief entity dictionary

//#include "quakedef.h"

char *pr_strings{nullptr};

/*
=================
ED_ClearEdict

Sets everything to nullptr
=================
*/
void ED_ClearEdict(edict_t *e)
{
	if(e)
	{
		memset(&e->v, 0, sizeof(e->v));
		e->free = false;
	};
};

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
edict_t *ED_Alloc()
{
	if(!sv.edicts)
		Sys_Error("ED_Alloc: No edicts yet");

	int i{0};
	edict_t *e{nullptr};

	for(i = svs.maxclients + 1; i < sv.num_edicts; ++i)
	{
		e = EDICT_NUM(i);
		// the first couple seconds of server time can involve a lot of
		// freeing and allocating, so relax the replacement policy
		if(e->free && (e->freetime < 2 || sv.time - e->freetime > 0.5))
		{
			ED_ClearEdict(e);
			return e;
		};
	};

	if(i == MAX_EDICTS)
		gpSystem->Error("ED_Alloc: no free edicts");

	sv.num_edicts++;
	e = EDICT_NUM(i);
	ED_ClearEdict(e);

	return e;
};

/*
=================
ED_Free

Marks the edict as free
FIXME: walk all entities and nullptr out references to this entity
=================
*/
void ED_Free(edict_t *ed)
{
	SV_UnlinkEdict(ed); // unlink from world bsp

	ed->free = true;
	ed->v.model = 0;
	ed->v.takedamage = 0;
	ed->v.modelindex = 0;
	ed->v.colormap = 0;
	ed->v.skin = 0;
	ed->v.frame = 0;
	VectorCopy(vec3_origin, ed->v.origin);
	VectorCopy(vec3_origin, ed->v.angles);
	ed->v.nextthink = -1;
	ed->v.solid = 0;

	ed->freetime = sv.time;
};

//===========================================================================

/*
============
ED_FindFunction
============
*/
dfunction_t *ED_FindFunction(const char *name)
{
	dfunction_t *func = (dfunction_t)Sys_GetExport_Wrapper(gamedll, name);
	if(func)
		return func;

	return nullptr;
};

// TODO: unused?
/*
=============
ED_Print

For debugging
=============
*/
/*
void ED_Print (edict_t *ed)
{
	int		l;
	ddef_t	*d;
	int		*v;
	int		i, j;
	char	*name;
	int		type;

	if (ed->free)
	{
		gpSystem->Printf ("FREE\n");
		return;
	}

	gpSystem->Printf("\nEDICT %i:\n", NUM_FOR_EDICT(ed));
	for (i=1 ; i<progs->numfielddefs ; i++)
	{
		d = &pr_fielddefs[i];
		name = PR_GetString(d->s_name);
		if (name[strlen(name)-2] == '_')
			continue;	// skip _x, _y, _z vars
			
		v = (int *)((char *)&ed->v + d->ofs*4);

	// if the value is still all 0, skip the field
		type = d->type & ~DEF_SAVEGLOBAL;
		
		for (j=0 ; j<type_size[type] ; j++)
			if (v[j])
				break;
		if (j == type_size[type])
			continue;
	
		gpSystem->Printf ("%s",name);
		l = strlen (name);
		while (l++ < 15)
			gpSystem->Printf (" ");

		gpSystem->Printf ("%s\n", PR_ValueString(d->type, (eval_t *)v));		
	}
}
*/

// TODO: unused?
/*
=============
ED_Write

For savegames
=============
*/
/*
void ED_Write (FILE *f, edict_t *ed)
{
	ddef_t	*d;
	int		*v;
	int		i, j;
	char	*name;
	int		type;

	fprintf (f, "{\n");

	if (ed->free)
	{
		fprintf (f, "}\n");
		return;
	}
	
	for (i=1 ; i<progs->numfielddefs ; i++)
	{
		d = &pr_fielddefs[i];
		name = PR_GetString(d->s_name);
		if (name[strlen(name)-2] == '_')
			continue;	// skip _x, _y, _z vars
			
		v = (int *)((char *)&ed->v + d->ofs*4);

	// if the value is still all 0, skip the field
		type = d->type & ~DEF_SAVEGLOBAL;
		for (j=0 ; j<type_size[type] ; j++)
			if (v[j])
				break;
		if (j == type_size[type])
			continue;
	
		fprintf (f,"\"%s\" ",name);
		fprintf (f,"\"%s\"\n", PR_UglyValueString(d->type, (eval_t *)v));		
	}

	fprintf (f, "}\n");
}
*/

// TODO: unused?
/*
void ED_PrintNum (int ent)
{
	ED_Print (EDICT_NUM(ent));
}
*/

// TODO: unused?
/*
=============
ED_PrintEdicts

For debugging, prints all the entities in the current server
=============
*/
/*
void ED_PrintEdicts ()
{
	int		i;
	
	gpSystem->Printf ("%i entities\n", sv.num_edicts);
	for (i=0 ; i<sv.num_edicts ; i++)
		ED_PrintNum (i);
};
*/

// TODO: unused?
/*
=============
ED_PrintEdict_f

For debugging, prints a single edict
=============
*/
/*
void ED_PrintEdict_f ()
{
	int		i;
	
	i = Q_atoi (Cmd_Argv(1));
	if (i >= sv.num_edicts)
	{
		gpSystem->Printf("Bad edict number\n");
		return;
	}
	ED_PrintNum (i);
}
*/

// TODO: unused?
/*
=============
ED_Count

For debugging
=============
*/
void ED_Count()
{
	edict_t *ent{nullptr};
	int active, models, solid, step;

	active = models = solid = step = 0;
	for(int i = 0; i < sv.num_edicts; ++i)
	{
		ent = EDICT_NUM(i);
		if(ent->free)
			continue;
		active++;
		if(ent->v.solid)
			solid++;
		if(ent->v.model)
			models++;
		if(ent->v.movetype == MOVETYPE_STEP)
			step++;
	};

	gpSystem->Printf("num_edicts:%3i\n", sv.num_edicts);
	gpSystem->Printf("active    :%3i\n", active);
	gpSystem->Printf("view      :%3i\n", models);
	gpSystem->Printf("touch     :%3i\n", solid);
	gpSystem->Printf("step      :%3i\n", step);
};

//============================================================================

/*
====================
ED_ParseEdict

Parses an edict out of the given string, returning the new position
ed should be a properly initialized empty edict.
Used for initial level load and for savegames.
====================
*/
char *ED_ParseEdict(char *data, edict_t *ent)
{
	//ddef_t		*key;
	bool anglehack{false};
	bool init{false};
	char keyname[256]{};
	int n;

	// clear it
	if(ent != sv.edicts) // hack
		Q_memset(&ent->v, 0, sizeof(ent->v));

	SuckOutClassname(data, ent);

	// go through all the dictionary pairs
	while(1)
	{
		// parse key
		data = (char*)COM_Parse(data);
		if(com_token[0] == '}')
			break;
		if(!data)
			gpSystem->Error("ED_ParseEntity: EOF without closing brace");

		// anglehack is to allow QuakeEd to write single scalar angles
		// and allow them to be turned into vectors. (FIXME...)
		if(!Q_strcmp(com_token, "angle"))
		{
			Q_strcpy(com_token, "angles");
			anglehack = true;
		}
		else
			anglehack = false;

		// FIXME: change light to _light to get rid of this hack
		if(!Q_strcmp(com_token, "light"))
			Q_strcpy(com_token, "light_lev"); // hack for single light def

		Q_strcpy(keyname, com_token);

		// another hack to fix keynames with trailing spaces
		n = Q_strlen(keyname);
		while(n && keyname[n - 1] == ' ')
		{
			keyname[n - 1] = 0;
			n--;
		};

		// parse value
		data = (char*)COM_Parse(data);
		if(!data)
			gpSystem->Error("ED_ParseEntity: EOF without closing brace");

		if(com_token[0] == '}')
			gpSystem->Error("ED_ParseEntity: closing brace without data");

		init = true;

		// keynames with a leading underscore are used for utility comments,
		// and are immediately discarded by quake
		if(keyname[0] == '_')
			continue;

		//if (!key)
		{
			//gpSystem->Printf ("'%s' is not a field\n", keyname);
			//continue;
		};

		if(anglehack)
		{
			char temp[32];
			strcpy(temp, com_token);
			sprintf(com_token, "0 %s 0", temp);
		};

		//if (!ED_ParseEpair ((void *)&ent->v, key, com_token))
			//Host_Error ("ED_ParseEdict: parse error");
	};

	if(!init)
		ent->free = true;

	return data;
};

/*
================
ED_LoadFromFile

The entities are directly placed in the array, rather than allocated with
ED_Alloc, because otherwise an error loading the map would have entity
number references out of order.

Creates a server's entity / program execution context by
parsing textual entity definitions out of an ent file.

Used for both fresh maps and savegame loads.  A fresh map would also need
to call ED_CallSpawnFunctions () to let the objects initialize themselves.
================
*/
void ED_LoadFromFile(char *data)
{
	edict_t *ent{nullptr};
	int inhibit{0};
	//dfunction_t	*func;

	gGlobalVariables.time = sv.time;

	// parse ents
	while(1)
	{
		// parse the opening brace
		data = (char*)COM_Parse(data);
		if(!data)
			break;
		if(com_token[0] != '{')
			gpSystem->Error("ED_LoadFromFile: found %s when expecting {", com_token);

		if(!ent)
			ent = EDICT_NUM(0);
		else
			ent = ED_Alloc();
		data = ED_ParseEdict(data, ent);

		// remove things from different skill levels or deathmatch
		if(deathmatch.GetValue())
		{
			if(((int)ent->v.spawnflags & SPAWNFLAG_NOT_DEATHMATCH))
			{
				ED_Free(ent);
				inhibit++;
				continue;
			};
		}
		else if((current_skill == 0 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_EASY)) || (current_skill == 1 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_MEDIUM)) || (current_skill >= 2 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_HARD)))
		{
			ED_Free(ent);
			inhibit++;
			continue;
		};

		//
		// immediately call spawn function
		//
		if(!ent->v.classname)
		{
			gpSystem->Printf("No classname for:\n");
			//ED_Print(ent); // TODO: actually unused in gs, should i fix it?
			ED_Free(ent);
			continue;
		};

		// look for the spawn function
		func = ED_FindFunction ( PR_GetString(ent->v.classname) ); // TODO

		if (!func)
		{
			gpSystem->Printf("No spawn function for:\n");
			//ED_Print(ent); // TODO: actually unused in gs, should i fix it?
			ED_Free(ent);
			continue;
		};

		//gGlobalVariables.self = EDICT_TO_PROG(ent);
		//PR_ExecuteProgram (func - pr_functions); // TODO: actually call the function
	};

	gpSystem->DevPrintf("%i entities inhibited\n", inhibit);
};

edict_t *EDICT_NUM(int n)
{
	if(n < 0 || n >= sv.max_edicts)
		gpSystem->Error("EDICT_NUM: bad number %i", n);
	return (edict_t *)((byte *)sv.edicts + (n) * sizeof(edict_t));
};

int NUM_FOR_EDICT(edict_t *e)
{
	int b = (byte *)e - (byte *)sv.edicts;
	b = b / sizeof(edict_t);

	if(b < 0 || b >= sv.num_edicts)
		gpSystem->Error("NUM_FOR_EDICT: bad pointer");
	return b;
};

void SuckOutClassname(char *data, edict_t *ent)
{
	// go through all the dictionary pairs
	while(1)
	{
		// parse key
		data = COM_Parse(data);
		if(com_token[0] == '}')
			break;
		if(!data)
			gpSystem->Error("ED_ParseEntity: EOF without closing brace");

		// anglehack is to allow QuakeEd to write single scalar angles
		// and allow them to be turned into vectors. (FIXME...)
		if(!Q_strcmp(com_token, "classname"))
		{
			// parse value
			data = COM_Parse(data);
			if(!data)
				gpSystem->Error("ED_ParseEntity: EOF without closing brace");

			if(com_token[0] == '}')
				gpSystem->Error("ED_ParseEntity: closing brace without data");
			
			break;
		};
	};
	
	ent->v.classname = PR_SetString(ED_NewString(com_token));
};

int IndexOfEdict(edict_t *ed)
{
	return 0; // TODO
};