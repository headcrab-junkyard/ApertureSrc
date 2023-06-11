/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2020, 2023 BlackPhrase
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

#pragma once

extern byte gammatable[256]; ///< Palette is sent through this
extern byte ramps[3][256];
extern float v_blend[4];

extern CConVar v_gamma;
extern CConVar lcd_x;

class CView
{
public:
	void Init();
	
	void RenderView();
	
	void UpdatePalette();
	
	float CalcRoll(vec3_t angles, vec3_t velocity);
};