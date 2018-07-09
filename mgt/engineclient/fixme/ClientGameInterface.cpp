/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 2018 BlackPhrase
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
/// @brief this implementation handles the linking of the engine to the client DLL

// NOTE: The reason why the client dll support is implemented this way is historical
// The GoldSrc is a fork of Source codebase somewhere at the beginning of November 1998
// Current Source is a single dll for both dedicated/listenserver modes (it skips client dll
// loading and does nothing inside these wrapper functions below)

#include "quakedef.h"
#include "game/client/IClientGame.hpp"

static cl_enginefunc_t gEngFuncs; // TODO: name overlap with server-side version? cl_enginefuncs in GS
IClientGame *gpClientGame{nullptr};

void *gpClientDLL{nullptr};

bool LoadClientDLL()
{
	//memcpy(&gEngFuncs, 0, sizeof(cl_enginefunc_t));
	//memcpy(&cl_funcs, 0, sizeof(cldll_func_t));

	gpClientDLL = Sys_LoadModule("mgt/cl_dll/client"); // TODO: FS_LoadLibrary or something else that will load it from the game folder instead of app folder

	if(!gpClientDLL)
		return false;

	auto fnClientDLLFactory{ Sys_GetFactory(gpClientDLL) };

	if(!fnClientDLLFactory)
		return false;

	gpClientGame = (IClientGame*)fnClientDLLFactory(MGT_CLIENTGAME_INTERFACE_VERSION, nullptr);

	if(!gpClientGame->Init(Sys_GetFactoryThis(), &gEngFuncs)) // TODO: So.... Are you alive?
		return false;

	return true;
};

void ClientDLL_Init()
{
	// TODO

	ClientDLL_Shutdown();

	if(!LoadClientDLL())
		return;

	ClientDLL_HudInit();
	ClientDLL_HudVidInit();
	ClientDLL_ClientMoveInit(nullptr /*TODO*/);
};

void ClientDLL_Shutdown()
{
	//if(gpClientGame)
	//gpClientGame->Shutdown();

	if(gpClientDLL)
	{
		Sys_UnloadModule(gpClientDLL);
		gpClientDLL = nullptr;
	};
};

void ClientDLL_HudInit()
{
	if(gpClientGame)
		gpClientGame->HudInit();
};

void ClientDLL_HudVidInit()
{
	if(gpClientGame)
		gpClientGame->HudVidInit();
};

void ClientDLL_UpdateClientData(){
	// TODO
};

void ClientDLL_Frame(double time)
{
	if(gpClientGame)
		gpClientGame->Frame(time);
};

void ClientDLL_HudRedraw(int intermission)
{
	if(gpClientGame)
		gpClientGame->HudRedraw(intermission);
};

void ClientDLL_MoveClient(struct playermove_s *ppmove){
	// TODO
};

void ClientDLL_ClientMoveInit(struct playermove_s *ppmove){
	// TODO
};

char ClientDLL_ClientTextureType(char *name)
{
	// TODO
	return '\0';
};

void ClientDLL_CreateMove(float frametime, struct usercmd_s *cmd, int active){
	// TODO
};

void ClientDLL_ActivateMouse()
{
	if(gpClientGame)
		gpClientGame->IN_ActivateMouse();
};

void ClientDLL_DeactivateMouse()
{
	if(gpClientGame)
		gpClientGame->IN_DeactivateMouse();
};

void ClientDLL_MouseEvent(int mstate)
{
	if(gpClientGame)
		gpClientGame->IN_MouseEvent(mstate);
};

void ClientDLL_ClearStates()
{
	if(gpClientGame)
		gpClientGame->IN_ClearStates();
};

int ClientDLL_IsThirdPerson()
{
	// TODO
	return 0;
};

void ClientDLL_GetCameraOffsets(float *ofs){
	// TODO
};

int ClientDLL_GraphKeyDown()
{
	// TODO
	return 0;
};

struct kbutton_s *ClientDLL_FindKey(const char *name)
{
	// TODO
	return nullptr;
};

void ClientDLL_CAM_Think(){
	// TODO
};

void ClientDLL_IN_Accumulate()
{
	if(gpClientGame)
		gpClientGame->IN_Accumulate();
};

void ClientDLL_CalcRefdef(struct ref_params_s *pparams){
	// TODO
};

int ClientDLL_AddEntity(int type, struct cl_entity_s *ent)
{
	// TODO
	return 0;
};

void ClientDLL_CreateEntities(){
	// TODO
};

void ClientDLL_DrawNormalTriangles(){
	// TODO
};

void ClientDLL_DrawTransparentTriangles(){
	// TODO
};

void ClientDLL_StudioEvent(const struct mstudioevent_s *event, const struct cl_entity_s *entity){
	// TODO
};

void ClientDLL_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed){
	// TODO
};

void ClientDLL_TxferLocalOverrides(struct entity_state_s *state, const struct clientdata_s *client){
	// TODO
};

void ClientDLL_ProcessPlayerState(struct entity_state_s *dst, const struct entity_state_s *src){
	// TODO
};

void ClientDLL_TxferPredictionData(struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd){
	// TODO
};

void ClientDLL_ReadDemoBuffer(int size, byte *buffer){
	// TODO
};

int ClientDLL_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size)
{
	// TODO
	return 0;
};

int ClientDLL_GetHullBounds(int hullnumber, float *mins, float *maxs)
{
	// TODO
	return 0;
};

void ClientDLL_VGui_ConsolePrint(const char *text){
	// TODO
};

int ClientDLL_Key_Event(int down, int keynum, const char *pszCurrentBinding)
{
	if(gpClientGame)
		return gpClientGame->Key_Event(down, keynum, pszCurrentBinding);

	return 0;
};

void ClientDLL_TempEntUpdate(double ft, double ct, double grav, struct tempent_s **ppFreeTE, struct tempent_s **ppActiveTE, int (*addEntity)(struct cl_entity_s *pEntity), void (*playTESound)(struct tempent_s *pTemp, float damp)){
	// TODO
};

struct cl_entity_s *ClientDLL_GetUserEntity(int index)
{
	// TODO
	return nullptr;
};

void ClientDLL_VoiceStatus(int entindex, qboolean bTalking){
	// TODO
};

void ClientDLL_DirectorMessage(int iSize, void *pbuf){
	// TODO
};

void ClientDLL_ChatInputPosition(int *x, int *y){
	// TODO
};