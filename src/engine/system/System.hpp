/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2021 BlackPhrase
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

#include <memory>

#include "engine/ISystem.hpp"

class CMemory;
class CSystemEventDispatcher;
interface ISystemImpl;

class CSystem /*final*/ : public ISystem
{
public:
	CSystem(ISystemImpl &aImpl);
	~CSystem();
	
	void Init(const char *asCmdLine); // TODO: bool?
	//void Shutdown() override {}
	
	void AddEventListener(ISystemEventListener *apListener) override;
	void RemoveEventListener(ISystemEventListener *apListener) override;
	
	//
	// memory protection
	//
	
	void MakeCodeWriteable(unsigned long startaddr, unsigned long length);

	//
	// system IO
	//
	
	void DebugLog(const char *file, const char *fmt, ...);

	/// send text to the console
	void Printf(const char *asMsg, ...) override;
	
	void DevPrintf(const char *asMsg, ...) override;
	
	void Warning(const char *asMsg, ...) override;
	
	/// an error will cause the entire program to exit
	void Error(const char *asMsg, ...) override; // TODO: FatalError?
	
	void Quit() override;

	double GetFloatTime() const override;
	double GetRealTime() const override;

	/// called to yield for a little bit so as
	/// not to hog cpu when paused or debugging
	void Sleep(int anMilliSecs);

	/// Perform Key_Event() callbacks until the input que is empty
	void SendKeyEvents();
	
	ICmdArgs *GetStartupArgs() const override {return mpCmdLine.get();}
	CMemory *GetMemory() const {return mpMemory.get();}
private:
	void LowFPPrecision();
	void HighFPPrecision();
	void SetFPCW();
private:
	std::unique_ptr<ICmdArgs> mpCmdLine;
	std::unique_ptr<CMemory> mpMemory;
	std::unique_ptr<CSystemEventDispatcher> mpEventDispatcher;
	
	ISystemImpl &mImpl;
};

//extern CSystem *gpSystem;