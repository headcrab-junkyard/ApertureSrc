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

#include <windows.h>

#include "InputSystem.hpp"

EXPOSE_SINGLE_INTERFACE(CInputSystem, IInputSystem, OGS_INPUTSYSTEM_INTERFACE_VERSION);

WNDPROC CInputSystem::mRootWndProc{nullptr};

LRESULT CALLBACK InputWndProc(HWND ahWnd, UINT anMsg, WPARAM anWParam, LPARAM anLParam)
{
	switch(anMsg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
		break;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		break;
	case WM_MOUSEWHEEL:
		break;
	case WM_MOUSEMOVE:
		break;
	};
	
	return CallWindowProc(CInputSystem::mRootWndProc, ahWnd, anMsg, anWParam, anLParam);
};

bool CInputSystem::Init(CreateInterfaceFn afnEngineFactory, bool abTextConsoleMode)
{
	return true;
};

void CInputSystem::Shutdown()
{
	DetachFromWindow();
};

void CInputSystem::Reset()
{
	for(int i = 0; i < 256; ++i)
		mvDownKeys[i] = false;
};

void CInputSystem::Poll()
{
	if(!mhWindow)
		return;
	
	PumpWindowMessages();
};

void CInputSystem::AttachToWindow(void *apWindow)
{
	// Detach from the previous window first
	DetachFromWindow();
	
	mhWindow = reinterpret_cast<HWND>(apWindow);	

	mRootWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(mhWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(InputWndProc)));
};

void CInputSystem::DetachFromWindow()
{
	// Check if already detached or not attached
	if(!mhWindow)
		return;
	
	SetWindowLongPtr(mhWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(mRootWndProc));
	mhWindow = nullptr;
};

bool CInputSystem::IsKeyDown(InputKey aeKey) const
{
	return mvDownKeys[static_cast<std::underlying_type_t<InputKey>>(aeKey)];
};

int CInputSystem::GetGamepadCount() const
{
	return 1; //XUSER_MAX_COUNT; // TODO
};

void CInputSystem::SetGamepadActive(int anIndex, bool abActive)
{
	//assert(anIndex > 0);
	//assert(anIndex < XUSER_MAX_COUNT);
	
	//if(anIndex < 0)
		//anIndex = 0;
	
	//if(anIndex > XUSER_MAX_COUNT())
		//anIndex = XUSER_MAX_COUNT;
	
	mvGamepads[anIndex] = abActive;
};

void CInputSystem::PumpWindowMessages()
{
	static MSG Msg{};
	
	if(PeekMessage(&Msg, mhWindow, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	};
};