/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2018, 2020 BlackPhrase
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

#include "CommonTypes.hpp"
#include "tier1/interface.h"

constexpr auto MGT_PHYSICS_INTERFACE_VERSION{"MGTPhysics001Alpha"};

struct IPhysicsWorld;

interface IPhysics : public IBaseInterface
{
	//virtual IPhysicsWorld *CreatePhysicsWorld() = 0;
	//virtual void DestroyPhysicsWorld(IPhysicsWorld *apWorld) = 0;
	
	virtual qboolean RecursiveHullCheck(hull_t *hull, int num, float p1f, float p2f, vec3_t p1, vec3_t p2, trace_t *trace) = 0;
};