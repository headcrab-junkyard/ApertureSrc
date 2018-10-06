/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "quakedef.h"
#include "engine/ICmdProcessor.hpp"
#include "engine/ICmdRegistry.hpp"

extern ICmdProcessor *gpCmdProcessor;
extern ICmdRegistry *gpCmdRegistry;

void Cbuf_AddText(const char *text)
{
	gpCmdProcessor->QueCmd(text, ICmdProcessor::InsertMode::Back);
};

void Cbuf_Execute()
{
	gpCmdProcessor->ExecQue();
};

void Cmd_AddCommand(const char *name, xcommand_t afnCallback)
{
	gpCmdRegistry->Add(name, afnCallback);
};

const char *Cmd_CompleteCommand(const char *partial)
{
	return gpCmdRegistry->Complete(partial);
};