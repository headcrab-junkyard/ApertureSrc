/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1997-2001 Id Software, Inc.
*	Copyright (C) 2017-2018 BlackPhrase
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
/// @brief render module interface

#include "public/interface.h"

constexpr auto MGT_RENDER_INTERFACE_VERSION{"MGTRender001Alpha"};

//constexpr auto MAX_DLIGHTS{32};
constexpr auto MAX_ENTITIES{128};
constexpr auto MAX_PARTICLES{4096};
//constexpr auto MAX_LIGHTSTYLES{256};

constexpr auto POWERSUIT_SCALE{4.0f};

#define SHELL_RED_COLOR		0xF20000
#define SHELL_GREEN_COLOR	0xD00000
#define SHELL_BLUE_COLOR	0xF30000

#define SHELL_RG_COLOR		0xDC0000
//#define SHELL_RB_COLOR		0x860000
#define SHELL_RB_COLOR		0x680000
#define SHELL_BG_COLOR		0x780000

//ROGUE
#define SHELL_DOUBLE_COLOR	0xDF0000 // 223
#define	SHELL_HALF_DAM_COLOR	0x900000
#define SHELL_CYAN_COLOR	0x720000
//ROGUE

#define SHELL_WHITE_COLOR	0xD70000

#include "common/cl_entity.h"

/*
typedef struct cl_entity_s
{
	struct model_s		*model;			// opaque type outside refresh
	float				angles[3];

	// most recent data
	float				origin[3];		// also used as RF_BEAM's "from"
	int					frame;			// also used as RF_BEAM's diameter

	// previous data for lerping
	float				oldorigin[3];	// also used as RF_BEAM's "to"
	int					oldframe;

	// misc
	float	backlerp;				// 0.0 = current, 1.0 = old
	int		skinnum;				// also used as RF_BEAM's palette index

	int		lightstyle;				// for flashing entities
	float	alpha;					// ignore if RF_TRANSLUCENT isn't set

	struct image_s	*skin;			// NULL for inline skin
	int		flags;

} cl_entity_t;
*/

constexpr auto ENTITY_FLAGS{68};

#include "common/dlight.h"

/*
typedef struct
{
	vec3_t	origin;
	vec3_t	color;
	float	intensity;
} dlight_t;
*/

#include "common/particledef.h"

/*
typedef struct
{
	vec3_t	origin;
	int		color;
	float	alpha;
} particle_t;
*/

//#include "common/lightstyle.h"

/*
typedef struct
{
	float		rgb[3];			// 0.0 - 2.0
	float		white;			// highest of rgb
} lightstyle_t;
*/

/*
struct refdef_t
{
	int	x{0}, y{0}, width{0}, height{0}; // in virtual screen coordinates
	float fov_x{0.0f}, fov_y{0.0f};
	
	vec3_t vieworg{};
	vec3_t viewangles{};
	
	float blend[4]{};			// rgba 0-1 full screen blend
	float time{0.0f};				// time is uesed to auto animate
	
	int rdflags{0};			// RDF_UNDERWATER, etc

	byte *areabits{nullptr};			// if not NULL, only areas with set bits will be drawn

	lightstyle_t *lightstyles{nullptr};	// [MAX_LIGHTSTYLES]

	int			num_entities{0};
	cl_entity_t	*entities{nullptr};

	int			num_dlights{0};
	dlight_t	*dlights{nullptr};

	int			num_particles{0};
	particle_t	*particles{nullptr};
};
*/

//
// these are the functions exported by the refresh module
//
struct IRender : public IBaseInterface
{
	/// called when the library is loaded
	virtual bool	Init( CreateInterfaceFn afnEngineFactory /*void *hinstance, void *wndproc*/ ) = 0;

	/// called before the library is unloaded
	virtual void	Shutdown() = 0;

	///
	virtual void	BeginFrame( float camera_separation ) = 0;
	
	///
	// if the pointers are not NULL, timing info will be returned
	virtual void	EndFrame (/*int *frontEndMsec, int *backEndMsec*/) = 0;
	
	///
	virtual void	RenderFrame (const refdef_t *fd) = 0;
	
	// All data that will be used in a level should be
	// registered before rendering any frames to prevent disk hits,
	// but they can still be registered at a later time
	// if necessary.
	//
	// Skins and images need to be differentiated, because skins
	// are flood filled to eliminate mip map edge errors, and pics have
	// an implicit "pics/" prepended to the name. (a pic name that starts with a
	// slash will not use the "pics/" prefix or the ".pcx" postfix)
	virtual void BeginRegistration(const char *map) = 0;
	virtual struct model_s *RegisterModel(const char *name) = 0;
	//virtual struct image_s *RegisterSkin(const char *name) = 0;
	//virtual struct image_s *RegisterPic(const char *name) = 0;
	
	// EndRegistration will free any remaining data that wasn't registered.
	// Any model_s or skin_s pointers from before the BeginRegistration
	// are no longer valid after EndRegistration.
	virtual void	EndRegistration() = 0;
	
	virtual void	SetSky(const char *name, float rotate, vec3_t axis) = 0;

	virtual void	DrawGetPicSize (int *w, int *h, const char *name) = 0;	// will return 0 0 if not found
	virtual void	DrawPic(int x, int y, const char *name) = 0;
	virtual void	DrawStretchPic (int x, int y, int w, int h, const char *name) = 0;
	virtual void	DrawChar (int x, int y, int c) = 0;
	virtual void	DrawTileClear(int x, int y, int w, int h, const char *name) = 0;
	virtual void	DrawFill(int x, int y, int w, int h, int c) = 0;
	virtual void	DrawFadeScreen() = 0;

	/// Draw images for cinematic rendering (which can have a different palette). Note that calls
	virtual void	DrawStretchRaw(int x, int y, int w, int h, int cols, int rows, byte *data) = 0;

	/// video mode and refresh state management entry points
	virtual void	CinematicSetPalette( const unsigned short *palette) = 0;	// NULL = game palette

	virtual void	AppActivate( bool activate ) = 0;
};

/*
//
// these are the functions imported by the refresh module
//
struct refimport_t
{
	// abort the game
	void	(*Sys_Error) (int errorLevel, const char *str, ...);

	void	(*Cmd_AddCommand) (const char *name, void(*cmd)());
	void	(*Cmd_RemoveCommand) (const char *name);
	
	int		(*Cmd_Argc) ();
	char	*(*Cmd_Argv) (int i);
	
	void	(*Cmd_ExecuteText) (int exec_when, const char *text);

	// print message on the local console
	void	(*Con_Printf) (int print_level, const char *str, ...);

	// files will be memory mapped read only
	// the returned buffer may be part of a larger pak file,
	// or a discrete file from anywhere in the quake search path
	// a -1 return means the file does not exist
	// NULL can be passed for buf to just determine existance
	int		(*FS_LoadFile) (char *name, void **buf);
	void	(*FS_FreeFile) (void *buf);

	// gamedir will be the current directory that generated
	// files should be stored to, ie: "f:\quake\id1"
	char	*(*FS_Gamedir) ();

	virtual bool Cvar_Register(cvar_t *var) = 0;
	virtual cvar_t *Cvar_Create(const char *name, const char *value, int flags) = 0;
	
	cvar_t	*(*Cvar_Get) (const char *name);
	
	cvar_t	*(*Cvar_Set)( const char *name, char *value );
	void	 (*Cvar_SetValue)( char *name, float value );

	bool	(*Vid_GetModeInfo)( int *width, int *height, int mode );
	void		(*Vid_MenuInit)();
	void		(*Vid_NewWindow)( int width, int height );
};
*/