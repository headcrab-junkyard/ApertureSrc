/*
 * This file is part of Magenta Engine
 *
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

#include "inputsystem/IInputSystemImpl.hpp"
#include "MouseWin.hpp"
#include "JoystickWin.hpp"

class CInputSystemWin final : public IInputSystemImpl
{
public:
	CInputSystemWin();
	~CInputSystemWin();
	
	void Poll() override;
	
	void AttachToWindow(void *apWindow) override;
	void DetachFromWindow() override;
	
	int GetGamepadCount() const override;
	void SetGamepadActive(int anGamepad, bool abActive) override;
private:
	static LRESULT CALLBACK InputWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	int MapKey(int anKey);
	
	void PumpInputEvents();
	void PollGamepadStates();
	
	CMouseWin mMouse{};
	CJoystickWin mJoystick[4]{};
	
	struct GamepadState
	{
		XINPUT_STATE mState{};
		bool mbActive{false};
	};
	
	GamepadState mvGamepads[XUSER_MAX_COUNT];
};