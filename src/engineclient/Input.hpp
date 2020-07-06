/*
Copyright (C) 2019-2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#pragma once

class CInput
{
public:
	void Init();

	void Shutdown();

	/// Oportunity for devices to stick commands on the script buffer
	void Commands();

	/// Add additional movement on top of the keyboard move cmd
	void Move(usercmd_t *cmd);

#ifdef _WIN32
	/// Restores all button and position states to defaults
	void ClearStates();
#endif
private:
	void LoadInputSystemModule();
	void UnloadInputSystemModule();
};