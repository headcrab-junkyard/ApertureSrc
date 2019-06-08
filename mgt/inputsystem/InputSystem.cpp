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

#include "InputSystem.hpp"
#include "IInputSystemImpl.hpp"
#include "InputEventDispatcher.hpp"

extern IInputSystem *GetInputSystem();

EXPOSE_SINGLE_INTERFACE_FUNCTION(CInputSystem, IInputSystem, MGT_INPUTSYSTEM_INTERFACE_VERSION, GetInputSystem);

CInputSystem::CInputSystem(IInputSystemImpl &aImpl) : mImpl(aImpl){}

CInputSystem::~CInputSystem() = default;

bool CInputSystem::Init(bool abConsoleTextMode)
{
	mpEventDispatcher = std::make_unique<CInputEventDispatcher>();
	
	//mImpl.Init();
	
	// TODO: handle abConsoleTextMode
	return true;
};

void CInputSystem::Shutdown()
{
	DetachFromWindow();
	//mImpl.Shutdown();
};

void CInputSystem::Reset()
{
	mvKeyStates.fill(0);
	mvOldKeyStates.fill(0);
	
	mImpl.Reset();
};

void CInputSystem::Poll()
{
	mvOldKeyStates = mvKeyStates;
	mImpl.Poll();
};

void CInputSystem::AttachToWindow(void *apWindow)
{
	mImpl.AttachToWindow(apWindow);
};

void CInputSystem::DetachFromWindow()
{
	mImpl.DetachFromWindow();
};

void CInputSystem::AddEventListener(IInputEventListener *apListener)
{
	mpEventDispatcher->AddListener(apListener);
};

void CInputSystem::RemoveEventListener(IInputEventListener *apListener)
{
	mpEventDispatcher->RemoveListener(apListener);
};

bool CInputSystem::IsKeyDown(eInputKey aeKey) const
{
	return mvKeyStates[aeKey];
};

int CInputSystem::GetAnalogValue(AnalogCode aeCode) const
{
	return 0; // TODO
};

int CInputSystem::GetGamepadCount() const
{
	return mImpl.GetGamepadCount();
};

void CInputSystem::SetGamepadActive(int anGamepad, bool abActive)
{
	mImpl.SetGamepadActive(anGamepad, abActive);
};