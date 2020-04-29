/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2017-2018, 2020 BlackPhrase
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

#include "engineclient/IBaseUI.hpp"

class CBaseUI : public IBaseUI
{
public:
	CBaseUI();
	~CBaseUI();
	
	virtual void Initialize(CreateInterfaceFn *factories, int count) override;
	virtual void Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion)  override;
	virtual void Shutdown() override;
	
	virtual int Key_Event(int down, int keynum, const char *pszCurrentBinding) override;
	
	virtual void CallEngineSurfaceProc(void *hwnd, unsigned int msg, unsigned int wparam, long lparam) override;
	
	virtual void Paint(int x, int y, int right, int bottom) override;
	
	virtual void HideGameUI() override;
	virtual void ActivateGameUI() override;
	
	virtual bool IsGameUIVisible() override;
	
	virtual void HideConsole() override;
	virtual void ShowConsole() override;
};