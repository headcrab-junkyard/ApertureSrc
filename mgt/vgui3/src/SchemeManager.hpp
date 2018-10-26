/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2018 BlackPhrase
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

#include "vgui3/IScheme.hpp"

namespace vgui3
{

class CSchemeManager : public ISchemeManager
{
public:
	CSchemeManager();
	~CSchemeManager();
	
	HScheme LoadSchemeFromFile(const char *fileName, const char *tag) override;

	void ReloadSchemes() override;

	HScheme GetDefaultScheme() override;
	HScheme GetScheme(const char *tag) override;

	IImage *GetImage(const char *imageName, bool hardwareFiltered) override;
	HTexture GetImageID(const char *imageName, bool hardwareFiltered) override;

	IScheme *GetIScheme(HScheme scheme) override;

	void Shutdown(bool full = true) override;

	int GetProportionalScaledValue(int normalizedValue) override;
	int GetProportionalNormalizedValue(int scaledValue) override;
};

}; // namespace vgui3