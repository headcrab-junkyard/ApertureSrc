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

#pragma once

#include "IInputImpl.hpp"

#include <windows.h>

class CInputWin final : public IInputImpl
{
public:
	CInputWin();
	~CInputWin();
	
	void Init() override;
	void Shutdown() override;
	
	void ActivateMouse();
	void DeactivateMouse();
private:
	bool InitDInput();
	
	void StartupMouse();
	void StartupJoystick();
	
	bool ReadJoystick();
	
	LPDIRECTINPUT g_pdi{nullptr};
	LPDIRECTINPUTDEVICE g_pMouse{nullptr};

	//JOYINFOEX ji;

	HINSTANCE hInstDI;
	
	unsigned int uiWheelMessage{0};
	
	int newmouseparms[3] = {0, 0, 1};
	int originalmouseparms[3];
	
	bool dinput{false};
	bool dinput_acquired{false};
	
	bool restore_spi{false};
	
	bool mouseinitialized{false};
	bool mouseactive{false};
	bool mouseactivatetoggle{false};
	bool mouseparmsvalid{false};
};