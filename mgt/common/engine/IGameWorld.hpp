/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief game world interface

#pragma once

#include "CommonTypes.hpp"

interface IEntity;

interface IGameWorld
{
	///
	virtual IEntity *CreateEntity(const char *asName = "") = 0; // TODO: const char *asName -> (string pool's string) int *asClassName?
	
	///
	virtual void DestroyEntity(IEntity *apEntity) = 0;
	
	///
	virtual IEntity *GetEntityByIndex(int nIndex) const = 0;
	
	///
	virtual IEntity *FindEntityByVars(struct entvars_s *pVars) const = 0; // TODO: should we support/expose that?
	
	///
	virtual int GetEntityCount() const = 0;
	
	///
	virtual void ChangeLevel(const char *mapname, const char *spartspot) = 0;
	
	///
	virtual void ParticleEffect(const float *org, const float *dir, float color, float count) = 0;
	
	///
	virtual void SetLightStyle(int style, const char *value) = 0;
	
	///
	virtual int CreateInstancedBaseline(int nClassName, struct entity_state_s *baseline) = 0;
	
	///
	virtual void CreateStaticDecal(const float *vOrigin, int nDecalIndex, int nEntityIndex, int nModelIndex) = 0;
};

interface IClientWorld
{
	///
	virtual int CreateVisibleEntity(int nType, /*struct cl_entity_s*/ IEntity *pEntity) = 0;
	
	///
	virtual /*struct cl_entity_s*/ ILocalPlayer *GetLocalPlayer() const = 0;
	
	///
	virtual /*struct cl_entity_s*/ IEntity *GetEntityByIndex(int nIndex) const = 0;
	
	///
	virtual void PlaySoundAtOrigin(const char *sSound, float fVolume, float *vOrigin) = 0;
	
	///
	virtual void PlaySoundByName(const char *sSound, float fVolume) = 0;
	
	///
	virtual void PlaySoundByIndex(int nIndex, float fVolume) = 0;
	
	/// read-only access to user info of each player
	/// nPlayerNum should be in the range (1, MaxClients)
	/// @return "" if no value is set or nullptr if player doesn't exist
	virtual const IInfoBuffer *GetUserInfo(int nPlayerNum) const = 0;
	
	///
	virtual void GetPlayerInfo(int nPlayerNum, hud_player_info_t *pInfo) = 0;
	
	///
	virtual bool IsLocalPlayer(int nPlayerNum) const = 0;
};

interface ILocalPlayer
{
	///
	virtual /*struct cl_entity_s*/ IEntity *GetViewModel() const = 0;
	
	///
	virtual float GetMaxSpeed() const = 0;
	
	///
	virtual int IsDucking() const = 0;
	
	///
	virtual /*Set/Get???*/ViewHeight(float *) = 0; // TODO
	
	///
	virtual /*Set/Get???*/Bounds(int hull, float *mins, float *maxs) = 0; // TODO
	
	///
	virtual SetViewAngles(float *vAngles) = 0;
	
	///
	virtual GetViewAngles(float *vAngles) = 0;
	
	///
	virtual void SetWeaponAnim(int nSequence, int nBody) = 0; // TODO: GetCurrentWeapon()->SetAnim?
	
	///
	virtual IInfoBuffer *GetUserInfo() const = 0;
	
	/// read-only physics info access
	virtual const IInfoBuffer *GetPhysicsInfo() const = 0;
	
	/// read-only server info accesss
	virtual const IInfoBuffer *GetServerInfo() const = 0;
};