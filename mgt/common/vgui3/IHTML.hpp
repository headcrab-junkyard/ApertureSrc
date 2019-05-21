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
#include "MouseCode.hpp"
#include "KeyCode.hpp"
#include "IImage.hpp"

namespace vgui3
{

struct IHTML
{
	enum MOUSE_STATE { UP, DOWN, MOVE };

	virtual void OpenURL(const char *url) = 0;
	virtual bool StopLoading() = 0;
	virtual bool Refresh() = 0;
	virtual bool Show(bool shown) = 0;
	virtual char *GetOpenedPage() = 0;
	virtual void OnSize(int x, int y, int w, int h) = 0;
	virtual void GetHTMLSize(int &wide, int &tall) = 0;
	virtual void Clear() = 0;
	virtual void AddText(const char *text) = 0;
	virtual void OnMouse(MouseCode code, MOUSE_STATE s, int x, int y) = 0;
	virtual void OnChar(wchar_t unichar) = 0;
	virtual void OnKeyDown(KeyCode code) = 0;
	virtual IImage *GetBitmap() = 0;
	virtual void SetVisible(bool state) = 0;
};

struct IHTMLEvents
{
	virtual bool OnStartURL(const char *url, const char *target, bool first) = 0;
	virtual void OnFinishURL(const char *url) = 0;
	virtual void OnProgressURL(long current, long maximum) = 0;
	virtual void OnSetStatusText(const char *text) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnLink() = 0;
	virtual void OffLink() = 0;
};

}; // namespace vgui3