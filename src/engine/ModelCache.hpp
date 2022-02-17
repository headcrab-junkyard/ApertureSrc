/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2021 BlackPhrase
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

class CModel;

using model_t = struct model_s;

class CModelCache
{
public:
	void Init();
	
	void ClearAll();
	
	model_t *GetForName(const char *name, bool crash) const;
	
	void TouchModel(const char *name);
private:
	model_t *FindName(const char *name) const;
};