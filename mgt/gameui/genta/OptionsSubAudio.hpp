/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1999-2005 Id Software, Inc.
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

#include "Text.hpp"
#include "Bitmap.hpp"
#include "Slider.hpp"
#include "List.hpp"

class COptionsSubAudio
{
public:
	COptionsSubAudio();
	~COptionsSubAudio();
private:
	struct soundOptionsInfo_t
	{
		menuframework_s menu{};

		MenuText banner{};
		MenuBitmap framel{};
		MenuBitmap framer{};

		MenuText graphics{};
		MenuText display{};
		MenuText sound{};
		MenuText network{};

		MenuSlider sfxvolume{};
		MenuSlider musicvolume{};
		MenuList quality{};
		//MenuRadioButton a3d{};

		MenuBitmap back;
	};
	
	soundOptionsInfo_t soundOptionsInfo{};
};