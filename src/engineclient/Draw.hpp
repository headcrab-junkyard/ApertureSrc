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

class CDraw
{
public:
	void Init();
	
	void Character(int x, int y, int num);
	void DebugChar(char num);
	
	void SubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height);
	void Pic(int x, int y, qpic_t *pic);
	
	void TransPic(int x, int y, qpic_t *pic);
	void TransPicTranslate(int x, int y, qpic_t *pic, byte *translation);
	
	void ConsoleBackground(int lines);
	
	void BeginDisc();
	void EndDisc();
	
	void TileClear(int x, int y, int w, int h);
	
	void Fill(int x, int y, int w, int h, int c);
	void FadeScreen();
	
	void String(int x, int y, const char *str);
	void Alt_String(int x, int y, const char *str);
	
	qpic_t *PicFromWad(const char *name);
	qpic_t *CachePic(const char *path);
	
	void Crosshair();
};