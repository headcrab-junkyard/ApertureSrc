/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020, 2022 BlackPhrase
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

//#include "quakedef.h"
#include "VoiceServer.hpp"

EXPOSE_SINGLE_INTERFACE(CVoiceServer, IVoiceServer, OGS_VOICESERVER_INTERFACE_VERSION);

bool CVoiceServer::SetClientListening(int anReceiver, int anSender, bool abState)
{
	if(anReceiver > 32 || anReceiver < 0) // TODO: find a way to get the actual max players value
		return false;
	
	if(anSender > 32 || anReceiver < 0) // TODO: find a way to get the actual max players value
		return false;
	
	mClientListeningMap[anReceiver][anSender] = abState;
	return true;
};

bool CVoiceServer::GetClientListening(int anReceiver, int anSender) const
{
	if(anReceiver > 32 || anReceiver < 0) // TODO: find a way to get the actual max players value
		return false;
	
	if(anSender > 32 || anReceiver < 0) // TODO: find a way to get the actual max players value
		return false;
	
	return mClientListeningMap[anReceiver][anSender];
};