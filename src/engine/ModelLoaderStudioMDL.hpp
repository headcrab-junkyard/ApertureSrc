/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2019, 2021 BlackPhrase
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
/// @brief (3ds) studio (max) (mdl) model loader

#pragma once

#include "IModelLoader.hpp"

interface ISystem;

class CModelLoaderStudioMDL final : public IModelLoader
{
public:
	CModelLoaderStudioMDL(ISystem *apSystem) : mpSystem(apSystem){}
	
	bool IsExtSupported(const char *asExt) const override;
	
	IModel *TryLoad(const char *asName) override;
private:
	ISystem *mpSystem{nullptr};
};