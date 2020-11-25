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

	memset(&mParms, 0, sizeof(mParms));

#if defined(GLQUAKE) || defined(SWDS)
	mParms.memsize = 16 * 1024 * 1024;
#else
	mParms.memsize = 8 * 1024 * 1024; // TODO: 5861376 in QW
#endif
	
	//COM_InitArgv(argc, argv); // TODO: parms.argc, parms.argv
	
	/*
	if(COM_CheckParm("-heapsize"))
	{
		int t = COM_CheckParm("-heapsize") + 1;

		if(t < com_argc)
			parms.memsize = Q_atoi(com_argv[t]) * 1024;
	};
	*/
	
	mParms.membase = malloc(mParms.memsize);
	
	if(!mParms.membase)
		mpSystem->Error("Not enough memory free; check disk space\n");
	
	// TODO: windows
	/*
	static char cwd[1024]{};

	_getcwd(cwd, sizeof(cwd));
	
	if(cwd[Q_strlen(cwd) - 1] == '\\')
		cwd[Q_strlen(cwd) - 1] = 0;
	*/
	
	mParms.basedir = (char*)apInitParams.sGameDir; //"."; // TODO: cwd

	/*
	// dedicated server ONLY!
	if(!COM_CheckParm("-dedicated"))
	{
		memcpy(newargv, argv, argc * 4);
		newargv[argc] = "-dedicated";
		argc++;
		argv = newargv;
		COM_InitArgv(argc, argv);
	}
	*/
	
	//mParms.argc = com_argc;
	//mParms.argv = com_argv;

	//isDedicated = (COM_CheckParm ("-dedicated") != 0);
	isDedicated = apInitParams.bDedicated;
	
	//Sys_InitGame(char *, char *, void *, int); // TODO

	//Sys_PageIn(parms.membase, parms.memsize); // TODO: windows
	
	//Sys_Init(); // TODO: windows
	
	FileSystem_Init(/*apInitParams.sGameDir*/mParms.basedir, (void*)apInitParams.fnLauncherFactory);

	Q_strcpy(com_gamedir, apInitParams.sGameDir);
	
	// because sound is off until we become active
	//S_BlockSound(); // TODO: windows
	
	//printf("Host_Init\n"); // TODO: Sys_Printf
	Host_Init(&mParms);
	
	oldtime = Sys_FloatTime(); // TODO: ?
	
	return true;
};

void CEngine::Shutdown()
{
	Host_Shutdown();
	
	FileSystem_Shutdown();
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
		// TODO: can't get outta this loop
		/*
		while(frametime < sys_ticrate.GetValue())
		{
			Sys_Sleep(); // TODO: 1ms
			// TODO
			newtime = Sys_FloatTime(); // TODO: ???
			frametime = newtime - oldtime; // TODO: ???
			//
			//continue; // TODO
			//
		};
		*/
	};
	
	Host_Frame(frametime);
	oldtime = newtime;

	return true;
};