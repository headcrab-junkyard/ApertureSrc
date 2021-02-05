/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020 BlackPhrase
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

#pragma once

#include "soundsystem/ISoundSystem.hpp"

class CSoundSystem : public ISoundSystem
{
public:
	bool Init(void *apMainWindow) override;
	
	void Frame(float afTimeStep) override;
	
	ISoundWorld *CreateWorld() override;
	void DestroyWorld(ISoundWorld *apWorld) override;
	
	void SetActiveWorld(ISoundWorld *apWorld) override {mpActiveWorld = apWorld;}
	ISoundWorld *GetActiveWorld() const override {return mpActiveWorld;}
private:
	void AttachToWindow(void *apWindow){mpWindow = apWindow;}
private:
	ISoundWorld *mpActiveWorld{nullptr};
};