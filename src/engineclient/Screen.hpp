/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2021 BlackPhrase
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

#pragma once

class CScreen
{
public:
	void Init();

	void Update();
	
	void SetActive(bool abActive);

	void SizeUp();
	void SizeDown();

	void BringDownConsole();
	void CenterPrint(const char *str);

	void BeginLoadingPlaque();
	void EndLoadingPlaque();

	int ModalMessage(const char *text);

#ifdef _WIN32
	void UpdateWholeScreen();
#endif
private:
	bool initialized{false}; ///< ready to draw		
	bool disabled_for_loading{false};
};