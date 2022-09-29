/*
Copyright (C) 2019-2021 BlackPhrase

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

class CSystem;
class CSizeBuffer;

class CCmdBuffer
{
public:
	CCmdBuffer(CSystem *apSystem);
	~CCmdBuffer();
	
	///
	void WaitFrames(uint anFrames){cmd_wait = anFrames;}
	
	/// As new commands are generated from the console or keybindings,
	/// the text is added to the end of the command buffer
	void AddText(const char *asText);

	/// When a command wants to issue other commands immediately, the text is
	/// inserted at the beginning of the buffer, before any remaining unexecuted
	/// commands
	void InsertText(const char *asText);

	/// Pulls off \n terminated lines of text from the command buffer and sends
	/// them through Cmd_ExecuteString.  Stops when the buffer is empty.
	/// Normally called once per frame, but may be explicitly invoked.
	/// Do not call inside a command function!
	void Execute();
private:
	/// Allocates an initial text buffer that will grow as needed
	void Init();
private:
	CSystem *mpSystem{nullptr};
	CSizeBuffer *cmd_text{nullptr};
	
	uint cmd_wait{0};
};