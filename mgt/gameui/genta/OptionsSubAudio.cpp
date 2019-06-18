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
/// @brief sound options menu

#include "OptionsSubAudio.hpp"

#define ART_FRAMEL "menu/art/frame2_l"
#define ART_FRAMER "menu/art/frame1_r"
#define ART_BACK0 "menu/art/back_0"
#define ART_BACK1 "menu/art/back_1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_EFFECTSVOLUME	14
#define ID_MUSICVOLUME		15
#define ID_QUALITY			16
//#define ID_A3D				17
#define ID_BACK				18

static const char *quality_items[] =
{
	"Low",
	"High",
	nullptr
};

COptionsSubAudio::COptionsSubAudio() = default;
COptionsSubAudio::~COptionsSubAudio() = default;


/*
===============
UI_SoundOptionsMenu_Init
===============
*/
void COptionsSubAudio::Init()
{
	int y;

	memset( &soundOptionsInfo, 0, sizeof(soundOptionsInfo) );

	Cache();
	
	soundOptionsInfo.menu.wrapAround = true;
	soundOptionsInfo.menu.fullscreen = true;

	soundOptionsInfo.banner.type		= MTYPE_BTEXT;
	soundOptionsInfo.banner.flags		= QMF_CENTER_JUSTIFY;
	soundOptionsInfo.banner.SetPos(320, 16);
	soundOptionsInfo.banner.string				= "SYSTEM SETUP";
	soundOptionsInfo.banner.color				= color_white;
	soundOptionsInfo.banner.style				= UI_CENTER;

	soundOptionsInfo.framel.name		= ART_FRAMEL;
	soundOptionsInfo.framel.flags		= QMF_INACTIVE;
	soundOptionsInfo.framel.SetPos(0, 78);
	soundOptionsInfo.framel.width				= 256;
	soundOptionsInfo.framel.height				= 329;

	soundOptionsInfo.framer.name		= ART_FRAMER;
	soundOptionsInfo.framer.flags		= QMF_INACTIVE;
	soundOptionsInfo.framer.x			= 376;
	soundOptionsInfo.framer.y			= 76;
	soundOptionsInfo.framer.width				= 256;
	soundOptionsInfo.framer.height				= 334;

	soundOptionsInfo.graphics.type		= MTYPE_PTEXT;
	soundOptionsInfo.graphics.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.graphics.id		= ID_GRAPHICS;
	soundOptionsInfo.graphics.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.graphics.x			= 216;
	soundOptionsInfo.graphics.y			= 240 - 2 * PROP_HEIGHT;
	soundOptionsInfo.graphics.string			= "GRAPHICS";
	soundOptionsInfo.graphics.style				= UI_RIGHT;
	soundOptionsInfo.graphics.color				= color_red;

	soundOptionsInfo.display.type		= MTYPE_PTEXT;
	soundOptionsInfo.display.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.display.id			= ID_DISPLAY;
	soundOptionsInfo.display.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.display.x			= 216;
	soundOptionsInfo.display.y			= 240 - PROP_HEIGHT;
	soundOptionsInfo.display.string				= "DISPLAY";
	soundOptionsInfo.display.style				= UI_RIGHT;
	soundOptionsInfo.display.color				= color_red;

	soundOptionsInfo.sound.type			= MTYPE_PTEXT;
	soundOptionsInfo.sound.flags		= QMF_RIGHT_JUSTIFY;
	soundOptionsInfo.sound.id			= ID_SOUND;
	soundOptionsInfo.sound.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sound.x			= 216;
	soundOptionsInfo.sound.y			= 240;
	soundOptionsInfo.sound.string				= "SOUND";
	soundOptionsInfo.sound.style				= UI_RIGHT;
	soundOptionsInfo.sound.color				= color_red;

	soundOptionsInfo.network.type		= MTYPE_PTEXT;
	soundOptionsInfo.network.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.network.id			= ID_NETWORK;
	soundOptionsInfo.network.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.network.x			= 216;
	soundOptionsInfo.network.y			= 240 + PROP_HEIGHT;
	soundOptionsInfo.network.string				= "NETWORK";
	soundOptionsInfo.network.style				= UI_RIGHT;
	soundOptionsInfo.network.color				= color_red;

	y = 240 - 1.5 * (BIGCHAR_HEIGHT + 2);
	soundOptionsInfo.sfxvolume.name		= "Effects Volume:";
	soundOptionsInfo.sfxvolume.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.sfxvolume.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.sfxvolume.id		= ID_EFFECTSVOLUME;
	soundOptionsInfo.sfxvolume.generic.x		= 400;
	soundOptionsInfo.sfxvolume.generic.y		= y;
	soundOptionsInfo.sfxvolume.minvalue			= 0;
	soundOptionsInfo.sfxvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.musicvolume.name		= "Music Volume:";
	soundOptionsInfo.musicvolume.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.musicvolume.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.musicvolume.id			= ID_MUSICVOLUME;
	soundOptionsInfo.musicvolume.SetPos(400, y);
	soundOptionsInfo.musicvolume.minvalue			= 0;
	soundOptionsInfo.musicvolume.maxvalue			= 10;

	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.quality.generic.type		= MTYPE_SPINCONTROL;
	soundOptionsInfo.quality.generic.name		= "Sound Quality:";
	soundOptionsInfo.quality.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.quality.generic.callback	= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.quality.generic.id			= ID_QUALITY;
	soundOptionsInfo.quality.generic.x			= 400;
	soundOptionsInfo.quality.generic.y			= y;
	soundOptionsInfo.quality.itemnames			= quality_items;
/*
	y += BIGCHAR_HEIGHT+2;
	soundOptionsInfo.a3d.generic.name			= "A3D:";
	soundOptionsInfo.a3d.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	soundOptionsInfo.a3d.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.a3d.generic.id				= ID_A3D;
	soundOptionsInfo.a3d.SetPos(400, y);
*/
	soundOptionsInfo.back.generic.name			= ART_BACK0;
	soundOptionsInfo.back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	soundOptionsInfo.back.generic.callback		= UI_SoundOptionsMenu_Event;
	soundOptionsInfo.back.generic.id			= ID_BACK;
	soundOptionsInfo.back.generic.x				= 0;
	soundOptionsInfo.back.generic.y				= 480-64;
	soundOptionsInfo.back.width					= 128;
	soundOptionsInfo.back.height				= 64;
	soundOptionsInfo.back.focuspic				= ART_BACK1;

	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.banner );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.framel );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.framer );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.graphics );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.display );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sound );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.network );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.sfxvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.musicvolume );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.quality );
//	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.a3d );
	Menu_AddItem( &soundOptionsInfo.menu, ( void * ) &soundOptionsInfo.back );

	soundOptionsInfo.sfxvolume.curvalue = gpEngine->Cvar_VariableValue( "s_volume" ) * 10;
	soundOptionsInfo.musicvolume.curvalue = gpEngine->Cvar_VariableValue( "s_musicvolume" ) * 10;
	soundOptionsInfo.quality.curvalue = !gpEngine->Cvar_VariableValue( "s_compression" );
//	soundOptionsInfo.a3d.curvalue = (int)gpEngine->Cvar_VariableValue( "s_usingA3D" );
};

/*
===============
UI_SoundOptionsMenu_Cache
===============
*/
void COptionsSubAudio::Cache()
{
	gpRender->RegisterShaderNoMip( ART_FRAMEL );
	gpRender->RegisterShaderNoMip( ART_FRAMER );
	gpRender->RegisterShaderNoMip( ART_BACK0 );
	gpRender->RegisterShaderNoMip( ART_BACK1 );
};

/*
=================
UI_SoundOptionsMenu_Event
=================
*/
void COptionsSubAudio::Event( void* ptr, int event )
{
	if( event != QM_ACTIVATED )
		return;

	switch( ((menucommon_s*)ptr)->id )
	{
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;

	case ID_EFFECTSVOLUME:
		gpEngine->Cvar_SetValue( "s_volume", soundOptionsInfo.sfxvolume.curvalue / 10 );
		break;

	case ID_MUSICVOLUME:
		gpEngine->Cvar_SetValue( "s_musicvolume", soundOptionsInfo.musicvolume.curvalue / 10 );
		break;

	case ID_QUALITY:
		if( soundOptionsInfo.quality.curvalue )
		{
			gpEngine->Cvar_SetValue( "s_khz", 22 );
			gpEngine->Cvar_SetValue( "s_compression", 0 );
		}
		else
		{
			gpEngine->Cvar_SetValue( "s_khz", 11 );
			gpEngine->Cvar_SetValue( "s_compression", 1 );
		};
		UI_ForceMenuOff();
		gpEngine->Cmd_ExecuteText( EXEC_APPEND, "snd_restart\n" );
		break;
/*
	case ID_A3D:
		if( soundOptionsInfo.a3d.curvalue )
			gpEngine->Cmd_ExecuteText( EXEC_NOW, "s_enable_a3d\n" );
		else 
			gpEngine->Cmd_ExecuteText( EXEC_NOW, "s_disable_a3d\n" );
		soundOptionsInfo.a3d.curvalue = (int)gpEngine->Cvar_VariableValue( "s_usingA3D" );
		break;
*/
	case ID_BACK:
		UI_PopMenu();
		break;
	};
};