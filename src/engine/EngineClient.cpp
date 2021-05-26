/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2021 BlackPhrase
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

#include "quakedef.h"
#include "EngineClient.hpp"
#include "engineclient/IEngineClient.hpp"

CEngineClient::CEngineClient(IEngineClient *apEngineClient)
	: mpEngineClient(apEngineClient){}

bool CEngineClient::Init()
{
	if(!mpEngineClient)
		return false;
	
	return mpEngineClient->Init(Sys_GetFactoryThis());
};

void CEngineClient::Shutdown()
{
	if(mpEngineClient)
		mpEngineClient->Shutdown();
};

bool CEngineClient::PreFrame()
{
	if(!mpEngineClient)
		return false;
	
	return mpEngineClient->PreFrame();
};

void CEngineClient::Frame(float afTimeStep)
{
	if(mpEngineClient)
		mpEngineClient->Frame(afTimeStep);
};