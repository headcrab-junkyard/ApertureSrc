/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2017-2021 BlackPhrase
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

#include "EngineClient.hpp"

#include "Video.hpp"
#include "Input.hpp"
#include "Sound.hpp"

#include "Screen.hpp"
#include "View.hpp"
#include "Draw.hpp"

#include "Keys.hpp"

#include "ClientState.hpp"

//#include "ClientGame.hpp"

EXPOSE_SINGLE_INTERFACE(CEngineClient, IEngineClient, OGS_ENGINECLIENT_INTERFACE_VERSION);

bool CEngineClient::Init(CreateInterfaceFn afnEngineFactory)
{
	mpSystem = reinterpret_cast<ISystem*>(afnEngineFactory(OGS_SYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpSystem)
		return false;
	
	mpVideo = std::make_unique<CVideo>();
	mpInput = std::make_unique<CInput>();
	mpSound = std::make_unique<CSound>();
	
	mpScreen = std::make_unique<CScreen>();
	mpView = std::make_unique<CView>();
	mpDraw = std::make_unique<CDraw>();
	
	mpKeys = std::make_unique<CKeys>();
	
	mpClientState = std::make_unique<CClientState>();
	
	//mpClientGame = std::make_unique<CClientGame>();
	
	mpClientState->state = ca_disconnected;

	host_basepal = (byte *)COM_LoadHunkFile("gfx/palette.lmp");
	
	if(!host_basepal)
		mpSystem->Error("Couldn't load gfx/palette.lmp");
	
	host_colormap = (byte *)COM_LoadHunkFile("gfx/colormap.lmp");
	
	if(!host_colormap)
		mpSystem->Error("Couldn't load gfx/colormap.lmp");

	mpWADCache->LoadWadFile("gfx.wad");
	mpWADCache->LoadWadFile("fonts.wad");
	
	mpConsole->Init();
	
	mpKeys->Init();
	
	// TODO: CWindowManager::CreateGameWindow(); // IGame::CreateGameWindow
	
	mpView->Init();
	mpChase->Init();
	
#ifndef _WIN32 // on non win32, mouse comes before video for security reasons
	mpInput->Init();
#endif

	mpVideo->Init(host_basepal);
	// TODO: GL_Init() here + no VID_Shutdown in GS (at least for hw)
	
	mpDraw->Init();
	mpScreen->Init();
	mpRenderer->Init();
	
#ifndef _WIN32
	// on Win32, sound initialization has to come before video initialization, so we
	// can put up a popup if the sound hardware is in use
	mpSound->Init();
#else

#ifdef GLQUAKE
	// FIXME: doesn't use the new one-window approach yet
	mpSound->Init();
#endif

#endif // _WIN32
	
	mpCDAudio->Init();
	mpStatusBar->Init();
	mpClientState->Init();
	
#ifdef _WIN32 // on non win32, mouse comes before video for security reasons
	mpInput->Init();
#endif

	mpRenderer->InitTextures();
	
	// GUI should be initialized before the client dll start to use it
	GUI_Startup();

	// Initialize the GameUI module here (TODO: move to vgui init?)
	//M_Init();
	
	LoadGameModule();
	
	LocalInit();
	
	return true;
};

/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void CEngineClient::Shutdown()
{
	// clear sounds
	mpSound->StopAllSounds(true); // S_DisableSounds()
	
	// keep Con_Printf from trying to update the screen
	mpScreen->disabled_for_loading = true; // TODO: revisit

	WriteConfig();
	
	// shutdown the game
	mpClientGame->Shutdown();
	
	UnloadGameModule();
	
	// shutdown UI
	mpGameUI->Shutdown();
	
	GUI_Shutdown();
	
	mpCDAudio->Shutdown();

	mpSound->Shutdown();
	mpInput->Shutdown();
	
	// TODO: CWindowManager::DestroyGameWindow(); // IGame::DestroyGameWindow
	
	//mpClientState->Shutdown(); // TODO: remove?
	
	mpVideo->Shutdown(); // TODO: Not present in GS
};

bool CEngineClient::PreFrame() const
{
	SendKeyEvents();
	
	return true;
};

void CEngineClient::Frame()
{
	static double time1 = 0;
	static double time2 = 0;
	static double time3 = 0;
	
	mpClient->Frame();
	
	mpClientGame->Frame();
	
	// update video
	if(host_speeds.GetValue())
		time1 = mpSystem->GetDoubleTime();

	mpVideo->Update();
	
	ClientDLL_HudRedraw(cl.intermission);
	
	
	if(host_speeds.GetValue())
	{
		time2 = mpSystem->GetDoubleTime();
		
		int pass1 = (time1 - time3) * 1000;
		
		time3 = mpSystem->GetDoubleTime();
		
		int pass2 = (time2 - time1) * 1000;
		int pass3 = (time3 - time2) * 1000;
		
		mpSystem->Printf("%3i tot %3i server %3i gfx %3i snd\n",
		           pass1 + pass2 + pass3, pass1, pass2, pass3);
	};
};

void CEngineClient::LocalInit()
{
	//
	// register our commands
	//
	mpCmd->AddCommand("cmd", Cmd_ForwardToServer_f);
};

/// Perform Key_Event() callbacks until the input que is empty
void CEngineClient::SendKeyEvents()
{
	Sys_SendKeyEvents();
};

void CEngineClient::LoadGameModule()
{
	mpClientGameModule = Sys_LoadModule(va("%s/bin/client", com_gamedir));
	
	if(!mpClientGameModule)
		mpSystem->Error("Failed to load the client dll!");
	
	auto fnClientGameFactory{Sys_GetFactory(mpClientGameModule)};
	
	if(!fnClientGameFactory)
		mpSystem->Error("Failed to get the client game module factory!");
	
	mpClientGame = fnClientGameFactory(mpClientGameModule);
	
	if(!mpClientGame)
		mpSystem->Error("Failed to get the client game interface!");
};

void CEngineClient:UnloadGameModule()
{
	if(!mpClientGameModule)
		return;
	
	if(mpClientGame)
	{
		mpClientGame->Shutdown();
		mpClientGame = nullptr;
	};
	
	Sys_UnloadModule(mpClientGameModule);
	mpClientGameModule = nullptr;
};

/*
===============
Host_WriteConfiguration

Writes key bindings and archived cvars to config.cfg
===============
*/
void CEngineClient::WriteConfig()
{
	if(!host_initialized)
		return;

	auto f{mpFileSystem->OpenPathID(va("%s/config.cfg", com_gamedir), "w")};

	if(!f)
	{
		Con_Printf("Couldn't write config.cfg.\n");
		return;
	};

	f->Printf("// This file is overwritten whenever you change your user settings in the game.\n");
	f->Printf("// Add custom configurations to the file \"userconfig.cfg\".\n\n");
	
	f->Printf("unbindall\n");
	
	Key_WriteBindings(f);
	Cvar_WriteVariables(f);

	// TODO: in_mlook
	// TODO: in_jlook
	
	f->Printf("+mlook\n");
	//f->Printf("+jlook\n"); // TODO

	f->Printf("exec userconfig.cfg\n");
	
	mpFileSystem->CloseFile(f);
};