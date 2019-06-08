/*
 * This file is part of Magenta Engine
 *
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

#include "InputSystemWin.hpp"
#include "public/keydefs.h"

#include <windows.h>
#include <xinput.h>

byte scantokey[128] = 
{ 
//  0           1       2       3       4       5       6       7 
//  8           9       A       B       C       D       E       F 
	0  ,    27,     '1',    '2',    '3',    '4',    '5',    '6', 
	'7',    '8',    '9',    '0',    '-',    '=',    K_BACKSPACE, 9, // 0 
	'q',    'w',    'e',    'r',    't',    'y',    'u',    'i', 
	'o',    'p',    '[',    ']',    13 ,    K_CTRL,'a',  's',      // 1 
	'd',    'f',    'g',    'h',    'j',    'k',    'l',    ';', 
	'\'' ,    '`',    K_SHIFT,'\\',  'z',    'x',    'c',    'v',      // 2 
	'b',    'n',    'm',    ',',    '.',    '/',    K_SHIFT,'*', 
	K_ALT,' ',   0  ,    K_F1, K_F2, K_F3, K_F4, K_F5,   // 3 
	K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE  ,    0  , K_HOME, 
	K_UPARROW,K_PGUP,'-',K_LEFTARROW,'5',K_RIGHTARROW,'+',K_END, //4 
	K_DOWNARROW,K_PGDN,K_INS,K_DEL,0,0,             0,              K_F11, 
	K_F12,0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 5 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0,        // 6 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0, 
	0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0  ,    0         // 7 
};

IInputSystem *GetInputSystem()
{
	static CInputSystemWin InputSystemWin();
	static CInputSystem InputSystem(InputSystemWin);
	return &InputSystem;
};

CInputSystemWin::CInputSystemWin() = default;
CInputSystemWin::~CInputSystemWin() = default;

void CInputSystemWin::Reset()
{
	for(int i = 0; i < XUSER_MAX_COUNT; ++i)
		ZeroMemory(&mvGamepads[i].mState, sizeof(XINPUT_STATE));
};

void CInputSystemWin::Poll()
{
	//if(!mbActive)
		//return;
	
	//if(!mbMsgPumpActive)
		//return;
	
	PumpInputEvents();
	PollGamepadStates();
};

void CInputSystemWin::AttachToWindow(void *apWindow)
{
	mpWindow = apWindow;
	mOriginalWndProc = GetWindowLongPtr(mpWindow, GWLP_WNDPROC);
	SetWindowLongPtr(mpWindow, GWLP_WNDPROC, InputWndProc);
};

void CInputSystemWin::DetachFromWindow()
{
	SetWindowLongPtr(mpWindow, GWLP_WNDPROC, mOriginalWndProc);
	mpWindow = nullptr;
};

int CInputSystemWin::GetGamepadCount() const
{
	return XUSER_MAX_COUNT;
};

void CInputSystemWin::SetGamepadActive(int anGamepad, bool abActive)
{
	if(anGamepad < 0)
		anGamepad = 0;
	
	if(anGamepad > XUSER_MAX_COUNT - 1)
		anGamepad = XUSER_MAX_COUNT - 1;
	
	mvGamepads[anGamepad].mbActive = abActive;
};

LRESULT CALLBACK CInputSystemWin::InputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(aMsg.lpMsg)
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		mvKeyStates[MapKey(wParam)] = true;
		mpEventDispatcher->KeyEvent(MapKey(wParam), true);
		break;
			
	case WM_KEYUP:
	case WM_SYSKEYUP:
		mvKeyStates[MapKey(wParam)] = false;
		mpEventDispatcher->KeyEvent(MapKey(wParam), false);
		break;
	};
};

/*
=======
MapKey

Map from windows to quake keynums
=======
*/
int CInputSystemWin::MapKey(int key)
{
	key = (key >> 16) & 255;
	if(key > 127)
		return 0;
	//if(scantokey[key] == 0)
		//Con_DPrintf("key 0x%02x has no translation\n", key); // TODO
	return scantokey[key];
};

void CInputSystemWin::PumpInputEvents()
{
	static MSG Msg{};
	
	if(PeekMessage(&Msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	};
};

void CInputSystemWin::PollGamepadStates()
{
	for(int i = 0; i < XUSER_MAX_COUNT; ++i)
	{
		if(mvGamepads[i].mbActive)
		{
			auto Result{XInputGetState(i, &mvGamepads[i].mState)};
			
			if(Result == ERROR_SUCCESS)
			{
				// TODO
			};
		};
	};
};