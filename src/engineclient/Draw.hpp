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

// TODO: CRenderer2D?
class CDrawer
{
public:
	void Init();
	
	void DrawChar(int x, int y, int num);
	void DrawDebugChar(char num);
	
	void DrawSubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height);
	void DrawPic(int x, int y, qpic_t *pic);
	
	void DrawTransPic(int x, int y, qpic_t *pic);
	void DrawTransPicTranslate(int x, int y, qpic_t *pic, byte *translation);
	
	void DrawConsoleBackground(int lines);
	
	void BeginDisc();
	void EndDisc();
	
	void DrawTileClear(int x, int y, int w, int h);
	
	void FillScreen(int x, int y, int w, int h, int c);
	void FadeScreen();
	
	void DrawString(int x, int y, const char *str);
	void DrawAltString(int x, int y, const char *str);
	
	qpic_t *GetPicFromWad(const char *name);
	qpic_t *CachePic(const char *path);
	
	void DrawCrosshair();
};