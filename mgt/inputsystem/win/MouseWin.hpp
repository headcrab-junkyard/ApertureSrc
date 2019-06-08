/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

class CMouseWin
{
public:
	CMouseWin();
	~CMouseWin();
	
	void ClearStates();
	
	void Show();
	void Hide();
private:
	LPDIRECTINPUTDEVICE g_pMouse{nullptr};
	
	POINT current_pos{};
	
	int newmouseparms[3] = {0, 0, 1};
	int originalmouseparms[3]{};
	
	int mouse_buttons{};
	int mouse_oldbuttonstate{};
	
	int mouse_x{}, mouse_y{};
	int old_mouse_x{}, old_mouse_y{};
	int mx_accum{}, my_accum{};
	
	bool mouseinitialized{false};
	bool mouseactive{false};
	bool mouseactivatetoggle{false};
	bool mouseparmsvalid{false};
};