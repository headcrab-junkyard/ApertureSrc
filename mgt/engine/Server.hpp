/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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

#include "usercmd.h"

struct svstats_t
{
	double active;
	double idle;
	int count;
	int packets;

	double latched_active;
	double latched_idle;
	int latched_packets;
};

// MAX_CHALLENGES is made large to prevent a denial
// of service attack that could cycle all of them
// out before legitimate users connected
#define MAX_CHALLENGES 1024

struct challenge_t
{
	netadr_t adr;
	int challenge;
	int time;
};

struct server_static_t
{
	int spawncount; // number of servers spawned since start, used to check late spawns

	int maxclients;
	int maxclientslimit;
	struct client_s *clients;    // [maxclients]
	int serverflags;             // episode completion information
	qboolean changelevel_issued; // cleared when at SV_SpawnServer

	svstats_t stats;

	challenge_t challenges[MAX_CHALLENGES]; // to prevent invalid IPs from connecting
	
	char info[MAX_SERVERINFO_STRING]; // TODO
};

//=============================================================================

enum server_state_t
{
	ss_loading,
	ss_active
};

struct server_t
{
	bool active; // false if only a net client

	bool paused;
	bool loadgame; // handle connections specially

	double time;

	int lastcheck; // used by PF_checkclient
	double lastchecktime;

	char name[64]; // map name
	char startspot[64];
	char modelname[64]; // maps/<name>.bsp, for model_precache[0]
	struct model_s *worldmodel;
	char *model_precache[MAX_MODELS]; // NULL terminated
	struct model_s *models[MAX_MODELS];
	char *sound_precache[MAX_SOUNDS]; // NULL terminated
	const char *lightstyles[MAX_LIGHTSTYLES];
	int num_edicts;
	int max_edicts;
	edict_t *edicts;      // can NOT be array indexed, because
	                      // edict_t is variable sized, but can
	                      // be used to reference the world ent
	server_state_t state; // some actions are only valid during load

	sizebuf_t datagram;
	byte datagram_buf[MAX_DATAGRAM];

	sizebuf_t reliable_datagram; // copied to all clients at end of frame
	byte reliable_datagram_buf[MAX_DATAGRAM];

	sizebuf_t signon;
	byte signon_buf[8192];
};

#define NUM_PING_TIMES 16
#define NUM_SPAWN_PARMS 16

// TODO: temp
#ifndef MAX_INFO_STRING
#define MAX_INFO_STRING 256
#endif

struct client_frame_t
{
	// received from client

	// reply
	double				senttime;
	float				ping_time;
	packet_entities_t	entities;
};

typedef struct client_s
{
	bool active;    // false = client is free
	bool spawned;   // client is fully in game (false = don't send datagrams)
	bool connected; // has been assigned to a client_t, but not in game yet
	bool drop;      // has been told to go to another level
	//int				lossage;			// loss percentage

	bool sendinfo; // at end of frame, send info to all
	                   // this prevents malicious multiple broadcasts

	int userid;                     // identifying number
	char userinfo[MAX_INFO_STRING]; // infostring

	usercmd_t lastcmd; // for filling in big drops and partial predictions
	double localtime;  // of last message

	edict_t *edict; // EDICT_NUM(clientnum+1)
	char name[32];  // for printing to other people
	
	int topcolor;
	int bottomcolor;

	client_frame_t frames[UPDATE_BACKUP]; // updates can be deltad from here
	
	float ping_times[NUM_PING_TIMES];
	int num_pings; // ping_times[num_pings%NUM_PING_TIMES]

	// the datagram is written to after every frame, but only cleared
	// when it is sent out to the client.  overflow is tolerated.
	sizebuf_t datagram;
	byte datagram_buf[MAX_DATAGRAM];

	double connection_started; // or time of disconnect for zombies
	bool send_message;     // set on frames a datagram arived on

	// spawn parms are carried from level to level
	float spawn_parms[NUM_SPAWN_PARMS];

	// client known data for deltas
	int old_frags;

	//===== NETWORK ============
	int chokecount;
	int delta_sequence; // -1 = no compression
	netchan_t netchan;
	
	// MGT
	
	int protocol{0};
} client_t;

//=============================================================================

#define SPAWNFLAG_NOT_EASY 256
#define SPAWNFLAG_NOT_MEDIUM 512
#define SPAWNFLAG_NOT_HARD 1024
#define SPAWNFLAG_NOT_DEATHMATCH 2048

#ifdef QUAKE2
// server flags
#define SFL_EPISODE_1 1
#define SFL_EPISODE_2 2
#define SFL_EPISODE_3 4
#define SFL_EPISODE_4 8
#define SFL_NEW_UNIT 16
#define SFL_NEW_EPISODE 32
#define SFL_CROSS_TRIGGERS 65280
#endif

//============================================================================

extern CConVar teamplay;
extern CConVar skill;
extern CConVar deathmatch;
extern CConVar coop;
extern CConVar fraglimit;
extern CConVar timelimit;

extern server_static_t svs; // persistant server info
extern server_t sv;         // local server

extern client_t *host_client;

extern jmp_buf host_abortserver;

extern double host_time;

extern edict_t *sv_player;

//===========================================================

void SV_Init();
void SV_Shutdown(bool abCrash);

void SV_StartParticle(vec3_t org, vec3_t dir, int color, int count);
void SV_StartSound(edict_t *entity, int channel, const char *sample, int volume, float attenuation);

void SV_DropClient(client_t *drop, bool crash, const char *fmt, ...);

void SV_SendClientMessages();
void SV_ClearDatagram();

int SV_ModelIndex(const char *name);

void SV_SetIdealPitch();

void SV_AddUpdates();

void SV_ClientThink();

void SV_ClientPrintf(client_t *cl, const char *fmt, ...);
void SV_BroadcastPrintf(const char *fmt, ...);

void SV_BroadcastCommand(const char *fmt, ...);

void SV_Frame();

void SV_Physics(double frametime);
bool SV_RunThink(edict_t *ent);

bool SV_CheckBottom(edict_t *ent);
bool SV_movestep(edict_t *ent, vec3_t move, bool relink);

void SV_WriteClientdataToMessage(edict_t *ent, sizebuf_t *msg);

void SV_MoveToGoal(edict_t *ent, float dist);

void SV_RunClients();
void SV_SaveSpawnparms();

void SV_SpawnServer(const char *server, const char *startspot);

//
// sv_main
//

void SV_ExecuteClientMessage(client_t *cl);

enum redirect_t
{
	RD_NONE,
	RD_CLIENT,
	RD_PACKET
};

void SV_BeginRedirect(redirect_t rd);
void SV_EndRedirect();