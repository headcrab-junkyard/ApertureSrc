/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020 BlackPhrase
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

#include "GameConsole.hpp"

EXPOSE_SINGLE_INTERFACE(CGameConsole, IGameConsole, OGS_GAMECONSOLE_INTERFACE_VERSION);

CGameConsole::CGameConsole() = default;
CGameConsole::~CGameConsole() = default;

void CGameConsole::Init()
{
	// TODO
};

void CGameConsole::SetActive(bool abActive)
{
	// TODO
};

bool CGameConsole::IsVisible() const
{
	// TODO
	return false;
};

void CGameConsole::Clear()
{
	// TODO
};

void CGameConsole::Printf(const char *asMsg, ...)
{
	// TODO
};

void CGameConsole::DevPrintf(const char *asMsg, ...)
{
	// TODO
};

//void CGameConsole::SetParent(int /*vgui3::VPANEL*/ parent)
//{
	// TODO
//};