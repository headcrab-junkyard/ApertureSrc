/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2015-2019 BlackPhrase
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
/// @brief sprite (spr) model loader

#pragma once

#include "engine/IModelLoader.hpp"

interface ISystem;

class CModelLoaderSPR final : public IModelLoader
{
public:
	CModelLoaderSPR(ISystem *apSystem) : mpSystem(apSystem){}
	
	bool IsExtSupported(const char *asExt) const override;
	
	IModel *TryLoad(const char *asName) override;
private:
	void Mod_LoadSpriteModel(model_t *mod, void *buffer);
	
	void *Mod_LoadSpriteFrame(void *pin, mspriteframe_t **ppframe);
	void *Mod_LoadSpriteGroup(void *pin, mspriteframe_t **ppframe);
	
	ISystem *mpSystem{nullptr};
};