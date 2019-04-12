/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2016-2018 Headcrab Garage
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief physics body interface

#pragma once

#include "common/CommonTypes.hpp"

interface IPhysicsBody
{
	/**
	*/
	virtual void MoveToOrigin(const Vector3 &avGoal, float afDist, int anMoveType) = 0;
	
	/**
	*/
	virtual int WalkMove(float afYaw, float afDist, int anMode) = 0;
	
	/**
	*/
	virtual int IsOnFloor() const = 0;
	
	/**
	*/
	virtual int DropToFloor() = 0;
	
	///
	virtual void ApplyForce(const Vector3 &avForce) = 0;
	
	///
	virtual void SetMass(float afMass) = 0;
	
	///
	virtual float GetMass() const = 0;
protected:
	virtual ~IPhysicsBody() = default;
};