/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020-2021 BlackPhrase
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

#include "inputsystem/IInputSystem.hpp"

class CInputSystem : public IInputSystem
{
public:
	bool Init(CreateInterfaceFn afnEngineFactory, bool abTextConsoleMode) override;
	void Shutdown() override;
	
	//void Update(float afTimeStep) override;
	
	void Reset() override;
	
	void Poll() override;
	
	void AttachToWindow(void *apWindow) override;
	void DetachFromWindow() override;
	
	bool IsKeyDown(InputKey aeKey) const override;
	
	int GetGamepadCount() const override;
	void SetGamepadActive(int anIndex, bool abState) override;
private:
	void PumpWindowMessages();
private:
	bool mvDownKeys[256]{};
	
	bool mvGamepads[4]{};
	
	HWND mhWindow{nullptr};
public:
	static WNDPROC mRootWndProc;
};