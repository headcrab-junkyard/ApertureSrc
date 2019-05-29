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
#include "InputEventDispatcher.hpp"
#include "public/keydefs.h"

EXPOSE_SINGLE_INTERFACE(CInputSystem, IInputSystem, MGT_INPUTSYSTEM_INTERFACE_VERSION);

CInputSystem::CInputSystem() = default;
CInputSystem::~CInputSystem() = default;

bool CInputSystem::Init(void *apWindow)
{
	mpEventDispatcher = std::make_unique<CInputEventDispatcher>();
	
	mpImpl->Init();
	
	AttachToWindow(apWindow);
	return true;
};

void CInputSystem::Shutdown()
{
	DetachFromWindow();
	mpImpl->Shutdown();
};

void CInputSystem::Frame()
{
	Poll();
};

void CInputSystem::AddEventListener(IInputEventListener *apListener)
{
	mpEventDispatcher->AddListener(apListener);
};

void CInputSystem::RemoveEventListener(IInputEventListener *apListener)
{
	mpEventDispatcher->RemoveListener(apListener);
};

void CInputSystem::AttachToWindow(void *apWindow)
{
};