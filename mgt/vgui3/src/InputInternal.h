/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2018 BlackPhrase
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

#include "vgui3/IInputInternal.h"

namespace vgui3
{

class CInputInternal : public IInputInternal
{
public:
	void RunFrame();

	void UpdateMouseFocus(int x, int y);

	void PanelDeleted(VPANEL panel);

	bool InternalCursorMoved(int x, int y);

	bool InternalMousePressed(MouseCode code);
	bool InternalMouseDoublePressed(MouseCode code);
	bool InternalMouseReleased(MouseCode code);
	bool InternalMouseWheeled(int delta);

	bool InternalKeyCodePressed(KeyCode code);
	void InternalKeyCodeTyped(KeyCode code);
	void InternalKeyTyped(wchar_t unichar);
	bool InternalKeyCodeReleased(KeyCode code);

	HInputContext CreateInputContext();
	void DestroyInputContext(HInputContext context);

	void AssociatePanelWithInputContext(HInputContext context, VPANEL pRoot);
	void ActivateInputContext(HInputContext context);

	VPANEL GetMouseCapture();

	bool IsChildOfModalPanel(VPANEL panel);

	void ResetInputContext(HInputContext context);
};

}; // namespace vgui3