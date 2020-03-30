#include "quakedef.h"
#include "Draw.hpp"

void CDraw::Init()
{
	Draw_Init();
};

void CDraw::Character(int x, int y, int num)
{
	Draw_Character(x, y, num);
};

void CDraw::DebugChar(char num)
{
	Draw_DebugChar(num);
};

void CDraw::SubPic(int x, int y, qpic_t *pic, int srcx, int srcy, int width, int height)
{
	Draw_SubPic(x, y, pic, srcx, srcy, width, height);
};

void CDraw::Pic(int x, int y, qpic_t *pic)
{
	Draw_Pic(x, y, pic);
};

void CDraw::TransPic(int x, int y, qpic_t *pic)
{
	Draw_TransPic(x, y, pic);
};

void CDraw::TransPicTranslate(int x, int y, qpic_t *pic, byte *translation)
{
	Draw_TransPicTranslate(x, y, pic, translation);
};

void CDraw::ConsoleBackground(int lines)
{
	Draw_ConsoleBackground(lines);
};

void CDraw::BeginDisc()
{
	Draw_BeginDisc();
};

void CDraw::EndDisc()
{
	Draw_EndDisc();
};

void CDraw::TileClear(int x, int y, int w, int h)
{
	Draw_TileClear(x, y, w, h);
};

void CDraw::Fill(int x, int y, int w, int h, int c)
{
	Draw_Fill(x, y, w, h, c);
};

void CDraw::FadeScreen()
{
	Draw_FadeScreen();
};

void CDraw::String(int x, int y, const char *str)
{
	Draw_String(x, y, str);
};

void CDraw::Alt_String(int x, int y, const char *str)
{
	Draw_Alt_String(x, y, str);
};

qpic_t *CDraw::PicFromWad(const char *name)
{
	return Draw_PicFromWad(name);
};

qpic_t *CDraw::CachePic(const char *path)
{
	return Draw_CachePic(path);
};

void CDraw::Crosshair()
{
	Draw_Crosshair();
};