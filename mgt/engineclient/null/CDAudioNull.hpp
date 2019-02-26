/*
*	This file is part of Magenta Engine
*
*	Copyright (C) 1996-1997 Id Software, Inc.
*	Copyright (C) 2017-2018 BlackPhrase
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
/// @brief null ICDAudio interface implementation

#include "icdaudio.h"

class CCDAudioNull final : public ICDAudio
{
public:
	CCDAudioNull() = default;
	~CCDAudioNull() = default;

	int Init() override {return 0;}
	void Shutdown() override {}

	void Frame() override {} // TODO: was Update

	void Pause() override {}
	void Resume() override {}
private:
	void Play(byte track, bool looping); // override
	void Stop(); // override
};