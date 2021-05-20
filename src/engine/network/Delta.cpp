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

#include "quakedef.h"
#include "Delta.hpp"

CDelta::CDelta(ISystem *apSystem, IFileSystem *apFileSystem)
	: mpSystem(apSystem), mpFileSytem(apFileSystem){}

void CDelta::Init()
{
	// TODO
};

void CDelta::Load(const char *filename)
{
	auto pFile{mpFileSytem->OpenFile(va("%s", filename), "rb")};
	
	if(!pFile)
		mpSystem->FatalError("DELTA_Load:  Couldn't load file %s", filename);
	
	// TODO
	
	mpFileSytem->CloseFile(pFile);
};

void CDelta::SetField(struct delta_s *pFields, const char *fieldname)
{
	// TODO
};

void CDelta::UnsetField(struct delta_s *pFields, const char *fieldname)
{
	// TODO
};

void CDelta::AddEncoder(char *name, void (*conditionalencode)(struct delta_s *pFields, const byte *from, const byte *to))
{
	// TODO
};

int CDelta::FindFieldIndex(struct delta_s *pFields, const char *fieldname)
{
	// TODO
	
	//("DELTA_FindFieldIndex:  Warning, couldn't find %s", fieldname);
	
	return 0;
};

void CDelta::SetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	// TODO
};

void CDelta::UnsetFieldByIndex(struct delta_s *pFields, int fieldNumber)
{
	// TODO
};

void CDelta::ParseField(const char *fieldname)
{
	if(!fieldname)
		mpSystem->FatalError("DELTA_ParseField:  Expecting fieldname");
	
	// TODO
};

void CDelta::ParseDescription()
{
	// TODO
};