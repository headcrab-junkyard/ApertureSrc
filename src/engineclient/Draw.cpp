#include "quakedef.h"
#include "Draw.hpp"

void CDrawer::Init()
{
	Draw_Init();
};

void CDrawer::DrawCharacter(int x, int y, int num)
{
	Draw_Character(x, y, num);
};

void CDrawer::DrawDebugChar(char num)
{
	Draw_DebugChar(num);
};

void CDrawer::DrawSubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height)
{
	Draw_SubPic(x, y, pic, srcx, srcy, width, height);
};

void CDrawer::DrawPic(int x, int y, qpic_t *pic)
{
	Draw_Pic(x, y, pic);
};

void CDrawer::DrawTransPic(int x, int y, qpic_t *pic)
{
	Draw_TransPic(x, y, pic);
};

void CDrawer::DrawTransPicTranslate(int x, int y, qpic_t *pic, byte *translation)
{
	Draw_TransPicTranslate(x, y, pic, translation);
};

void CDrawer::DrawConsoleBackground(int lines)
{
	Draw_ConsoleBackground(lines);
};

void CDrawer::BeginDisc()
{
	Draw_BeginDisc();
};

void CDrawer::EndDisc()
{
	Draw_EndDisc();
};

void CDrawer::DrawTileClear(int x, int y, int w, int h)
{
	Draw_TileClear(x, y, w, h);
};

void CDrawer::FillScreen(int x, int y, int w, int h, int c)
{
	Draw_Fill(x, y, w, h, c);
};

void CDrawer::FadeScreen()
{
	Draw_FadeScreen();
};

void CDrawer::DrawString(int x, int y, const char *str)
{
	Draw_String(x, y, str);
};

void CDrawer::DrawAltString(int x, int y, const char *str)
{
	Draw_Alt_String(x, y, str);
};

qpic_t *CDrawer::GetPicFromWad(const char *name)
{
	return Draw_PicFromWad(name);
};

qpic_t *CDrawer::CachePic(const char *path)
{
	return Draw_CachePic(path);
};

void CDrawer::DrawCrosshair()
{
	Draw_Crosshair();
};