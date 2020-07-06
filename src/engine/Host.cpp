/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2020 BlackPhrase
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

#include "Host.hpp"

bool CHost::Init()
{
	mpMemory->Init();
	mpCmdBuffer->Init();
	mpCmd->Init();
	
	COM_Init();
	
	mpSystem->Init();
	
	InitLocal();
	
	mpNetwork->Init();
	Netchan_Init();
	
	mpProgs->Init();
	mpModelCache->Init();
	
	mpServer->Init();
	
	if(mpEngineClient)
		if(!mpEngineClient->Init())
			return false;
	
	return true;
};

void CHost::Shutdown()
{
	if(mpEngineClient)
		mpEngineClient->Shutdown();
	
	mpNetwork->Shutdown();
	
	mpSystem->Quit();
};

bool CHost::Frame()
{
	if(mpEngineClient)
		if(!mpEngineClient->Frame())
			return false;
	
	return true;
};