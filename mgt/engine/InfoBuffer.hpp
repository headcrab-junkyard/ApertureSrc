/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
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

#include "engine/IInfoBuffer.hpp"

class CInfoBuffer final : public IInfoBuffer
{
public:
	CInfoBuffer(char *asBuf);
	~CInfoBuffer();
	
	void SetKeyValue(const char *asKey, const char *asValue) override;
	const char *GetKeyValue(const char *asKey) const override;
	
	void RemoveKey(const char *asKey) override;
private:
	char *buf{nullptr};
};