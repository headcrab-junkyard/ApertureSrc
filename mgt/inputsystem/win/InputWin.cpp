/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-2005 Id Software, Inc.
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

#include "InputWin.hpp"

#define iDirectInputCreate(a, b, c, d) pDirectInputCreate(a, b, c, d)

#define NUM_OBJECTS (sizeof(rgodf) / sizeof(rgodf[0]))

constexpr auto DINPUT_BUFFERSIZE{16};

HRESULT (WINAPI *pDirectInputCreate)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUT *lplpDirectInput, LPUNKNOWN punkOuter);

// NOTE TTimo: would be easier using c_dfDIMouse or c_dfDIMouse2
static DIDATAFORMAT df =
{
	sizeof(DIDATAFORMAT),       // this structure
	sizeof(DIOBJECTDATAFORMAT), // size of object data format
	DIDF_RELAXIS,               // absolute axis coordinates
	sizeof(MYDATA),             // device data size
	NUM_OBJECTS,                // number of objects
	rgodf,                      // and here they are
};

CInputWin::CInputWin() = default;
CInputWin::~CInputWin() = default;

void CInputWin::Startup()
{
	StartupMouse();
	StartupJoystick();
};

/*
===========
IN_Init
===========
*/
void CInputWin::Init()
{
	uiWheelMessage = RegisterWindowMessage("MSWHEEL_ROLLMSG");
	
	Startup();
};

/*
==========================
IN_ShutdownDIMouse
==========================
*/
void IN_ShutdownDIMouse()
{
	if(g_pMouse)
	{
		IDirectInputDevice_Release(g_pMouse);
		g_pMouse = nullptr;
	};

	if(g_pdi)
	{
		IDirectInput_Release(g_pdi);
		g_pdi = nullptr;
	};
};

/*
===========
IN_Shutdown
===========
*/
void CInputWin::Shutdown()
{
	DeactivateMouse();
	IN_ShowMouse();

	IN_ShutdownDIMouse();
};

/*
==================
IN_Frame

Called every frame, even if not generating commands
==================
*/
void CInputWin::Frame()
{
};

/*
===========
IN_Activate

Called when the main window gains or loses focus.
The window may have been destroyed and recreated
between a deactivate and an activate.
===========
*/
void CInputWin::SetActive(bool abActive)
{
	in_appactive = abActive;
	
	if(!abActive)
		DeactivateMouse();
};

/*
===========
IN_ActivateMouse
===========
*/
void CInputWin::ActivateMouse()
{
	mouseactivatetoggle = true;

	if(mouseinitialized)
	{
		if(dinput)
		{
			if(g_pMouse)
			{
				if(!dinput_acquired)
				{
					IDirectInputDevice_Acquire(g_pMouse);
					dinput_acquired = true;
				};
			}
			else
				return;
		}
		else
		{
			if(mouseparmsvalid)
				restore_spi = SystemParametersInfo(SPI_SETMOUSE, 0, newmouseparms, 0);

			SetCursorPos(window_center_x, window_center_y);
			SetCapture(mainwindow);
			ClipCursor(&window_rect);
		};

		mouseactive = true;
	};
};

/*
===========
IN_DeactivateMouse
===========
*/
void CInputWin::DeactivateMouse()
{
	mouseactivatetoggle = false;

	if(mouseinitialized)
	{
		if(dinput)
		{
			if(g_pMouse)
			{
				if(dinput_acquired)
				{
					IDirectInputDevice_Unacquire(g_pMouse);
					dinput_acquired = false;
				};
			};
		}
		else
		{
			if(restore_spi)
				SystemParametersInfo(SPI_SETMOUSE, 0, originalmouseparms, 0);

			ClipCursor(nullptr);
			ReleaseCapture();
		};

		mouseactive = false;
	};
};

/*
===========
IN_InitDInput
===========
*/
bool IN_InitDInput()
{
	DIPROPDWORD dipdw =
	{
		{
			sizeof(DIPROPDWORD),  // diph.dwSize
			sizeof(DIPROPHEADER), // diph.dwHeaderSize
			0,                    // diph.dwObj
			DIPH_DEVICE,          // diph.dwHow
		},
		DINPUT_BUFFERSIZE, // dwData
	};

	//Com_Printf( "Initializing DirectInput...\n");

	if(!hInstDI)
	{
		hInstDI = LoadLibrary("dinput.dll");

		if(hInstDI == nullptr)
		{
			Con_SafePrintf("Couldn't load dinput.dll\n");
			return false;
		};
	};

	if(!pDirectInputCreate)
	{
		pDirectInputCreate = (void *)GetProcAddress(hInstDI, "DirectInputCreateA");

		if(!pDirectInputCreate)
		{
			Con_SafePrintf("Couldn't get DI proc addr\n");
			return false;
		};
	};

	// register with DirectInput and get an IDirectInput to play with.
	HRESULT hr = iDirectInputCreate(global_hInstance, DIRECTINPUT_VERSION, &g_pdi, nullptr);

	if(FAILED(hr))
	{
		//Com_Printf("iDirectInputCreate failed\n");
		return false;
	};

	// obtain an interface to the system mouse device.
	hr = IDirectInput_CreateDevice(g_pdi, &GUID_SysMouse, &g_pMouse, nullptr);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't open DI mouse device\n"); //Com_Printf ("Couldn't open DI mouse device\n");
		return false;
	};

	// set the data format to "mouse format".
	hr = IDirectInputDevice_SetDataFormat(g_pMouse, &df);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI mouse format\n"); //Com_Printf ("Couldn't set DI mouse format\n");
		return false;
	};

	// set the cooperativity level.
	hr = IDirectInputDevice_SetCooperativeLevel(g_pMouse, mainwindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	// https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=50
	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI coop level\n"); //Com_Printf ("Couldn't set DI coop level\n");
		return false;
	};

	// set the buffer size to DINPUT_BUFFERSIZE elements.
	// the buffer size is a DWORD property associated with the device
	hr = IDirectInputDevice_SetProperty(g_pMouse, DIPROP_BUFFERSIZE, &dipdw.diph);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI buffersize\n"); //Com_Printf ("Couldn't set DI buffersize\n");
		return false;
	};

	// clear any pending samples
	//IN_DIMouse( &x, &y );
	//IN_DIMouse( &x, &y );

	//Com_Printf( "DirectInput initialized.\n");
	return true;
};

/*
===========
IN_StartupMouse
===========
*/
void CInputWin::StartupMouse()
{
	HDC hdc;

	if(COM_CheckParm("-nomouse"))
		return;

	mouseinitialized = true;

	if(COM_CheckParm("-dinput"))
	{
		dinput = InitDInput();

		if(dinput)
			Con_SafePrintf("DirectInput initialized\n");
		else
			Con_SafePrintf("DirectInput not initialized\n");
	};

	if(!dinput)
	{
		mouseparmsvalid = SystemParametersInfo(SPI_GETMOUSE, 0, originalmouseparms, 0);

		if(mouseparmsvalid)
		{
			if(COM_CheckParm("-noforcemspd"))
				newmouseparms[2] = originalmouseparms[2];

			if(COM_CheckParm("-noforcemaccel"))
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
			};

			if(COM_CheckParm("-noforcemparms"))
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
				newmouseparms[2] = originalmouseparms[2];
			};
		};
	};

	mouse_buttons = 3;

	// if a fullscreen video mode was set before the mouse was initialized,
	// set the mouse state appropriately
	if(mouseactivatetoggle)
		IN_ActivateMouse();
};

/* 
=============== 
IN_StartupJoystick 
=============== 
*/
void CInputWin::StartupJoystick()
{
	int i, numdevs;
	JOYCAPS jc;
	MMRESULT mmr;

	// assume no joystick
	joy_avail = false;

	// abort startup if user requests no joystick
	if(COM_CheckParm("-nojoy"))
		return;

	// verify joystick driver is present
	if((numdevs = joyGetNumDevs()) == 0)
	{
		Con_Printf("\njoystick not found -- driver not present\n\n");
		return;
	};

	// cycle through the joystick ids for the first valid one
	for(joy_id = 0; joy_id < numdevs; joy_id++)
	{
		memset(&ji, 0, sizeof(ji));
		ji.dwSize = sizeof(ji);
		ji.dwFlags = JOY_RETURNCENTERED;

		if((mmr = joyGetPosEx(joy_id, &ji)) == JOYERR_NOERROR)
			break;
	};

	// abort startup if we didn't find a valid joystick
	if(mmr != JOYERR_NOERROR)
	{
		Con_Printf("\njoystick not found -- no valid joysticks (%x)\n\n", mmr);
		return;
	};

	// get the capabilities of the selected joystick
	// abort startup if command fails
	memset(&jc, 0, sizeof(jc));
	if((mmr = joyGetDevCaps(joy_id, &jc, sizeof(jc))) != JOYERR_NOERROR)
	{
		Con_Printf("\njoystick not found -- invalid joystick capabilities (%x)\n\n", mmr);
		return;
	};

	// save the joystick's number of buttons and POV status
	joy_numbuttons = jc.wNumButtons;
	joy_haspov = jc.wCaps & JOYCAPS_HASPOV;

	// old button and POV states default to no buttons pressed
	joy_oldbuttonstate = joy_oldpovstate = 0;

	// mark the joystick as available and advanced initialization not completed
	// this is needed as cvars are not available during initialization

	joy_avail = true;
	joy_advancedinit = false;

	Con_Printf("\njoystick detected\n\n");
};

/* 
=============== 
IN_ReadJoystick
=============== 
*/
bool CInputWin::ReadJoystick()
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