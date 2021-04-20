/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020 BlackPhrase
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
/// @brief physics world interface

#pragma once

#include "CommonTypes.hpp"

class idVec3;
interface IPhysicsBody;

interface IPhysicsWorld
{
	///
	virtual void Simulate(float afTime, float afTimeStep) = 0;
	
	///
	virtual IPhysicsBody *CreateBody() = 0;
	
	///
	virtual void DestroyBody(IPhysicsBody *apBody) = 0;
	
	///
	virtual void SetGravity(const idVec3 &avGravity) = 0;
	
	
	///
	virtual const idVec3 &GetGravity() const = 0;
	
	///
	virtual struct pmtrace_s *TraceLine(float *start, float *end, int flags, int usehull, int ignore_pe) // PM_TraceLine
};