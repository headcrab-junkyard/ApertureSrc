/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019 BlackPhrase
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
/// @brief windows mouse code

#include "MouseWin.hpp"

// mouse variables
cvar_t m_filter = { "m_filter", "0" };

static bool mouseshowtoggle = 1;

static unsigned int mstate_di;

void CMouseWin::ClearStates()
{
	if(mouseactive)
	{
		mx_accum = 0;
		my_accum = 0;
		mouse_oldbuttonstate = 0;
	};
};

/*
===========
IN_UpdateClipCursor
===========
*/
void IN_UpdateClipCursor()
{
	if(mouseinitialized && mouseactive && !dinput)
		ClipCursor(&window_rect);
};

/*
===========
IN_ShowMouse
===========
*/
void CMouseWin::Show()
{
	if(!mouseshowtoggle)
	{
		ShowCursor(TRUE);
		mouseshowtoggle = 1;
	};
};

/*
===========
IN_HideMouse
===========
*/
void CMouseWin::Hide()
{
	if(mouseshowtoggle)
	{
		ShowCursor(FALSE);
		mouseshowtoggle = 0;
	};
};

/*
===========
IN_SetQuakeMouseState
===========
*/
void CInputSystem::SetQuakeMouseState()
{
	if(mouseactivatetoggle)
		IN_ActivateMouse();
};

/*
===========
IN_RestoreOriginalMouseState
===========
*/
void CInputSystem::RestoreOriginalMouseState()
{
	if(mouseactivatetoggle)
	{
		IN_DeactivateMouse();
		mouseactivatetoggle = true;
	};

	// try to redraw the cursor so it gets reinitialized, because sometimes it
	// has garbage after the mode switch
	ShowCursor(TRUE);
	ShowCursor(FALSE);
};

/*
===========
IN_MouseEvent
===========
*/
void CInputSystem::MouseEvent(int mstate)
{
	if(mouseactive && !dinput)
	{
		// perform button actions
		for(int i = 0; i < mouse_buttons; i++)
		{
			if((mstate & (1 << i)) && !(mouse_oldbuttonstate & (1 << i)))
				Key_Event(K_MOUSE1 + i, true);

			if(!(mstate & (1 << i)) && (mouse_oldbuttonstate & (1 << i)))
				Key_Event(K_MOUSE1 + i, false);
		};

		mouse_oldbuttonstate = mstate;
	};
};