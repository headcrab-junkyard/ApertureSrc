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
/// @brief net api implementation

#include "quakedef.h"
#include "net_api.h"

void NET_InitNetworking(){
	// TODO
};

void NET_Status(struct net_status_s *status){
	// TODO
};

void NET_SendRequest(int context, int request, int flags, double timeout, struct netadr_s *remote_address, net_api_response_func_t response){
	// TODO
};

void NET_CancelRequest(int context){
	// TODO
};

void NET_CancelAllRequests(){
	// TODO
};

net_api_t netapi =
{
  NET_InitNetworking,
  NET_Status,

  NET_SendRequest,
  NET_CancelRequest,
  NET_CancelAllRequests,

  nullptr, //NET_AdrToString, // TODO
  nullptr, //NET_CompareAdr, // TODO
  nullptr, //NET_StringToAdr, // TODO

  nullptr, //Info_ValueForKey, // TODO
  nullptr, //Info_RemoveKey, // TODO
  nullptr //Info_SetValueForKey // TODO
};