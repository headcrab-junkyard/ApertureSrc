/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
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

#pragma once

#include <memory>
#include "engine/ISystem.hpp"

class CSystemEventDispatcher;

class CSystem final : public ISystem
{
public:
	CSystem();
	~CSystem();

	//bool Init() override {return true;}
	//void Shutdown() override {}
	
	void AddEventListener(ISystemEventListener *apListener) override;
	void RemoveEventListener(ISystemEventListener *apListener) override;
	
	void Printf(const char *asMsg, ...) override;
	
	void Error(const char *asMsg, ...) override;
	
	void Quit() override;

	double GetFloatTime() const override;
private:
	std::unique_ptr<CSystemEventDispatcher> mpEventDispatcher;
};

//extern CSystem *gpSystem;