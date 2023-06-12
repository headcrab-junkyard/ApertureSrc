/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019-2023 BlackPhrase
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
#include "ConVar.hpp"

CConVar::CConVar(const char *asName, const char *asDefValue, const char *asDesc)
{
	mpData->name = asName; //strcpy(mpData->name, asName);
	
	strcpy(mpData->string, asDefValue); // TODO: alloc
	
	msDefValue = asDefValue;
	msDesc = asDesc;
}

CConVar::CConVar(cvar_t *apData, const char *asDesc)
{
	mpData = apData;
	msDesc = asDesc;
};

void CConVar::Reset()
{
	strcpy(mpData->value, msDefValue);
};

void CConVar::SetChangeValueListener(IConVarValueListener *apListener)
{
	mpValueChangeListener = apListener;
};

const char *CConVar::GetName() const
{
	return mpData->name;
};

const char *CConVar::GetDesc() const
{
	return msDesc;
};

void CConVar::SetFlags(int anFlags)
{
	mpData->flags = anFlags;
};

void CConVar::AddFlags(int anFlags)
{
	mpData->flags |= anFlags;
};

bool CConVar::HasFlags(int anFlags) const
{
	return mpData->flags & anFlags;
};

int CConVar::GetFlags() const
{
	return mpData->flags;
};

void CConVar::SetString(const char *asValue)
{
	auto sOldValue{asValue};
	
	strcpy(mpData->string, asValue); // TODO: alloc
	
	if(mpValueChangeListener)
		mpValueChangeListener(GetName(), sOldValue, asValue);
};

void CConVar::SetInt(int anValue)
{
	SetString(std::to_string(anValue));
};

void CConVar::SetFloat(float afValue)
{
	SetString(std::to_string(afValue));
};

void CConVar::SetBool(bool abValue)
{
	SetString(std::to_string(abValue));
};

const char *CConVar::GetString() const
{
	return mpData->string;
};

int CConVar::GetInt() const
{
	return atoi(mpData->string);
};

float CConVar::GetFloat() const
{
	return atof(mpData->string);
};

bool CConVar::GetBool() const
{
	if(GetInt() > 0 || !strcmp(GetString(), "true"))
		return true;
	return false;
};