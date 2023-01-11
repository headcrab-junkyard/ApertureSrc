/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2018, 2020, 2022-2023 BlackPhrase
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
/// @brief physics module interface

#pragma once

#include <next/CommonTypes.hpp>
#include <tier1/interface.h>

constexpr auto OGS_PHYSICS_INTERFACE_VERSION{"OGSPhysics001"};

interface IPhysicsWorld;

interface IPhysics : public IBaseInterface
{
	virtual IPhysicsWorld *CreateWorld() = 0;
	virtual void DestroyWorld(IPhysicsWorld *apWorld) = 0;
	
	//virtual bool RecursiveHullCheck(hull_t *hull, int num, float p1f, float p2f, vec3_t p1, vec3_t p2, trace_t *trace) = 0;
};