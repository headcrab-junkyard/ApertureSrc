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

#include "quakedef.h"
#include "Engine.hpp"

// TODO: temp
#include "engineclient/IEngineClient.hpp"
extern IEngineClient *gpEngineClient;

//static CEngine gEngine; // g_Engine
//IEngine *gpEngine = &gEngine; // eng

EXPOSE_SINGLE_INTERFACE(CEngine, IEngine, MGT_ENGINE_INTERFACE_VERSION);

CEngine::CEngine() = default;
CEngine::~CEngine() = default;

//bool CEngine::Init(bool dedicated, const char *basedir, const char *cmdline)
bool CEngine::Init(const SInitData &apInitParams)
{
	//static quakeparms_t parms; // TODO: static?

	//memset(&parms, 0, sizeof(parms));

	//mParms. =;

	//parms.memsize = 8*1024*1024; // TODO: 5861376 in QW
	//parms.membase = malloc (parms.memsize);
	//parms.basedir = ".";

	//COM_InitArgv (argc, argv); // TODO: parms.argc, parms.argv

	//parms.argc = com_argc;
	//parms.argv = com_argv;

	//isDedicated = (COM_CheckParm ("-dedicated") != 0);

	//Sys_InitGame(char *, char *, void *, int); // TODO

	//printf("Host_Init\n");
	Host_Init(&mParms);
	return true;
};

void CEngine::Shutdown()
{
	Host_Shutdown();
};

bool CEngine::Frame()
{
	if(!isDedicated) // TODO: if gpEngineClient is nullptr then we're in dedicated mode (right???), no need to check for isDedicated
		if(gpEngineClient)
			if(!gpEngineClient->PreFrame())
				return true; // TODO: returning true here because false will cause it to shutdown
	
	newtime = Sys_FloatTime();
	frametime = newtime - oldtime;
	
	if(isDedicated)
	{
		while(frametime < sys_ticrate.value)
		{
			Sys_Sleep(); // TODO: 1ms?
			newtime = Sys_FloatTime();
			frametime = newtime - oldtime;
		};
	};
	
	Host_Frame(frametime);
	oldtime = newtime;

	return true;
};