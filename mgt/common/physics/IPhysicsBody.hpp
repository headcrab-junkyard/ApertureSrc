/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2020, 2023 BlackPhrase
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
/// @brief physics body interface

#pragma once

#include <next/CommonTypes.hpp>

class idVec3;
interface IPhysicsWorld;
interface IPhysicsMaterial;

interface IPhysicsBody
{
	/**
	*/
	//virtual void MoveToOrigin(const idVec3 &avGoal, float afDist, int anMoveType) = 0;
	
	/**
	*/
	//virtual int WalkMove(float afYaw, float afDist, int anMode) = 0;
	
	/**
	*/
	//virtual int IsOnFloor() const = 0;
	
	/**
	*/
	//virtual int DropToFloor() = 0;
	
	///
	virtual void ApplyForce(const idVec3 &avForce) = 0;
	
	// Position/origin
	
	/// NOTE: This will teleport the body
	virtual void SetWorldPosition(const idVec3 &avPosition /*, const idVec3 &avAngles*/) = 0;
	
	///
	virtual const idVec3 &GetWorldPosition() const = 0;
	
	///
	virtual void SetAngles(const idVec3 &avAngles) = 0;
	
	///
	virtual const idVec3 &GetAngles() const = 0;
	
	// Gravity
	
	/// Enable/disable gravity for this body
	virtual void SetGravityActive(bool abState) = 0;
	
	///
	virtual void IsGravityActive() const = 0;
	
	// Velocity
	
	///
	virtual void SetLinearVelocity(const idVec3 &avVelocity) = 0;
	
	///
	virtual const idVec3 &GetLinearVelocity() const = 0;
	
	///
	virtual void SetAngularVelocity(const idVec3 &avVelocity) = 0;
	
	///
	virtual const idVec3 &GetAngularVelocity() const = 0;
	
	// Material
	
	///
	virtual void SetMaterial(IPhysicsMaterial *apMaterial) = 0;
	
	///
	virtual IPhysicsMaterial *GetMaterial() const = 0;
	
	// Collisions
	
	/// Enable/disable collisions for this body
	// TODO: EnableCollisions?
	virtual void SetCollisionActive(bool abState) = 0;
	
	///
	// TODO: IsCollisionEnabled?
	virtual bool IsCollisionActive() const = 0;
	
	// Mass accessors
	
	///
	virtual void SetMass(float afMass) = 0;
	
	///
	virtual float GetMass() const = 0;
	
	//
	
	///
	virtual IPhysicsWorld *GetWorld() const = 0;
};