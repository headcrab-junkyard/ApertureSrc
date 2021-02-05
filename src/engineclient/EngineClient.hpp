/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020-2021 BlackPhrase
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

#include "engineclient/IEngineClient.hpp"

class CVideo;
class CInput;
class CSound;

class CScreen;
class CView;
class CDraw;

class CWADCache;

class CKeys;

interface IClientGame;

class CEngineClient final : public IEngineClient
{
public:
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	bool PreFrame() const override;
	void Frame() override;
private:
	void LocalInit();
	
	void SendKeyEvents();
	
	void UpdateScreen();

	void InitGameModule();
	void ShutdownGameModule();
	
	void WriteConfig();
private:
	std::unique_ptr<CVideo> mpVideo;
	std::unique_ptr<CInput> mpInput;
	std::unique_ptr<CSound> mpSound;
	
	std::unique_ptr<CScreen> mpScreen;
	std::unique_ptr<CView> mpView;
	std::unique_ptr<CDraw> mpDraw;
	
	std::unique_ptr<CWADCache> mpWADCache;
	
	std::unique_ptr<CKeys> mpKeys;

	IClientGame *mpClientGame{nullptr};
	
	void *mpClientGameModule{nullptr};
};