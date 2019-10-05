/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2019 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief stores all the model loaders inside and redirects model loading queries to them

#pragma once

#include "CommonTypes.hpp"

//class CWorld3D;
//class CMesh;
//class CSprite;

interface IModelLoader;
using tModelLoaderVec = std::vector<IModelLoader*>;

class CModelLoaderHandler
{
public:
	CModelLoaderHandler();
	~CModelLoaderHandler();

	void AddLoader(IModelLoader *apLoader);

	//CWorld3D *LoadWorld3D(const char *asName);
	//CMesh *LoadMesh(const char *asName);
	//CSprite *LoadSprite(const char *asName);
private:
	tModelLoaderVec mvLoaders;
};