/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2019 BlackPhrase
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

#include "game/server/IGame.hpp"

class CGame : public IGame
{
public:
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	void Frame(double afFrameTime) override;
	
	const char *GetDescription() const override {return "GSClutch";}
};