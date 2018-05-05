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
/// @brief engine core component

#pragma once

#include <memory>
#include "core/IEngineCore.hpp"
#include "EngineTypes.hpp"

class CTextConsole;
class CCommandLine;
class CLogger;
class CCmdBuffer;
class CCmdRegistry;
class CCvarRegistry;
class CLogicTimer;
class CUpdater;
class CCmdProcessor;
class CEngineStateMachine;
class CPluginManager;

class CEngineCore final : public IEngineCore
{
public:
	CEngineCore() = default;
	~CEngineCore() = default;
	
	bool Init(const SInitData &apInitData) override;
	void Shutdown() override;
	
	/// Called repeatedly as the foreground thread for rendering and game logic
	bool Frame() override;
	
	void SetState(eEngineState aeState); // TODO: void PushState(IEngineState *apState)
	//void PopState();
	eEngineState GetState() const; // TODO: IEngineState *GetState() const
	
	bool IsInitialized() const override {return mbInitialized;}
	
	// List of methods that probably should be placed somewhere else
/*
	void Render();
	
	void BeginFrame();
	void EndFrame();

	void Error(const tString &asMessage, ...);

	void WriteConfig();
	
	void NewInstance(const char *asName, const char *asFinalMsg);
	
	void AbortCurrentFrame();

	void Print(const char *asMsg);
	
	bool AddPluginToInit(const char *asPath) const;
	IEnginePlugin *LoadPlugin(const char *asPath) const;
	IEnginePlugin *GetPlugin(const char *asName) const;
*/
private:
	SInitData mHostParms{};
	
	std::unique_ptr<CTextConsole> mpTextConsole;
	
	std::unique_ptr<CCommandLine> mpCmdLine;
	std::unique_ptr<CLogger> mpLogger;
	std::unique_ptr<CCmdBuffer> mpCmdBuffer;
	std::unique_ptr<CCmdRegistry> mpCmdRegistry;
	std::unique_ptr<CCvarRegistry> mpCvarRegistry;
	
	//std::unique_ptr<CLogicTimer> mpLogicTimer;
	//std::unique_ptr<CUpdater> mpUpdater;
	
	//std::unique_ptr<CCmdProcessor> mpCmdProcessor;
	
	std::unique_ptr<CEngineStateMachine> mpStateMachine;
	
	//std::unique_ptr<CPluginManager> mpPluginManager;
	
	int mnFrameCount{0};
	
	bool mbInitialized{false}; ///< Can't run or shutdown before init
	bool mbWantClose{false}; ///< Play main loop until this var is false
};