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
/// @brief windows joystick code

#include "JoystickWin.hpp"

// joystick defines and variables
// where should defines be moved?
#define JOY_ABSOLUTE_AXIS 0x00000000 // control like a joystick
#define JOY_RELATIVE_AXIS 0x00000010 // control like a mouse, spinner, trackball
#define JOY_MAX_AXES 6               // X, Y, Z, R, U, V
#define JOY_AXIS_X 0
#define JOY_AXIS_Y 1
#define JOY_AXIS_Z 2
#define JOY_AXIS_R 3
#define JOY_AXIS_U 4
#define JOY_AXIS_V 5

enum _ControlList
{
	AxisNada = 0,
	AxisForward,
	AxisLook,
	AxisSide,
	AxisTurn
};

DWORD dwAxisFlags[JOY_MAX_AXES] =
{
  JOY_RETURNX, JOY_RETURNY, JOY_RETURNZ, JOY_RETURNR, JOY_RETURNU, JOY_RETURNV
};

DWORD dwAxisMap[JOY_MAX_AXES];
DWORD dwControlMap[JOY_MAX_AXES];
PDWORD pdwRawValue[JOY_MAX_AXES];

// none of these cvars are saved over a session
// this means that advanced controller configuration needs to be executed
// each time.  this avoids any problems with getting back to a default usage
// or when changing from one controller to another.  this way at least something
// works.
cvar_t in_joystick = { "joystick", "0", true };
cvar_t joy_name = { "joyname", "joystick" };
cvar_t joy_advanced = { "joyadvanced", "0" };
cvar_t joy_advaxisx = { "joyadvaxisx", "0" };
cvar_t joy_advaxisy = { "joyadvaxisy", "0" };
cvar_t joy_advaxisz = { "joyadvaxisz", "0" };
cvar_t joy_advaxisr = { "joyadvaxisr", "0" };
cvar_t joy_advaxisu = { "joyadvaxisu", "0" };
cvar_t joy_advaxisv = { "joyadvaxisv", "0" };
cvar_t joy_forwardthreshold = { "joyforwardthreshold", "0.15" };
cvar_t joy_sidethreshold = { "joysidethreshold", "0.15" };
cvar_t joy_pitchthreshold = { "joypitchthreshold", "0.15" };
cvar_t joy_yawthreshold = { "joyyawthreshold", "0.15" };
cvar_t joy_forwardsensitivity = { "joyforwardsensitivity", "-1.0" };
cvar_t joy_sidesensitivity = { "joysidesensitivity", "-1.0" };
cvar_t joy_pitchsensitivity = { "joypitchsensitivity", "1.0" };
cvar_t joy_yawsensitivity = { "joyyawsensitivity", "-1.0" };
cvar_t joy_wwhack1 = { "joywwhack1", "0.0" };
cvar_t joy_wwhack2 = { "joywwhack2", "0.0" };

bool joy_avail, joy_advancedinit, joy_haspov;
DWORD joy_oldbuttonstate, joy_oldpovstate;

int joy_id;
DWORD joy_flags;
DWORD joy_numbuttons;

typedef struct MYDATA
{
	LONG lX;       // X axis goes here
	LONG lY;       // Y axis goes here
	LONG lZ;       // Z axis goes here
	BYTE bButtonA; // One button goes here
	BYTE bButtonB; // Another button goes here
	BYTE bButtonC; // Another button goes here
	BYTE bButtonD; // Another button goes here
} MYDATA;

static DIOBJECTDATAFORMAT rgodf[] =
{
	{&GUID_XAxis, FIELD_OFFSET(MYDATA, lX), DIDFT_AXIS | DIDFT_ANYINSTANCE, 0,},
	{&GUID_YAxis, FIELD_OFFSET(MYDATA, lY), DIDFT_AXIS | DIDFT_ANYINSTANCE, 0,},
	{&GUID_ZAxis, FIELD_OFFSET(MYDATA, lZ), 0x80000000 | DIDFT_AXIS | DIDFT_ANYINSTANCE, 0,},
	{0, FIELD_OFFSET(MYDATA, bButtonA), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
	{0, FIELD_OFFSET(MYDATA, bButtonB), DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
	{0, FIELD_OFFSET(MYDATA, bButtonC), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
	{0, FIELD_OFFSET(MYDATA, bButtonD), 0x80000000 | DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0,},
};

// forward-referenced functions
void IN_StartupJoystick();
void Joy_AdvancedUpdate_f();
void IN_JoyMove(usercmd_t *cmd);

/*
===========
RawValuePointer
===========
*/
PDWORD RawValuePointer(int axis)
{
	switch(axis)
	{
	case JOY_AXIS_X:
		return &ji.dwXpos;
	case JOY_AXIS_Y:
		return &ji.dwYpos;
	case JOY_AXIS_Z:
		return &ji.dwZpos;
	case JOY_AXIS_R:
		return &ji.dwRpos;
	case JOY_AXIS_U:
		return &ji.dwUpos;
	case JOY_AXIS_V:
		return &ji.dwVpos;
	}
}

/*
===========
Joy_AdvancedUpdate_f
===========
*/
void Joy_AdvancedUpdate_f()
{
	// called once by IN_ReadJoystick and by user whenever an update is needed
	// cvars are now available
	int i;
	DWORD dwTemp;

	// initialize all the maps
	for(i = 0; i < JOY_MAX_AXES; i++)
	{
		dwAxisMap[i] = AxisNada;
		dwControlMap[i] = JOY_ABSOLUTE_AXIS;
		pdwRawValue[i] = RawValuePointer(i);
	}

	if(joy_advanced.value == 0.0)
	{
		// default joystick initialization
		// 2 axes only with joystick control
		dwAxisMap[JOY_AXIS_X] = AxisTurn;
		// dwControlMap[JOY_AXIS_X] = JOY_ABSOLUTE_AXIS;
		dwAxisMap[JOY_AXIS_Y] = AxisForward;
		// dwControlMap[JOY_AXIS_Y] = JOY_ABSOLUTE_AXIS;
	}
	else
	{
		if(Q_strcmp(joy_name.string, "joystick") != 0)
		{
			// notify user of advanced controller
			Con_Printf("\n%s configured\n\n", joy_name.string);
		}

		// advanced initialization here
		// data supplied by user via joy_axisn cvars
		dwTemp = (DWORD)joy_advaxisx.value;
		dwAxisMap[JOY_AXIS_X] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_X] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD)joy_advaxisy.value;
		dwAxisMap[JOY_AXIS_Y] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Y] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD)joy_advaxisz.value;
		dwAxisMap[JOY_AXIS_Z] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_Z] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD)joy_advaxisr.value;
		dwAxisMap[JOY_AXIS_R] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_R] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD)joy_advaxisu.value;
		dwAxisMap[JOY_AXIS_U] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_U] = dwTemp & JOY_RELATIVE_AXIS;
		dwTemp = (DWORD)joy_advaxisv.value;
		dwAxisMap[JOY_AXIS_V] = dwTemp & 0x0000000f;
		dwControlMap[JOY_AXIS_V] = dwTemp & JOY_RELATIVE_AXIS;
	}

	// compute the axes to collect from DirectInput
	joy_flags = JOY_RETURNCENTERED | JOY_RETURNBUTTONS | JOY_RETURNPOV;
	for(i = 0; i < JOY_MAX_AXES; i++)
	{
		if(dwAxisMap[i] != AxisNada)
		{
			joy_flags |= dwAxisFlags[i];
		}
	}
}

/*
===========
IN_Commands
===========
*/
void IN_Commands()
{
	int i, key_index;
	DWORD buttonstate, povstate;

	if(!joy_avail)
	{
		return;
	}

	// loop through the joystick buttons
	// key a joystick event or auxillary event for higher number buttons for each state change
	buttonstate = ji.dwButtons;
	for(i = 0; i < joy_numbuttons; i++)
	{
		if((buttonstate & (1 << i)) && !(joy_oldbuttonstate & (1 << i)))
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event(key_index + i, true);
		}

		if(!(buttonstate & (1 << i)) && (joy_oldbuttonstate & (1 << i)))
		{
			key_index = (i < 4) ? K_JOY1 : K_AUX1;
			Key_Event(key_index + i, false);
		}
	}
	joy_oldbuttonstate = buttonstate;

	if(joy_haspov)
	{
		// convert POV information into 4 bits of state information
		// this avoids any potential problems related to moving from one
		// direction to another without going through the center position
		povstate = 0;
		if(ji.dwPOV != JOY_POVCENTERED)
		{
			if(ji.dwPOV == JOY_POVFORWARD)
				povstate |= 0x01;
			if(ji.dwPOV == JOY_POVRIGHT)
				povstate |= 0x02;
			if(ji.dwPOV == JOY_POVBACKWARD)
				povstate |= 0x04;
			if(ji.dwPOV == JOY_POVLEFT)
				povstate |= 0x08;
		}
		// determine which bits have changed and key an auxillary event for each change
		for(i = 0; i < 4; i++)
		{
			if((povstate & (1 << i)) && !(joy_oldpovstate & (1 << i)))
			{
				Key_Event(K_AUX29 + i, true);
			}

			if(!(povstate & (1 << i)) && (joy_oldpovstate & (1 << i)))
			{
				Key_Event(K_AUX29 + i, false);
			}
		}
		joy_oldpovstate = povstate;
	}
}

bool CJoystickWin::Startup()
{
	memset(&ji, 0, sizeof(ji));
	ji.dwSize = sizeof(ji);
	ji.dwFlags = JOY_RETURNCENTERED;

	if((mmr = joyGetPosEx(joy_id, &ji)) == JOYERR_NOERROR)
		return true;
	
	return false;
};

/*
=============== 
IN_ReadJoystick
=============== 
*/
bool CJoystickWin::Read()
{
	memset(&ji, 0, sizeof(ji));
	ji.dwSize = sizeof(ji);
	ji.dwFlags = joy_flags;

	if(joyGetPosEx(joy_id, &ji) == JOYERR_NOERROR)
	{
		// this is a hack -- there is a bug in the Logitech WingMan Warrior DirectInput Driver
		// rather than having 32768 be the zero point, they have the zero point at 32668
		// go figure -- anyway, now we get the full resolution out of the device
		if(joy_wwhack1.value != 0.0)
			ji.dwUpos += 100;

		return true;
	}
	else
	{
		// read error occurred
		// turning off the joystick seems too harsh for 1 read error,\
		// but what should be done?
		// Con_Printf ("IN_ReadJoystick: no response\n");
		// joy_avail = false;
		return false;
	};
};