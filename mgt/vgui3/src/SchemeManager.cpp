/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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

#include "SchemeManager.hpp"

namespace vgui3
{

CSchemeManager::CSchemeManager() = default;
CSchemeManager::~CSchemeManager() = default;

HScheme CSchemeManager::LoadSchemeFromFile(const char *fileName, const char *tag)
{
	return 0;
};

void CSchemeManager::ReloadSchemes()
{
};

HScheme CSchemeManager::GetDefaultScheme()
{
	return 0;
};

HScheme CSchemeManager::GetScheme(const char *tag)
{
	return 0;
};

IImage *CSchemeManager::GetImage(const char *imageName, bool hardwareFiltered)
{
	return nullptr;
};

HTexture CSchemeManager::GetImageID(const char *imageName, bool hardwareFiltered)
{
	return 0;
};

IScheme *CSchemeManager::GetIScheme(HScheme scheme)
{
	return nullptr;
};

void CSchemeManager::Shutdown(bool full)
{
};

int CSchemeManager::GetProportionalScaledValue(int normalizedValue)
{
	return 0;
};

int CSchemeManager::GetProportionalNormalizedValue(int scaledValue)
{
	return 0;
};

}; // namespace vgui3