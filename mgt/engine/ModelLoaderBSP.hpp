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
/// @brief brush (bsp) model loader

#pragma once

#include "engine/IModelLoader.hpp"

interface ISystem;

class CModelLoaderBSP final : public IModelLoader
{
public:
	CModelLoaderBSP(ISystem *apSystem) : mpSystem(apSystem){}
	
	bool IsExtSupported(const char *asExt) const override;
	
	IModel *TryLoad(const char *asName) override;
private:
	void Mod_LoadBrushModel(model_t *model, void *buffer);
	
	void CalcSurfaceExtents(msurface_t *s);
	
	void Mod_LoadTextures(lump_t *l);
	void Mod_LoadVertexes(lump_t *l);
	void Mod_LoadSubmodels(lump_t *l);
	void Mod_LoadEdges(lump_t *l);
	void Mod_LoadTexinfo(lump_t *l);
	void Mod_LoadFaces(lump_t *l);
	void Mod_LoadNodes(lump_t *l);
	void Mod_LoadLeafs(lump_t *l);
	void Mod_LoadClipnodes(lump_t *l);
	void Mod_LoadMarksurfaces(lump_t *l);
	void Mod_LoadSurfedges(lump_t *l);
	void Mod_LoadPlanes(lump_t *l);
	
	ISystem *mpSystem{nullptr};
};