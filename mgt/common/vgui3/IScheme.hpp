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

#include "VGUI.hpp"
#include "tier1/interface.h"
#include "Color.h"

#define VGUI_SCHEME_INTERFACE_VERSION "VGUI_Scheme009"

//class Color;

namespace vgui3
{

using HScheme = unsigned long;
using HTexture = unsigned long;

struct IBorder;
struct IImage;

struct IScheme : public IBaseInterface
{
	virtual const char *GetResourceString(const char *stringName) = 0;
	virtual IBorder *GetBorder(const char *borderName) = 0;
	virtual HFont GetFont(const char *fontName, bool proportional = false) = 0;
	virtual Color GetColor(const char *colorName, Color defaultColor) = 0;
};

struct ISchemeManager: public IBaseInterface
{
	virtual HScheme LoadSchemeFromFile(const char *fileName, const char *tag) = 0;
	virtual void ReloadSchemes() = 0;
	virtual HScheme GetDefaultScheme() = 0;
	virtual HScheme GetScheme(const char *tag) = 0;
	virtual IImage *GetImage(const char *imageName, bool hardwareFiltered) = 0;
	virtual HTexture GetImageID(const char *imageName, bool hardwareFiltered) = 0;
	virtual IScheme *GetIScheme(HScheme scheme) = 0;
	virtual void Shutdown(bool full = true) = 0;
	virtual int GetProportionalScaledValue(int normalizedValue) = 0;
	virtual int GetProportionalNormalizedValue(int scaledValue) = 0;
};

}; // namespace vgui3