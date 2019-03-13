/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
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
/// @brief this implementation handles the linking of the engine to the client DLL

// NOTE: The reason why the client dll support is implemented this way is historical
// The GoldSrc is a fork of Source codebase somewhere at the beginning of November 1998
// Current Source is a single dll for both dedicated/listenserver modes (it skips client dll
// loading and does nothing inside these wrapper functions below)

#include "quakedef.h"
#include "ClientGame.hpp"
#include "game/client/IClientGame.hpp"

static cl_enginefunc_t gEngFuncs; // TODO: name overlap with server-side version? cl_enginefuncs in GS

CClientGame::CClientGame() = default;
CClientGame::~CClientGame() = default;

void CClientGame::Init()
{
	// TODO

	Shutdown();

#ifndef MGT_CLIENTGAME_HARD_LINKED
	if(!LoadModule())
		return;
#endif

	ClientGame_HudInit();
	ClientGame_HudVidInit();
	ClientGame_ClientMoveInit(nullptr /*TODO*/);
};

void CClientGame::Shutdown()
{
	if(mpGame)
		mpGame->Shutdown();

#ifndef MGT_CLIENTGAME_HARD_LINKED
	UnloadModule();
#endif
};

void ClientGame_HudInit()
{
	if(mpGame)
		mpGame->HudInit();
};

void ClientGame_HudVidInit()
{
	if(mpGame)
		mpGame->HudVidInit();
};

void ClientGame_UpdateClientData(){
	// TODO
};

void ClientGame_Frame(double time)
{
	if(mpGame)
		mpGame->Frame(time);
};

void ClientGame_HudRedraw(int intermission)
{
	if(mpGame)
		mpGame->HudRedraw(intermission);
};

void ClientGame_MoveClient(struct playermove_s *ppmove){
	// TODO
};

void ClientGame_ClientMoveInit(struct playermove_s *ppmove){
	// TODO
};

char ClientGame_ClientTextureType(const char *name)
{
	// TODO
	return '\0';
};

void ClientGame_CreateMove(float frametime, struct usercmd_s *cmd, bool active){
	// TODO
};

void ClientGame_ActivateMouse()
{
	if(mpGame)
		mpGame->IN_ActivateMouse();
};

void ClientGame_DeactivateMouse()
{
	if(mpGame)
		mpGame->IN_DeactivateMouse();
};

void ClientGame_MouseEvent(int mstate)
{
	if(mpGame)
		mpGame->IN_MouseEvent(mstate);
};

void ClientGame_ClearStates()
{
	if(mpGame)
		mpGame->IN_ClearStates();
};

int ClientGame_IsThirdPerson()
{
	// TODO
	return 0;
};

void ClientGame_GetCameraOffsets(float *ofs){
	// TODO
};

int ClientGame_GraphKeyDown()
{
	// TODO
	return 0;
};

struct kbutton_s *ClientGame_FindKey(const char *name)
{
	// TODO
	return nullptr;
};

void ClientGame_CAM_Think(){
	// TODO
};

void ClientGame_IN_Accumulate()
{
	if(mpGame)
		mpGame->IN_Accumulate();
};

void ClientGame_CalcRefdef(struct ref_params_s *pparams){
	// TODO
};

int ClientGame_AddEntity(int type, struct cl_entity_s *ent)
{
	// TODO
	return 0;
};

void ClientGame_CreateEntities(){
	// TODO
};

void ClientGame_DrawNormalTriangles(){
	// TODO
};

void ClientGame_DrawTransparentTriangles(){
	// TODO
};

void ClientGame_StudioEvent(const struct mstudioevent_s *event, const struct cl_entity_s *entity){
	// TODO
};

void ClientGame_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed){
	// TODO
};

void ClientGame_TxferLocalOverrides(struct entity_state_s *state, const struct clientdata_s *client){
	// TODO
};

void ClientGame_ProcessPlayerState(struct entity_state_s *dst, const struct entity_state_s *src){
	// TODO
};

void ClientGame_TxferPredictionData(struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd){
	// TODO
};

void ClientGame_ReadDemoBuffer(int size, byte *buffer){
	// TODO
};

int ClientGame_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size)
{
	// TODO
	return 0;
};

int ClientGame_GetHullBounds(int hullnumber, float *mins, float *maxs)
{
	// TODO
	return 0;
};

void ClientGame_VGui_ConsolePrint(const char *text){
	// TODO
};

int ClientGame_Key_Event(bool down, int keynum, const char *pszCurrentBinding)
{
	if(mpGame)
		return mpGame->Key_Event(down, keynum, pszCurrentBinding);

	return 0;
};

void ClientGame_TempEntUpdate(double ft, double ct, double grav, struct tempent_s **ppFreeTE, struct tempent_s **ppActiveTE, int (*addEntity)(struct cl_entity_s *pEntity), void (*playTESound)(struct tempent_s *pTemp, float damp)){
	// TODO
};

struct cl_entity_s *ClientGame_GetUserEntity(int index)
{
	// TODO
	return nullptr;
};

void ClientGame_VoiceStatus(int entindex, bool bTalking){
	// TODO
};

void ClientGame_DirectorMessage(int iSize, void *pbuf){
	// TODO
};

void ClientGame_ChatInputPosition(int *x, int *y){
	// TODO
};

bool CClientGame::LoadModule()
{
	//memcpy(&gEngFuncs, 0, sizeof(cl_enginefunc_t));

	mpClientLib = Sys_LoadModule(va("%s/bin/client", com_gamedir)); // TODO: FS_LoadLibrary or something else that will load it from the game folder instead of app folder

	if(!mpClientLib)
		return false;

	auto fnClientDLLFactory{ Sys_GetFactory(mpClientLib) };

	if(!fnClientDLLFactory)
		return false;

	mpGame = (IClientGame*)fnClientDLLFactory(MGT_CLIENTGAME_INTERFACE_VERSION, nullptr);

	if(!mpGame->Init(Sys_GetFactoryThis(), &gEngFuncs)) // TODO: So.... Are you alive?
		return false;

	return true;
};

void CClientGame::UnloadModule()
{
	if(mpClientLib)
	{
		Sys_UnloadModule(mpClientLib);
		mpClientLib = nullptr;
	};
};