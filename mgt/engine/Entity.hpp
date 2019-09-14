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
/// @brief entity class

#pragma once

#include "engine/IEntity.hpp"

class CEntity final : public IEntity
{
public:
	CEntity(edict_t *apEdict, IGameEntity *apGameEntity);
	~CEntity();
	
	void Think();
	
	void SetGameEntity(IGameEntity *apGameEntity) override {mpGameEntity = apGameEntity;}
	IGameEntity *GetGameEntity() const override {return mpGameEntity;}
	
	void SetOrigin(const float *org) override;
	void SetSize(const float *mins, const float *maxs) override;
	void SetModel(const char *asName) override;
	
	//void MoveToOrigin() override;
	int WalkMove(float yaw, float dist, int nMode) override;
	
	void ChangeYaw() override;
	void ChangePitch() override;
	
	int IsOnFloor() const override;
	int DropToFloor() override;
	
	float CheckBottom();
private:
	void SetMinMaxSize(const float *min, const float *max, bool rotate);
	
	edict_t *e{nullptr};
	IGameEntity *mpGameEntity{nullptr};
};