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
/// @brief engine client interface implementation

#pragma once

#include <memory>
#include "engineclient/IEngineClient.hpp"

struct ISystem;
struct IFileSystem;
struct IConsole;

struct ISound;
struct IInput;
struct IGameUI;
//struct ICvarRegistry;
class CConsole;
class CVideo;
class CScreen;
class CView;
class CCDAudio;
class CClient;
class CChase;
class CKey;
class CMenu;
class CClientGame;

class CEngineClient final : public IEngineClient //, public IEngineEventListener
{
public:
	CEngineClient();
	~CEngineClient();

	bool Init(CreateInterfaceFn afnEngineFactory /*, tWinHandle ahWindow*/) override;
	void Shutdown() override;

	void ClearMemory() override;

	bool FilterTime(double frametime) const override;
	
	bool PreFrame() override;
	//void PostFrame();

	void Frame() override;
	//void Update(float afTimeStep);
	//void PostUpdate(float afTimeStep);
	
	void UpdateScreen() override;
	
	//void Render();
	//void PostRender();

	void Printf(const char *message, ...);
	void DPrintf(const char *message, ...);

	void ForwardCmdToServer(const ICmdArgs &apArgs) override;

	void HostEndGame() override;
	void HostError() override;
	void HostServerShutdown() override;
	
	void ConInit() override;
	void ConPrint(const char *msg) override;
	
	bool OnQuit() override;
	
	void OnMap(const ICmdArgs &apArgs) override;
	
	void ConnectToLocalServer(const ICmdArgs &apArgs) override;
	
	bool IsDemoPlayback() override;
	bool CanSaveGame() override;
private:
	void LocalInit();

	void WriteConfig(/*const char *filename*/);
	void WriteVideoConfig();
	void WriteRenderConfig();
	//void WriteOpenGLConfig(); // TODO: move to graphics module?

	void CheckStartupVids();
	//void CreateStartupVids();
	
	void SendKeyEvents();

	ISound *LoadSoundModule();
	IInput *LoadInputModule();
	IGameUI *LoadGameUIModule();
	//IClientGame *LoadClientGameModule();

	//std::unique_ptr<CConsole> mpConsole;
	//std::unique_ptr<CCDAudio> mpCDAudio;
	//std::unique_ptr<CVideo> mpVideo;
	//std::unique_ptr<CScreen> mpScreen;
	//std::unique_ptr<CView> mpView;
	//std::unique_ptr<CKey> mpKey;
	//std::unique_ptr<CMenu> mpMenu;
	//std::unique_ptr<CClient> mpClient;
	//std::unique_ptr<CChase> mpChase;
	//std::unique_ptr<CClientGame> mpClientGame;

	//ICvarRegistry *mpCvarRegistry{nullptr};

	ISystem *mpSystem{nullptr};
	IFileSystem *mpFileSystem{ nullptr };
	IConsole *mpConsole{nullptr};
	
	ISound *mpSound{ nullptr };
	IInput *mpInput{ nullptr };
	IGameUI *mpGameUI{ nullptr };
	//IClientGame *mpClientGame{nullptr};

	bool mbInitialized{ false };
};