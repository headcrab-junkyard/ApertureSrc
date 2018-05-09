/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2015-2018 BlackPhrase
*
*	Magenta Engine is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
*	(at your option) any later version.
*
*	Magenta Engine is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*	GNU General Public License for more details.
*
*	You should have received a copy of the GNU General Public License
*	along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief interface that game module exposes to the engine

#pragma once

#include "common/CommonTypes.hpp"
#include "common/Interface.hpp"

constexpr auto MGT_GAME_INTERFACE_VERSION{"MGTGame001Alpha"};

interface IGame : public IBaseInterface
{
	/**
	* Initialize the game for the first time
	*/
	virtual bool Init(CreateInterfaceFn afnModuleFactory) = 0;
	
	/// Shut down the entire game
	virtual void Shutdown() = 0;
	
	/**
	*/
	virtual void Update(float afTimeStep) = 0;
	
	/// @return Current game title
	virtual const char *GetDescription() const = 0;
};