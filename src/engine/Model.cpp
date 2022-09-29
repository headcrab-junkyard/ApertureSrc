/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019-2020, 2022 BlackPhrase
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

#include "quakedef.h"
#include "Model.hpp"

void *CModel::GetExtraData() const
{
	return Mod_Extradata(mpData);
};

mleaf_t *CModel::PointInLeaf(float *p) const
{
	return Mod_PointInLeaf(p, mpData);
};

byte *CModel::LeafPVS(mleaf_t *apLeaf) const
{
	return Mod_LeafPVS(apLeaf, mpData);
};