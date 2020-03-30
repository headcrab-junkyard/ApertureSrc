/*
Copyright (C) 2019-2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

typedef struct model_s model_t;

class CModel
{
public:
	//CModel(model_t *apModel);
	
	void *GetExtradata() const; // handles caching

	mleaf_t *PointInLeaf(float *p) const;
	byte *LeafPVS(mleaf_t *leaf) const;
private:
	model_t *model{nullptr};
};