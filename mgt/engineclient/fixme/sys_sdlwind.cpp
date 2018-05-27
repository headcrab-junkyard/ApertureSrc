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
/// @brief

class CGame
{
public:
	CGame();
	~CGame();

	void CreateGameWindow();

	void PlayStartupVideos();
	void PlayVideoAndWait(const char *name, bool);

	void SleepUntilInput();
};

CGame::CGame() = default;
CGame::~CGame() = default;

void CGame::CreateGameWindow(){};

void CGame::PlayStartupVideos(){};

void CGame::PlayVideoAndWait(const char *name, bool){};

void CGame::SleepUntilInput(){};