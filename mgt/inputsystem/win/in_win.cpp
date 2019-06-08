/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
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
/// @brief windows mouse and joystick code

// 02/21/97 JCB Added extended DirectInput code to support external controllers.

#include <dinput.h>
#include "quakedef.h"
#include "winquake.h"

/*
===========
Force_CenterView_f
===========
*/
void Force_CenterView_f()
{
	cl.viewangles[PITCH] = 0;
}

/*
===========
IN_Init
===========
*/
void CInputSystem::Init()
{
	// mouse variables
	Cvar_RegisterVariable(&m_filter);

	// joystick variables
	Cvar_RegisterVariable(&in_joystick);
	Cvar_RegisterVariable(&joy_name);
	Cvar_RegisterVariable(&joy_advanced);
	Cvar_RegisterVariable(&joy_advaxisx);
	Cvar_RegisterVariable(&joy_advaxisy);
	Cvar_RegisterVariable(&joy_advaxisz);
	Cvar_RegisterVariable(&joy_advaxisr);
	Cvar_RegisterVariable(&joy_advaxisu);
	Cvar_RegisterVariable(&joy_advaxisv);
	Cvar_RegisterVariable(&joy_forwardthreshold);
	Cvar_RegisterVariable(&joy_sidethreshold);
	Cvar_RegisterVariable(&joy_pitchthreshold);
	Cvar_RegisterVariable(&joy_yawthreshold);
	Cvar_RegisterVariable(&joy_forwardsensitivity);
	Cvar_RegisterVariable(&joy_sidesensitivity);
	Cvar_RegisterVariable(&joy_pitchsensitivity);
	Cvar_RegisterVariable(&joy_yawsensitivity);
	Cvar_RegisterVariable(&joy_wwhack1);
	Cvar_RegisterVariable(&joy_wwhack2);

	Cmd_AddCommand("force_centerview", Force_CenterView_f);
	Cmd_AddCommand("joyadvancedupdate", Joy_AdvancedUpdate_f);
}

/*
===========
IN_MouseMove
===========
*/
void IN_MouseMove(usercmd_t *cmd)
{
	if(!mouseactive)
		return;

	if(dinput)
	{
		int i;
		DIDEVICEOBJECTDATA od;
		DWORD dwElements;
		HRESULT hr;
	
		mx = 0;
		my = 0;

		for(;;)
		{
			dwElements = 1;

			hr = IDirectInputDevice_GetDeviceData(g_pMouse,
			                                      sizeof(DIDEVICEOBJECTDATA), &od, &dwElements, 0);

			if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
			{
				dinput_acquired = true;
				IDirectInputDevice_Acquire(g_pMouse);
				break;
			}

			/* Unable to read data or no data available */
			if(FAILED(hr) || dwElements == 0)
			{
				break;
			}

			/* Look at the element to see what happened */

			switch(od.dwOfs)
			{
			case DIMOFS_X:
				mx += od.dwData;
				break;

			case DIMOFS_Y:
				my += od.dwData;
				break;

			case DIMOFS_BUTTON0:
				if(od.dwData & 0x80)
					mstate_di |= 1;
				else
					mstate_di &= ~1;
				break;

			case DIMOFS_BUTTON1:
				if(od.dwData & 0x80)
					mstate_di |= (1 << 1);
				else
					mstate_di &= ~(1 << 1);
				break;

			case DIMOFS_BUTTON2:
				if(od.dwData & 0x80)
					mstate_di |= (1 << 2);
				else
					mstate_di &= ~(1 << 2);
				break;
			}
		}

		// perform button actions
		for(i = 0; i < mouse_buttons; i++)
		{
			if((mstate_di & (1 << i)) &&
			   !(mouse_oldbuttonstate & (1 << i)))
			{
				Key_Event(K_MOUSE1 + i, true);
			}

			if(!(mstate_di & (1 << i)) &&
			   (mouse_oldbuttonstate & (1 << i)))
			{
				Key_Event(K_MOUSE1 + i, false);
			}
		}

		mouse_oldbuttonstate = mstate_di;
	}
	else
	{
	};
}

/*
===========
IN_Move
===========
*/
void IN_Move(usercmd_t *cmd)
{
	if(ActiveApp && !Minimized)
		gpClientGame->IN_Move(cmd);
};

/*
===========
IN_Accumulate
===========
*/
void IN_Accumulate()
{
	int mx, my;
	HDC hdc;

	if(mouseactive)
	{
		if(!dinput)
		{
			GetCursorPos(&current_pos);

			mx_accum += current_pos.x - window_center_x;
			my_accum += current_pos.y - window_center_y;

			// force the mouse to the center, so there's room to move
			SetCursorPos(window_center_x, window_center_y);
		}
	}
}

/*
===================
IN_ClearStates
===================
*/
void IN_ClearStates()
{
	CMouseWin::ClearStates();
}

/*
===========
IN_JoyMove
===========
*/
void IN_JoyMove(usercmd_t *cmd)
{
	
	
	

	// complete initialization if first time in
	// this is needed as cvars are not available at initialization time
	if(joy_advancedinit != true)
	{
		Joy_AdvancedUpdate_f();
		joy_advancedinit = true;
	}

	// verify joystick is available and that the user wants to use it
	if(!joy_avail || !in_joystick.value)
	{
		return;
	}

	// collect the joystick data, if possible
	if(IN_ReadJoystick() != true)
	{
		return;
	}
}