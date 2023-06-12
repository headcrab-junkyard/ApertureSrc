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

#pragma once

//#include <next/CommonTypes.hpp>
#include <public/tier1/IConVar.hpp>

using cvar_t = struct cvar_s;

class CConVar : public IConVar
{
public:
	CConVar(const char *asName, const char *asDefValue, const char *asDesc = "");
	CConVar(cvar_t *apData, const char *asDesc = "");
	
	void Reset() override;
	
	void SetChangeValueListener(IConVarValueListener *apListener) override;
	
	const char *GetName() const override;
	const char *GetDesc() const override;
	
	void SetFlags(int anFlags) override;
	void AddFlags(int anFlags) override;
	bool HasFlags(int anFlags) const override;
	int GetFlags() const override;
	
	void SetString(const char *asValue) override;
	void SetInt(int anValue) override;
	void SetFloat(float afValue) override;
	void SetBool(bool abValue) override;
	
	const char *GetString() const override;
	int GetInt() const override;
	float GetFloat() const override;
	bool GetBool() const override;
private:
	cvar_t *mpData{nullptr};
	IConVarValueListener *mpValueChangeListener{nullptr};
	const char *msDefValue{""};
	const char *msDesc{""};
};