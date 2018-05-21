/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief IGameUIFuncs interface implementation

#include "quakedef.h"
#include "GameUIFuncs.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXPOSE_SINGLE_INTERFACE(CGameUIFuncs, IGameUIFuncs, MGT_GAMEUIFUNCS_INTERFACE_VERSION);

CGameUIFuncs::CGameUIFuncs() = default;
CGameUIFuncs::~CGameUIFuncs() = default;

bool CGameUIFuncs::IsKeyDown(char const *keyname, bool &isdown)
{
	return false; // TODO: Key_IsDown(keyname, isdown);
};

const char *CGameUIFuncs::Key_NameForKey(int keynum)
{
	return Key_NameForKey(keynum);
};

const char *CGameUIFuncs::Key_BindingForKey(int keynum)
{
	return Key_BindingForKey(keynum);
};

vgui3::KeyCode CGameUIFuncs::GetVGUI2KeyCodeForBind(const char *bind)
{
	return vgui3::KeyCode::KEY_NONE;
};

void CGameUIFuncs::GetVideoModes(struct vmode_s **liststart, int *count)
{
};

void CGameUIFuncs::GetCurrentVideoMode(int *wide, int *tall, int *bpp)
{
};

void CGameUIFuncs::GetCurrentRenderer(char *name, int namelen, int *windowed, int *hdmodels, int *addons_folder, int *vid_level)
{
};

bool CGameUIFuncs::IsConnectedToVACSecureServer()
{
	return false;
};

int CGameUIFuncs::Key_KeyStringToKeyNum(const char *string)
{
	return Key_KeyStringToKeyNum(string);
};