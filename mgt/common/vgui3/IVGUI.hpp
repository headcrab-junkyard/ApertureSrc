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

#pragma once

#include "VGUI.hpp"
#include "tier1/interface.h"

constexpr auto MGT_VGUI3_INTERFACE_VERSION{"MGTVGUI3001Alpha"};

class KeyValues;

namespace vgui3
{

using HPanel = unsigned long;
using HContext = int;

enum
{
	DEFAULT_VGUI_CONTEXT = ((HContext)~0)
};

struct IVGui : public IBaseInterface
{
	virtual bool Init(CreateInterfaceFn *factoryList, int numFactories) = 0;
	virtual void Shutdown() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual bool IsRunning() = 0;
	virtual void RunFrame() = 0;
	virtual void ShutdownMessage(unsigned int shutdownID) = 0;
	virtual VPANEL AllocPanel() = 0;
	virtual void FreePanel(VPANEL panel) = 0;
	virtual void DPrintf(const char *format, ...) = 0;
	virtual void DPrintf2(const char *format, ...) = 0;
	virtual void SpewAllActivePanelNames() = 0;
	virtual HPanel PanelToHandle(VPANEL panel) = 0;
	virtual VPANEL HandleToPanel(HPanel index) = 0;
	virtual void MarkPanelForDeletion(VPANEL panel) = 0;
	virtual void AddTickSignal(VPANEL panel, int intervalMilliseconds = 0) = 0;
	virtual void RemoveTickSignal(VPANEL panel) = 0;
	virtual void PostMessage(VPANEL target, KeyValues *params, VPANEL from, float delaySeconds = 0.0) = 0;
	virtual HContext CreateContext() = 0;
	virtual void DestroyContext(HContext context) = 0;
	virtual void AssociatePanelWithContext(HContext context, VPANEL pRoot) = 0;
	virtual void ActivateContext(HContext context) = 0;
	virtual void SetSleep(bool abState) = 0;
	virtual bool GetShouldVGuiControlSleep() = 0;
};

}; // namespace vgui3