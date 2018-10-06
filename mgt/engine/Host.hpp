/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2017-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief host subsystem header

#pragma once

//#include <memory>
//#include "core/CommonTypes.hpp"

//=============================================================================

// the host system specifies the base of the directory tree, the
// command line parms passed to the program, and the amount of memory
// available for the program to use

typedef struct quakeparms_s
{
	char *basedir{nullptr};
	char *cachedir{nullptr}; ///< for development over ISDN lines
	
	int argc{0};
	char **argv{nullptr};
	
	void *membase{nullptr};
	int memsize{0};
} quakeparms_t;

extern quakeparms_t host_parms;

extern CConVar hostname; // TODO: wrong place?
extern CConVar sys_ticrate;
extern CConVar sys_nostdout;
extern CConVar developer;

extern bool host_initialized; // true if into command execution
extern double host_frametime;
//extern byte *host_basepal;
//extern byte *host_colormap;
extern int host_framecount; // incremented every frame, never reset
extern double realtime;     // not bounded in any way, changed at
                            // start of every frame, never reset

void Host_Quit_f();

void Host_ClearMemory();

void Host_InitCommands();

void Host_Init(quakeparms_t *parms);
void Host_Shutdown();

void Host_Error(const char *error, ...);
void Host_EndGame(const char *message, ...);

void Host_Frame(float time); // TODO: state, stateinfo

void Host_ClientCommands(client_t *host_client, const char *fmt, ...);
void Host_ShutdownServer(bool crash);
//void Host_WriteConfiguration ();

class CHost final : public IHost
{
public:
	CHost();
	~CHost();
	
	void Init(quakeparms_t *parms);
	void Shutdown();
	
	void Frame(float time);
	
	void EndGame(const char *message, ...) override;
	
	void Error(const char *message, ...) override;
	
	void ClearMemory() override;
	
	void ServerFrame();
	
	void ShutdownServer(bool crash) override;
	
	bool IsServerActive() const override;
private:
	void InitLocal();
	void InitCommands();
};

extern bool msg_suppress_1; // suppresses resolution and cache size console output
                            //  an fullscreen DIB focus gain/loss

extern int current_skill;   // skill level for currently loaded level (in case
                            //  the user changes the cvar while the level is
                            //  running, this reflects the level actually in use)

extern bool isDedicated;

extern int minimum_memory;