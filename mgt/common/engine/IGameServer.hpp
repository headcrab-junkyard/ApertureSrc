/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2016-2020 BlackPhrase
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
/// @brief game server interface

#pragma once

#include "CommonTypes.hpp"

constexpr auto OGS_GAMESERVER_INTERFACE_VERSION{"OGSGameServer003"};

interface IGameClient;
interface IGameClientEventListener;

interface IGameServerEventListener
{
	///
	virtual void OnActivate() = 0;
	
	///
	virtual void OnDeactivate() = 0;
};

interface IGameServer : public IBaseInterface
{
	/// Is this a dedicated server?
	/// @return true if the server is in dedicated mode
	virtual bool IsDedicated() const = 0;
	
	///
	virtual uint GetMaxClients() const = 0;
	
	///
	virtual void AddEventListener(IGameServerEventListener *apListener) = 0;
	
	///
	virtual void RemoveEventListener(IGameServerEventListener *apListener) = 0;
	
	///
	virtual void AddClientEventListener(IGameClientEventListener *apListener) = 0;
	
	///
	virtual void RemoveClientEventListener(IGameClientEventListener *apListener) = 0;
	
	///
	virtual IGameClient *GetClientByName(const char *asName) = 0;
	
	///
	virtual IGameClient *GetClientByIndex(int anID) = 0; // -1 for random?
	//virtual IGameClient *GetClient(int anID) = 0;
	
	/**
	* Send a message to all clients
	*
	* @param
	* @param
	*/
	virtual void BroadcastPrintf(const char *asMsg, ...) = 0;
	
	///
	virtual int RegisterUserMsg(const char *sName, int nSize) = 0;
	
	/** Begin the Entity or UserMessage and dispatch to network layer */
	virtual INetMsg *MessageBegin(int anMsgDest, int anMsgType, const float *avOrigin, IEntity *apEntity) = 0;
	
	/** Begin a message from a server side entity to its client side counterpart (func_breakable glass, e.g.) */
	//virtual INetMsg *EntityMessageBegin(int anEntIndex, ServerClass *apEntClass, bool abReliable) = 0;
	
	/** Begin a usermessage from the server to the client dll */
	//virtual INetMsg *UserMessageBegin(IRecipientFilter *apFilter, int anMsgType) = 0;
	
	/** Finish the Entity or UserMessage and dispatch to network layer */
	virtual void MessageEnd(INetMsg *apMsg) = 0;
	
	/**
	* Marks the filename for consistency checking
	* This should be called after precaching the file
	*/
	virtual void ForceExactFile(const char *asName) = 0;
	virtual void ForceModelBounds(const char *asName, const vec3_t &avMins, const vec3_t &avMaxs) = 0;
	
	/**
	*/
	virtual int GetCurrentPlayer() const = 0; // TODO: should be removed
	
	/**
	* Create a bot with the given name
	* @return nullptr if fake client can't be created
	*/
	virtual IGameClient *CreateFakeClient(const char *asName/*, bool abReportFakeClient = true*/) = 0;
	
	///
	virtual IInfoBuffer *GetServerInfo() const = 0; // TODO
	
	///
	virtual IInfoBuffer *GetPhysicsInfo() const = 0; // TODO
};