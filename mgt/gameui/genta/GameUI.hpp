/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1999-2005 Id Software, Inc.
 * Copyright (C) 2018-2019 BlackPhrase
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

#pragma once

#include "gameui/IGameUI.hpp"

class CGameUI final : public IGameUI
{
public:
	CGameUI();
	~CGameUI();
	
	void Init(CreateInterfaceFn *factories, int count) override;
	void Start(/*cl_enginefunc_t*/ struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void /*IBaseSystem*/ *system) override;
	void Shutdown() override;

	int Activate() override;
	int ActivateDemoUI() override;

	void RunFrame() override;
	
	void Hide() override;

	bool IsActive() const override;
	
	int HasExclusiveInput() override;

	void ConnectToServer(const char *game, int IP, int port) override;
	void DisconnectFromServer() override;

	void LoadingStarted(const char *resourceType, const char *resourceName) override;
	void LoadingFinished(const char *resourceType, const char *resourceName) override;

	void StartProgressBar(const char *progressType, int progressSteps) override;
	int ContinueProgressBar(int progressPoint, float progressFraction) override;
	void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason) override;

	int SetProgressBarStatusText(const char *statusText) override;

	void SetSecondaryProgressBar(float progress) override;
	void SetSecondaryProgressBarText(const char *statusText) override;

	void OnDisconnectFromServer(int eSteamLoginFailure, const char *username) override;
private:
	typedef struct _tag_menuframework
	{
		void Menu_AddItem(void *item );
		
		void Menu_AdjustCursor(int dir );
		
		void Menu_Draw();
		
		void *Menu_ItemAtCursor();
		
		sfxHandle_t Menu_ActivateItem(menucommon_s* item );
		
		void Menu_SetCursor(int cursor );
		void Menu_SetCursorToItem(void* ptr );
		
		sfxHandle_t Menu_DefaultKey(int key );

		int	cursor;
		int cursor_prev;

		int	nitems;
		void *items[MAX_MENUITEMS];

		void (*draw) (void);
		sfxHandle_t (*key) (int key);

		bool	wrapAround;
		bool	fullscreen;
		bool	showlogo;
	} menuframework_s;

	typedef struct MenuElement // MenuItem
	{
	public:
		MenuElement(int type, const char *name, unsigned int flags, void (*callback)( void *self, int event ), int id);
		
		void SetPos(int x, int y);
		
		void (*callback)( void *self, int event );
		void (*statusbar)( void *self );
		void (*ownerdraw)( void *self );

		enum class Type : int
		{
			Null = 0,
			Slider,
			MTYPE_ACTION,
			MTYPE_SPINCONTROL,
			MTYPE_FIELD,
			RadioButton,
			Bitmap,
			Text,
			ScrollList,
			MTYPE_PTEXT,
			MTYPE_BTEXT
		};
		
		Type type;
		const char *name;
		int	id;
		int x, y;
		int left;
		int	top;
		int	right;
		int	bottom;
		menuframework_s *parent;
		int menuPosition;
		unsigned int flags;
	} menucommon_s;

	// NOTE: from uiStatic_t
	int					frametime;
	int					realtime;
	int					cursorx;
	int					cursory;
	int					menusp;
	
	menuframework_s*	activemenu;
	menuframework_s*	stack[MAX_MENUDEPTH];
	
	glconfig_t			glconfig;
	
	bool			debug;
	
	qhandle_t			whiteShader;
	qhandle_t			menuBackShader;
	qhandle_t			menuBackNoLogoShader;
	qhandle_t			charset;
	qhandle_t			charsetProp;
	qhandle_t			charsetPropGlow;
	qhandle_t			charsetPropB;
	qhandle_t			cursor;
	qhandle_t			rb_on;
	qhandle_t			rb_off;
	
	float				scale;
	float				bias;
	
	bool			demoversion;
	bool			firstdraw;
	
	bool mbActive{false};
};