/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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
/// @brief event manager interface

#pragma once

#include "CommonTypes.hpp"

constexpr auto MGT_EVENTMANAGER_INTERFACE_VERSION{"MGTEventManager001Alpha"};
// TODO: server/client versions?

using pfnEventHookCallback = void (*)(struct event_args_s *pArgs);

struct SEvent
{
	int nFlags;
	const edict_t *pInvoker;
	unsigned short nEventIndex;
	float fDelay;
	
	float *vOrigin;
	float *vAngles;
	
	float fParam1;
	float fParam2;
	
	int nParam1;
	int nParam2;
	
	int nBParam1;
	int nBParam2;
};

interface IEventManager : public IBaseInterface
{
	///
	virtual void HookEvent(const char *asName, pfnEventHookCallback) = 0; // TODO: RegisterHooker? (client only)
	
	///
	virtual unsigned short PrecacheEvent(int nType, const char *sName) = 0;
	
	///
	virtual void PlaybackEvent(const SEvent &aEvent) = 0;
};