/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2017-2018 BlackPhrase
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

#include "quakedef.h"
#include "BaseUI.h"
#include "GameUI/IGameUI.h"
#include "GameUI/IGameConsole.h"

extern IGameUI *gpGameUI;
extern IGameConsole *gpGameConsole;

CBaseUI gBaseUI;
IBaseUI *gpBaseUI{&gBaseUI};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CBaseUI, IBaseUI, BASEUI_INTERFACE_VERSION, gBaseUI);

CBaseUI::CBaseUI() = default;
CBaseUI::~CBaseUI() = default;

void CBaseUI::Initialize(CreateInterfaceFn *factories, int count)
{
/*
	if (!gConfigs.bInitialied)
		Config_Init();

	g_hVGUI2 = (HINTERFACEMODULE)GetModuleHandle("vgui2.dll");

	if(g_hVGUI2)
	{
		auto fnVGUI2CreateInterface{Sys_GetFactory(g_hVGUI2)};
		auto fnEngineCreateInterface{Sys_GetFactoryThis()};

		IKeyValuesSystem *pKeyValuesSystem = (IKeyValuesSystem *)fnVGUI2CreateInterface(KEYVALUESSYSTEM_INTERFACE_VERSION, NULL);

		vgui::IVGui *pVGui = (vgui::IVGui *)fnVGUI2CreateInterface(VGUI_IVGUI_INTERFACE_VERSION, NULL);
		vgui::ISurface *pSurface = (vgui::ISurface *)fnEngineCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, NULL);
		vgui::ISchemeManager *pSchemeManager = (vgui::ISchemeManager *)fnVGUI2CreateInterface(VGUI_SCHEME_INTERFACE_VERSION, NULL);
		vgui::ILocalize *pLocalize = (vgui::ILocalize *)fnVGUI2CreateInterface(VGUI_LOCALIZE_INTERFACE_VERSION, NULL);
		vgui::IInputInternal *pInput = (vgui::IInputInternal *)fnVGUI2CreateInterface(VGUI_INPUT_INTERFACE_VERSION, NULL);
		vgui::IPanel *pPanel = (vgui::IPanel *)fnVGUI2CreateInterface(VGUI_PANEL_INTERFACE_VERSION, NULL);

		KeyCode_InitKeyTranslationTable();
		Panel_InstallHook(pPanel);
		Surface_InstallHook(pSurface);
		SchemeManager_InstallHook(pSchemeManager);
		Input_InstallHook(pInput);
		KeyValuesSystem_InstallHook(pKeyValuesSystem);

		vgui::VGui_LoadEngineInterfaces(fnVGUI2CreateInterface, fnEngineCreateInterface);
	};

	vgui_stencil_test = gEngfuncs.pfnRegisterVariable("vgui_stencil_test", "0", FCVAR_ARCHIVE);
	vgui_message_dialog_modal = engine->pfnRegisterVariable("vgui_message_dialog_modal", "1", FCVAR_ARCHIVE);
	vgui_emulatemouse = engine->pfnGetCvarPointer("vgui_emulatemouse");
*/
};

void CBaseUI::Start(struct cl_enginefuncs_s *engineFuncs, int interfaceVersion)
{
/*
	CreateInterfaceFn factories[2];
	factories[0] = g_pMetaHookAPI->GetEngineFactory();
	factories[1] = Sys_GetFactory(g_hVGUI2);

	vgui::g_pInput->SetIMEWindow(g_hMainWnd);

	g_pVGuiLocalize = (vgui::ILocalize *)(Sys_GetFactoryThis()(VGUI_LOCALIZE_INTERFACE_VERSION, NULL));
	g_pVGuiLocalize->AddFile("resource/valve_%language%.txt", "GAME");

	g_pFileSystem->CreateDirHierarchy("cfg", "DEFAULT_WRITE_PATH");

	//if(1)
	{
		engineFuncs->pfnAddCommand("_setlanguage", SetLanguage_f);
		engineFuncs->pfnAddCommand("_dump_fonttexture", DumpFontTexture_f);
	};
*/
};

void CBaseUI::Shutdown()
{
	// TODO
};

int CBaseUI::Key_Event(int down, int keynum, const char *pszCurrentBinding)
{
	// TODO
	
	//ClientDLL_Key_Event(down, keynum, pszCurrentBinding); // TODO: ????????????????????
	
	return 0;
};

void CBaseUI::CallEngineSurfaceProc(void *hwnd, unsigned int msg, unsigned int wparam, long lparam)
{
	// TODO
};

void CBaseUI::Paint(int x, int y, int right, int bottom)
{
	// TODO
};

void CBaseUI::HideGameUI()
{
	gpGameUI->HideGameUI();
};

void CBaseUI::ActivateGameUI()
{
	gpGameUI->ActivateGameUI();
};

bool CBaseUI::IsGameUIVisible()
{
	return gpGameUI->IsGameUIActive();
};

void CBaseUI::HideConsole()
{
	gpGameConsole->Hide();
};

void CBaseUI::ShowConsole()
{
	gpGameConsole->Activate();
	//gpGameUI->ActivateGameUI();
};