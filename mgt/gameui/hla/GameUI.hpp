/*
 * This file is part of OGSNext Engine
 *
 * Copyright (C) 2018-2020 BlackPhrase
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

#pragma once

#include "gameui/IGameUI.hpp"

class CGameUI final : public IGameUI
{
public:
	CGameUI();
	~CGameUI();
	
	void Init(CreateInterfaceFn *factories, int count) override;
	void Start(/*cl_enginefunc_t*/ struct cl_enginefuncs_s *engineFuncs, int interfaceVersion, void /*IBaseSystem*/ *system) override;
	void Shutdown() override;

	int Activate() override;
	int ActivateDemoUI() override;

	void RunFrame() override;
	
	void Hide() override;

	bool IsActive() const override;
	
	int HasExclusiveInput() override;

	void ConnectToServer(const char *game, int IP, int port) override;
	void DisconnectFromServer() override;

	void LoadingStarted(const char *resourceType, const char *resourceName) override;
	void LoadingFinished(const char *resourceType, const char *resourceName) override;

	void StartProgressBar(const char *progressType, int progressSteps) override;
	int ContinueProgressBar(int progressPoint, float progressFraction) override;
	void StopProgressBar(bool bError, const char *failureReason, const char *extendedReason) override;

	int SetProgressBarStatusText(const char *statusText) override;

	void SetSecondaryProgressBar(float progress) override;
	void SetSecondaryProgressBarText(const char *statusText) override;

	void OnDisconnectFromServer(int eSteamLoginFailure, const char *username) override;
private:
	bool mbActive{false};
};