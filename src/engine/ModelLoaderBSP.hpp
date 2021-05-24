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
/// @brief brush (bsp) model loader

#pragma once

#include "engine/IModelLoader.hpp"

interface ISystem;

class CModelLoaderBSP final : public IModelLoader
{
public:
	CModelLoaderBSP(ISystem *apSystem);
	
	bool IsExtSupported(const char *asExt) const override;
	
	IModel *TryLoad(const char *asName) override;
private:
	void LoadBrushModel(model_t *model, void *buffer);
	
	void CalcSurfaceExtents(msurface_t *s);
	
	void LoadTextures(lump_t *l);
	void LoadVertexes(lump_t *l);
	void LoadSubmodels(lump_t *l);
	void LoadEdges(lump_t *l);
	void LoadTexinfo(lump_t *l);
	void LoadFaces(lump_t *l);
	void LoadNodes(lump_t *l);
	void LoadLeafs(lump_t *l);
	void LoadClipnodes(lump_t *l);
	void LoadMarksurfaces(lump_t *l);
	void LoadSurfedges(lump_t *l);
	void LoadPlanes(lump_t *l);
	
	ISystem *mpSystem{nullptr};
	model_t *loadmodel{nullptr};
	byte *mod_base{nullptr};
};