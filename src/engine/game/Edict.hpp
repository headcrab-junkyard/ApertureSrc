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

class CEdict
{
public:
	edict_t *Alloc();
	void Free(edict_t *ed);

	/// returns a copy of the string allocated from the server's string heap
	char *NewString(const char *string);

	void Print(edict_t *ed);
	void Write(FILE *f, edict_t *ed);
	
	char *ParseEdict(char *data, edict_t *ent);

	void WriteGlobals(FILE *f);
	void ParseGlobals(char *data);

	void LoadFromFile(char *data);

	void PrintEdicts();
	void PrintNum(int ent);

	//eval_t *GetEdictFieldValue(edict_t *ed, const char *field);
};