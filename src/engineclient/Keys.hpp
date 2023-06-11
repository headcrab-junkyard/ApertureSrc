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

#include "CommonTypes.hpp"

interface IFile;

class CKeys
{
public:
	void Init(); // TODO: not present in q3
	
	void ClearStates();
	
	void Key_Event(int key, bool down); // TODO: not present in q3
	
	void SetBinding(int keynum, const char *binding);
	const char *GetBinding(int anKey) const;
	
	void WriteBindings(IFile *apFile); // TODO: WriteBindingsToFile?
	
	bool IsDown(int anKey) const;
};