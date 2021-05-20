/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018, 2021 BlackPhrase
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

#include "CommonTypes.hpp"

struct delta_s;

interface ISystem;
interface IFileSystem;

class CDelta
{
public:
	CDelta(ISystem *apSystem, IFileSystem *apFileSystem);
	
	void Init();

	void Load(const char *filename);

	void SetField(struct delta_s *pFields, const char *fieldname);
	void UnsetField(struct delta_s *pFields, const char *fieldname);

	void AddEncoder(char *name, void (*conditionalencode)(struct delta_s *pFields, const byte *from, const byte *to));

	int FindFieldIndex(struct delta_s *pFields, const char *fieldname);

	void SetFieldByIndex(struct delta_s *pFields, int fieldNumber);
	void UnsetFieldByIndex(struct delta_s *pFields, int fieldNumber);

	void ParseField(const char *fieldname);

	void ParseDescription();
private:
	ISystem *mpSystem{nullptr};
	IFileSystem *mpFileSytem{nullptr};
};