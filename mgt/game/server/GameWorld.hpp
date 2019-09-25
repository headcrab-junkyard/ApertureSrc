/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2017-2019 BlackPhrase
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

#pragma once

#include "engine/IGameWorld.hpp"

class CGameWorld final : public IGameWorld
{
public:
	CGameWorld();
	~CGameWorld();
	
	edict_t *SpawnEntity();
	void RemoveEntity(edict_t *apEntity);
	
	void ChangeLevel(const char *s1, const char *s2);
	
	void SetLightStyle(/*float*/ int style, const char *val);
	
	void SpawnParticle(float *org, float *dir, float color, float count);
	
	void AmbientSound(float *pos, const char *samp, float vol, float attenuation);
	
	edict_t *FindEntitiesInRadius(float *org, float rad);
private:
};