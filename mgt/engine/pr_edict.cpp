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

#include "quakedef.h"
#include "game/server/IGame.hpp"
//#include "Progs.hpp"

globalvars_t gGlobalVariables{};

//cvar_t nomonsters = {"nomonsters", "0"}; // TODO: unused?

// TODO: unused?
/*
============
PR_ValueString

Returns a string describing *data in a type specific manner
=============
*/
/*
char *PR_ValueString (etype_t type, eval_t *val)
{
	static char	line[256];
	ddef_t		*def;
	dfunction_t	*f;
	
	type &= ~DEF_SAVEGLOBAL;

	switch (type)
	{
	case ev_string:
		sprintf (line, "%s", PR_GetString(val->string));
		break;
	case ev_entity:	
		sprintf (line, "entity %i", NUM_FOR_EDICT(PROG_TO_EDICT(val->edict)) );
		break;
	case ev_function:
		f = pr_functions + val->function;
		sprintf (line, "%s()", PR_GetString(f->s_name));
		break;
	case ev_field:
		def = ED_FieldAtOfs ( val->_int );
		sprintf (line, ".%s", PR_GetString(def->s_name));
		break;
	case ev_void:
		sprintf (line, "void");
		break;
	case ev_float:
		sprintf (line, "%5.1f", val->_float);
		break;
	case ev_vector:
		sprintf (line, "'%5.1f %5.1f %5.1f'", val->vector[0], val->vector[1], val->vector[2]);
		break;
	case ev_pointer:
		sprintf (line, "pointer");
		break;
	default:
		sprintf (line, "bad type %i", type);
		break;
	}
	
	return line;
}
*/

// TODO: unused?
/*
============
PR_UglyValueString

Returns a string describing *data in a type specific manner
Easier to parse than PR_ValueString
=============
*/
/*
char *PR_UglyValueString (etype_t type, eval_t *val)
{
	static char	line[256];
	ddef_t		*def;
	dfunction_t	*f;
	
	type &= ~DEF_SAVEGLOBAL;

	switch (type)
	{
	case ev_string:
		sprintf (line, "%s", PR_GetString(val->string));
		break;
	case ev_entity:	
		sprintf (line, "%i", NUM_FOR_EDICT(PROG_TO_EDICT(val->edict)));
		break;
	case ev_function:
		f = pr_functions + val->function;
		sprintf (line, "%s", PR_GetString(f->s_name));
		break;
	case ev_field:
		def = ED_FieldAtOfs ( val->_int );
		sprintf (line, "%s", PR_GetString(def->s_name));
		break;
	case ev_void:
		sprintf (line, "void");
		break;
	case ev_float:
		sprintf (line, "%f", val->_float);
		break;
	case ev_vector:
		sprintf (line, "%f %f %f", val->vector[0], val->vector[1], val->vector[2]);
		break;
	default:
		sprintf (line, "bad type %i", type);
		break;
	}
	
	return line;
}
*/

/*
==============================================================================

					ARCHIVING GLOBALS

FIXME: need to tag constants, doesn't really work
==============================================================================
*/

// TODO: unused?
/*
=============
ED_WriteGlobals
=============
*/
/*
void ED_WriteGlobals (FILE *f)
{
	ddef_t		*def;
	int			i;
	char		*name;
	int			type;

	fprintf (f,"{\n");
	for (i=0 ; i<progs->numglobaldefs ; i++)
	{
		def = &pr_globaldefs[i];
		type = def->type;
		if ( !(def->type & DEF_SAVEGLOBAL) )
			continue;
		type &= ~DEF_SAVEGLOBAL;

		if (type != ev_string
		&& type != ev_float
		&& type != ev_entity)
			continue;

		name = PR_GetString(def->s_name);
		fprintf (f,"\"%s\" ", name);
		fprintf (f,"\"%s\"\n", PR_UglyValueString(type, (eval_t *)&pr_globals[def->ofs]));		
	}
	fprintf (f,"}\n");
}
*/

// TODO: unused?
/*
=============
ED_ParseGlobals
=============
*/
/*
void ED_ParseGlobals (char *data)
{
	char	keyname[64];
	ddef_t	*key;

	while (1)
	{	
	// parse key
		data = (char*)COM_Parse (data);
		if (com_token[0] == '}')
			break;
		if (!data)
			Sys_Error ("ED_ParseEntity: EOF without closing brace");

		strcpy (keyname, com_token);

	// parse value	
		data = (char*)COM_Parse (data);
		if (!data)
			Sys_Error ("ED_ParseEntity: EOF without closing brace");

		if (com_token[0] == '}')
			Sys_Error ("ED_ParseEntity: closing brace without data");

		if (!key)
		{
			gpSystem->Printf ("'%s' is not a global\n", keyname);
			continue;
		};

		//if (!ED_ParseEpair ((void *)pr_globals, key, com_token))
			//Host_Error ("ED_ParseGlobals: parse error");
	};
};
*/

//============================================================================

/*
=============
ED_NewString
=============
*/
char *ED_NewString(char *string)
{
	char *pnew, *new_p;
	int i, l;

	l = Q_strlen(string) + 1;
	pnew = (char*)Hunk_Alloc(l);
	new_p = pnew;

	for(i = 0; i < l; ++i)
	{
		if(string[i] == '\\' && i < l - 1)
		{
			i++;
			if(string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	};

	return pnew;
};

//typedef void (*pfnGiveFnptrsToDll)(enginefuncs_t *apEngFuncs, globalvars_t *apGlobals);

void *gpGameLib{nullptr};
IGame *gpGame{nullptr};

/*
===============
PR_LoadProgs
===============
*/
void PR_LoadProgs() // our temporary LoadEntityDLLs
{
	gpGameLib = Sys_LoadModule(va("%s/bin/server", com_gamedir));

	if(!gpGameLib)
		gpSystem->Error("PR_LoadProgs: couldn't load game dll!");

	//pr_strings = (char *)progs + progs->ofs_strings; // TODO

	auto fnGameFactory{Sys_GetFactory(gpGameLib)};
	
	if(!fnGameFactory)
		return;
	
	gpGame = (IGame*)fnGameFactory(MGT_GAME_INTERFACE_VERSION, nullptr);
	
	if(!gpGame)
		return;
	
	//if(nDLLVersion != INTERFACE_VERSION)
		//Sys_Error("game dll has wrong version number (%i should be %i)", nDLLVersion, INTERFACE_VERSION);
	
	//fnGiveFnptrsToDll = (pfnGiveFnptrsToDll)Sys_GetExport(gamedll, "GiveFnptrsToDll");
	//fnGiveFnptrsToDll(&gEngFuncs, &gGlobalVariables); // TODO
};

/*
===============
PR_Init
===============
*/
//void CProgs::Init() // TODO
void PR_Init()
{
	//Cmd_AddCommand("edict", ED_PrintEdict_f);
	//Cmd_AddCommand("edicts", ED_PrintEdicts);
	//Cmd_AddCommand("edictcount", ED_Count);
	//mpCmd->Add("profile", PR_Profile_f);
	
	//mpCvar->Register(&nomonsters);
};