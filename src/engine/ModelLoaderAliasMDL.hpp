/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2019, 2021 BlackPhrase
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
/// @brief alias (mdl) model loader

#pragma once

#include "engine/IModelLoader.hpp"

interface ISystem;

class CModelLoaderAliasMDL final : public IModelLoader
{
public:
	CModelLoaderAliasMDL(ISystem *apSystem);
	
	bool IsExtSupported(const char *asExt) const override;
	
	IModel *TryLoad(const char *asName) override;
private:
	void LoadAliasModel(model_t *mod, void *buffer);
	
	void *LoadAliasFrame(void *pin, int *pframeindex, int numv, trivertx_t *pbboxmin, trivertx_t *pbboxmax, aliashdr_t *pheader, const char *asName);
	void *LoadAliasGroup(void *pin, int *pframeindex, int numv, trivertx_t *pbboxmin, trivertx_t *pbboxmax, aliashdr_t *pheader, const char *name);
	void *LoadAliasSkin(void *pin, int *pskinindex, int skinsize, aliashdr_t *pheader);
	void *LoadAliasSkinGroup(void *pin, int *pskinindex, int skinsize, aliashdr_t *pheader);
	
	ISystem *mpSystem{nullptr};
};