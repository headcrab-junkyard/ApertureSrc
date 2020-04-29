/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018, 2020 BlackPhrase
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
/// @brief client sound i/o functions

#pragma once

#define CVOXFILESENTENCEMAX 1536

extern cvar_t bgmvolume;
//extern cvar_t volume;

typedef struct sfx_s sfx_t;

void S_Init();
void S_Shutdown();

void S_Update(vec3_t origin, vec3_t v_forward, vec3_t v_right, vec3_t v_up);
void S_ExtraUpdate();

void S_ClearBuffer();

void S_BeginPrecaching();
void S_EndPrecaching();

sfx_t *S_PrecacheSound(const char *sample);

void S_TouchSound(const char *sample);

void S_LocalSound(const char *sample);

void S_StartDynamicSound(int entnum, int entchannel, sfx_t *sfx, vec3_t origin, float fvol, float attenuation);
void S_StartStaticSound(sfx_t *sfx, vec3_t origin, float vol, float attenuation);

void S_StopSound(int entnum, int entchannel);
void S_StopAllSounds(bool clear);