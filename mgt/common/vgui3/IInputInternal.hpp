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

#include "IInput.hpp"

#define VGUI_INPUTINTERNAL_INTERFACE_VERSION "VGUI_InputInternal001"

namespace vgui3
{

typedef int HInputContext;

#define DEFAULT_INPUT_CONTEXT ((HInputContext)~0)

struct IInputInternal : public IInput
{
	virtual void RunFrame() = 0;
	virtual void UpdateMouseFocus(int x, int y) = 0;
	virtual void PanelDeleted(VPANEL panel) = 0;
	virtual bool InternalCursorMoved(int x, int y) = 0;
	virtual bool InternalMousePressed(MouseCode code) = 0;
	virtual bool InternalMouseDoublePressed(MouseCode code) = 0;
	virtual bool InternalMouseReleased(MouseCode code) = 0;
	virtual bool InternalMouseWheeled(int delta) = 0;
	virtual bool InternalKeyCodePressed(KeyCode code) = 0;
	virtual void InternalKeyCodeTyped(KeyCode code) = 0;
	virtual void InternalKeyTyped(wchar_t unichar) = 0;
	virtual bool InternalKeyCodeReleased(KeyCode code) = 0;
	virtual HInputContext CreateInputContext() = 0;
	virtual void DestroyInputContext(HInputContext context) = 0;
	virtual void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot) = 0;
	virtual void ActivateInputContext(HInputContext context) = 0;
	virtual VPANEL GetMouseCapture() = 0;
	virtual bool IsChildOfModalPanel(VPANEL panel) = 0;
	virtual void ResetInputContext(HInputContext context) = 0;
};

}; // namespace vgui3