/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2016-2018 BlackPhrase
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
/// @brief game client interface

#pragma once

#include "CommonTypes.hpp"

interface IEntity;

interface IGameClient
{
	///
	virtual void Disconnect(const char *asReason, ...) = 0;
	
	/**
	* Returns the server assigned userid for this player
	* Useful for logging frags, etc
	* Returns -1 if the edict couldn't be found in the list of players
	*
	* @return
	*/
	virtual int GetUserID() const = 0;
	
	/// Useful for logging frags, etc
	/// @return the server assigned WONid for this player or -1 if error
	virtual uint GetWONID() const = 0;
	
	/// @return Player's authentication ID
	virtual const char *GetAuthID() const = 0;
	
	/// @return Player's network ID as string
	//virtual const char *GetNetworkIDString() const = 0;
	
	/**
	* Get stats info interface for a client netchannel
	*
	* @return
	*/
	//virtual INetChannelInfo *GetNetInfo() const = 0;
	
	/**
	* Get info about player's ping and packet loss
	*
	* @param
	* @param
	*/
	struct TGameClientNetworkStats
	{
		int mnPing;
		int mnPacketLoss;
	};
	
	virtual void GetNetworkStats(TGameClientNetworkStats &apStats) = 0;
	
	/**
	* Fill in the player info structure for the specified player index (name, model, etc)
	*
	* @param
	* @return
	*/
	//virtual bool GetInfo(player_info_t *apInfo) const = 0;
	
	/**
	* Sets a USERINFO client ConVar for a fakeclient
	*/
	//virtual void SetConVarValue(const char *asConVarName, const char *asValue) = 0;
	
	/**
	* Get a convar keyvalue for s specified client
	*
	* @param
	* @return
	*/
	//virtual const char *GetConVarValue(const char *asName) const = 0;
	
	/**
	* Fade out the client's volume level toward silence (or fadePercent)
	*
	* @param
	* @param
	* @param
	* @param
	*/
	virtual void FadeVolume(float afFadePercent, float afFadeOutSeconds, float afHoldTime, float afFadeInSeconds) = 0;
	
	/**
	* Set client's max movement speed
	*
	* @param
	*/
	virtual void SetMaxSpeed(float afSpeed) = 0;
	
	/**
	* Change client's info key value
	*
	* @param
	* @param
	* @param
	*/
	//virtual void SetInfo(const char *asKey, const char *asValue) = 0;
	//virtual void SetKeyValue(char *asInfoBuffer, char *asKey, char *asValue) = 0;
	
	/**
	* Change a specified player's "view entity" (i.e., use the view entity position/orientation for rendering the client view)
	*/
	virtual void SetViewEntity(const IEntity *apViewEnt) = 0;
	
	/**
	* Set the player's crosshair angle
	*/
	virtual void SetCrosshairAngle(float afPitch, float afYaw) = 0;
	
	/**
	*/
	virtual void QueryCvarValue(const char *asCvarName) = 0;
	
	/**
	*/
	virtual void QueryCvarValueEx(const char *asCvarName, int anRequestID) = 0;
	
	/**
	*/
	//virtual void RunPlayerMove(const Vector3 &avViewAngles, float afForwardMove, float afSideMove, float afUpMove, unsigned short anButtons, byte anImpulse, byte anMSec) = 0;
	
	/**
	* Print a message to the client console
	* Engine callbacks so game dll can print messages to individual clients
	*
	* @param
	* @param
	*/
	enum class PrintType : int
	{
		Console,
		Center,
		Chat
	};
	virtual void Printf(PrintType aeType, const char *asMsg, ...) = 0;
	
	/**
	* Issue the specified command to the specified client (mimics that client typing the command at the console)
	*
	* @param
	* @param
	*/
	virtual void SendCommand(const char *asCmd, ...) = 0;
	
	/// @return Pointer to client's entity
	virtual IEntity *GetEntity() const = 0;
	
	///
	//virtual INetChannel *GetNetChan() const = 0;
	
	///
	//virtual const char *GetName() const = 0;
	
	///
	virtual void SetPhysicsKeyValue(const char *key, const char *value) = 0;
	
	///
	virtual const char *GetPhysicsKeyValue(const char *key) const = 0;
	
	///
	virtual const char *GetPhysicsInfoString() const = 0;
	//virtual IInfoKeyBuffer *GetPhysicsInfoBuffer() = 0; // TODO
	
	/**
	* Update client data
	*
	* @param
	* @param
	* @param
	*/
	//virtual void UpdateData(int abSendWeapons, clientdata_t *apClientData) = 0;
	
	/**
	* Get player's weapon data
	*
	* @param
	* @param
	* @return
	*/
	//virtual int GetWeaponData(weapon_data_t *apWeaponData) = 0;
};