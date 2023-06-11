/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2020-2021 BlackPhrase
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

#include "Screen.hpp"

/*
==================
SCR_Init
==================
*/
void CScreen::Init()
{
	SCR_Init();
	
	initialized = true;
};

/*
==================
SCR_UpdateScreen

This is called every frame, and can also be called explicitly to flush
text to the screen.

WARNING: be very careful calling this from elsewhere, because the refresh
needs almost the entire 256k of stack space!
==================
*/
void CScreen::Update()
{
	// not initialized yet
	if(!initialized)
	// TODO: if (!scr_initialized || !con_initialized)
		return;
	
	if(disabled_for_loading)
		return;
	
	mpRenderer->BeginFrame();
	
	DrawScreenField(STEREO_CENTER);
	
	mpRenderer->EndFrame(nullptr, nullptr);
};

void CScreen::SetActive(bool abState)
{
	disabled_for_loading = abState;
};

void CScreen::SizeUp()
{
	SCR_SizeUp();
};

void CScreen::SizeDown()
{
	SCR_SizeDown();
};

void CScreen::BringDownConsole()
{
	//SCR_BringDownConsole(); // TODO
};

void CScreen::CenterPrint(const char *str)
{
	SCR_CenterPrint(str);
};

void CScreen::BeginLoadingPlaque()
{
	SCR_BeginLoadingPlaque();
};

void CScreen::EndLoadingPlaque()
{
	SCR_EndLoadingPlaque();
};

int CScreen::ModalMessage(const char *text)
{
	return 0; // SCR_ModalMessage(text); // TODO
};

#ifdef _WIN32
void CScreen::UpdateWholeScreen()
{
	//SCR_UpdateWholeScreen(); // TODO
};
#endif

void CScreen::DrawScreenField()
{
	//
	// determine size of refresh window
	//
	if (oldfov != scr_fov.value)
	{
		oldfov = scr_fov.value;
		vid.recalc_refdef = true;
	}

	if (oldscreensize != scr_viewsize.value)
	{
		oldscreensize = scr_viewsize.value;
		vid.recalc_refdef = true;
	}

	if (vid.recalc_refdef)
		SCR_CalcRefdef ();

//
// do 3D refresh drawing, and then update the screen
//
	SCR_SetUpToDrawConsole ();
	
	V_RenderView ();

	GL_Set2D ();

	//
	// draw any areas not covered by the refresh
	//
	SCR_TileClear ();

	if (scr_drawdialog)
	{
		Sbar_Draw ();
		mpDrawer->FadeScreen();
		SCR_DrawNotifyString ();
		scr_copyeverything = true;
	}
	else if (scr_drawloading)
	{
		SCR_DrawLoading();
		Sbar_Draw ();
	}
	else if (cl.intermission == 1 && key_dest == key_game)
	{
		Sbar_IntermissionOverlay ();
	}
	else if (cl.intermission == 2 && key_dest == key_game)
	{
		Sbar_FinaleOverlay ();
		SCR_CheckDrawCenterString ();
	}
	else
	{
		// TODO: gl only
		if (crosshair.value)
			mpDrawer->DrawChar(scr_vrect.x + scr_vrect.width/2, scr_vrect.y + scr_vrect.height/2, '+');
		//
		
		SCR_DrawRam ();
		SCR_DrawNet ();
		SCR_DrawTurtle ();
		SCR_DrawPause ();
		SCR_CheckDrawCenterString ();
		Sbar_Draw ();
		SCR_DrawConsole ();	
		M_Draw ();
	};

	V_UpdatePalette ();
};