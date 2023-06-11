/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2023 BlackPhrase
 *
 * OGSNext Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGSNext Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGSNext Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"

cvar_t voice_avggain = {"voice_avggain", "0.5"};
cvar_t voice_clientdebug = {"voice_clientdebug", "0"};
cvar_t voice_dsound = {"voice_dsound", "1"};
cvar_t voice_enable = {"voice_enable", "1", FCVAR_ARCHIVE};
cvar_t voice_fadeouttime = {"voice_fadeouttime", "0.1"};
cvar_t voice_forcemicrecord = {"voice_forcemicrecord", "1", FCVAR_ARCHIVE};
cvar_t voice_inputfromfile = {"voice_inputfromfile", "0"};
cvar_t voice_loopback = {"voice_loopback", "0"};
cvar_t voice_maxgain = {"voice_maxgain", "5"};
cvar_t voice_modenable = {"voice_modenable", "0", FCVAR_ARCHIVE};
cvar_t voice_overdrive = {"voice_overdrive", "2"};
cvar_t voice_overdrivefadetime = {"voice_overdrivefadetime", "0.4"};
cvar_t voice_profile = {"voice_profile", "0"};
cvar_t voice_recordtofile = {"voice_recordtofile", "0"};
cvar_t voice_scale = {"voice_scale", "0", FCVAR_ARCHIVE};
cvar_t voice_showchannels = {"voice_showchannels", "0"};
cvar_t voice_showincoming = {"voice_showincoming", "0"};

void CVoice::Init()
{
	// TODO
};

void CVoice::Shutdown()
{
	// TODO
};

void CVoice::RegisterCvars()
{
	// TODO
	Cvar_RegisterVariable(&voice_avggain);
	Cvar_RegisterVariable(&voice_clientdebug);
	Cvar_RegisterVariable(&voice_dsound);
	Cvar_RegisterVariable(&voice_enable);
	Cvar_RegisterVariable(&voice_fadeouttime);
	Cvar_RegisterVariable(&voice_forcemicrecord);
	Cvar_RegisterVariable(&voice_inputfromfile);
	Cvar_RegisterVariable(&voice_loopback);
	Cvar_RegisterVariable(&voice_maxgain);
	Cvar_RegisterVariable(&voice_modenable);
	Cvar_RegisterVariable(&voice_overdrive);
	Cvar_RegisterVariable(&voice_overdrivefadetime);
	Cvar_RegisterVariable(&voice_profile);
	Cvar_RegisterVariable(&voice_recordtofile);
	Cvar_RegisterVariable(&voice_scale);
	Cvar_RegisterVariable(&voice_showchannels);
	Cvar_RegisterVariable(&voice_showincoming);
};