/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2022 BlackPhrase
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
/// @brief entity dictionary pool class

#pragma once

#include "CommonTypes.hpp"

interface IFile;
class CEntityDict;

class CEntityDictPool
{
public:
	CEntityDict *Alloc();
	void Free(CEntityDict *apDict);

	void WriteGlobals(IFile *apFile);
	void ParseGlobals(char *asData);

	void LoadFromFile(char *asData);

	void PrintEdicts();
	void PrintNum(int anEnt);
private:
	CEntityDict *mpHead{nullptr};
};