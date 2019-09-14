/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018-2019 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

#include "engine/ICmdArgs.hpp"

class CCmdArgs final : public ICmdArgs
{
public:
	CCmdArgs(const char *text);
	~CCmdArgs();
	
	const char *GetByIndex(int anArg) const override;
	
	int CheckParm(const char *parm) const override;
	
	const char *GetArgValue(const char *asArg) const override;
	
	int GetCount() const override;
	
	const char *ToString() const override;
};