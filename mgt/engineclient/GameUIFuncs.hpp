/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2020 BlackPhrase
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
/// @brief IGameUIFuncs interface implementation

#pragma once

#include "engineclient/IGameUIFuncs.hpp"

class CGameUIFuncs final : public IGameUIFuncs
{
public:
	CGameUIFuncs();
	~CGameUIFuncs();

	bool IsKeyDown(char const *keyname, bool &isdown) override;

	const char *Key_NameForKey(int keynum) override;
	const char *Key_BindingForKey(int keynum) override;

	vgui2::KeyCode GetVGUI2KeyCodeForBind(const char *bind) override;

	void GetVideoModes(struct vmode_s **liststart, int *count) override;
	void GetCurrentVideoMode(int *wide, int *tall, int *bpp) override;

	void GetCurrentRenderer(char *name, int namelen,
	                        int *windowed, int *hdmodels,
	                        int *addons_folder, int *vid_level) override;
	//void GetCurrentRenderer(char *name, int namelen, int *windowed) override;

	bool IsConnectedToVACSecureServer() override;

	int Key_KeyStringToKeyNum(const char *string) override;
};