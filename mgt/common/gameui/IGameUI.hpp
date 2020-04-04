/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2015-2018, 2020 BlackPhrase
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
/// @brief Game UI module interface

#pragma once

#include "CommonTypes.hpp"
#include "Interface.hpp"

constexpr auto MGT_GAMEUI_INTERFACE_VERSION{"MGTGameUI001Alpha"};

using cl_enginefuncs_t = struct cl_enginefuncs_s;

interface IGameUI : public IBaseInterface
{
	///
	virtual bool Init(CreateInterfaceFn *apFactories, int anCount) = 0;
	
	///
	virtual void Shutdown() = 0;
	
	// connect
	// postinit
	
	///
	virtual void Start(cl_enginefuncs_t *apEngineFuncs, int anInterfaceVersion, void *apSystem) = 0; // IBaseSystem
	
	///
	virtual int ActivateGameUI() = 0;
	
	///
	virtual int ActivateDemoUI() = 0;
	
	virtual void RunFrame() = 0;
	
	//virtual void Update(float afTimeStep) = 0;
	//virtual void PostUpdate() = 0;
	
	///
	virtual void SetActive(bool abActive) = 0;
	
	///
	virtual bool IsActive() const = 0;
	
	///
	virtual int HasExclusiveInput() = 0;
	
	///
	virtual void ConnectToServer(const char *asGame, int anIP, int anPort) = 0;
	
	///
	virtual void DisconnectFromServer() = 0;
	
	///
	virtual void Hide() = 0;
	
	///
	virtual void LoadingStarted(const char *asResourceType, const char *asResourceName) = 0;
	
	///
	virtual void LoadingFinished(const char *asResourceType, const char *asResourceName) = 0;
	
	///
	virtual void StartProgressBar(const char *asProgressType, int anProgressSteps) = 0;
	
	///
	virtual int ContinueProgressBar(int anProgressPoint, float afProgressFraction) = 0;
	
	///
	virtual void StopProgressBar(bool abError, const char *asFailureReason, const char *asExtendedReason = "") = 0;
	
	///
	virtual int SetProgressBarStatusText(const char *asStatusText) = 0;
	
	///
	virtual void SetSecondaryProgressBar(float afProgress) = 0;
	
	///
	virtual void SetSecondaryProgressBarText(const char *asStatusText) = 0;
};