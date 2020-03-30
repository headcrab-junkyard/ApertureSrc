#include "quakedef.h"
#include "Screen.hpp"

void CScreen::Init()
{
	SCR_Init();
};

void CScreen::Update()
{
	SCR_UpdateScreen();
};

void CScreen::SizeUp()
{
	SCR_SizeUp();
};

void CScreen::SizeDown()
{
	SCR_SizeDown();
};

void CScreen::BringDownConsole()
{
	//SCR_BringDownConsole(); // TODO
};

void CScreen::CenterPrint(const char *str)
{
	SCR_CenterPrint(str);
};

void CScreen::BeginLoadingPlaque()
{
	SCR_BeginLoadingPlaque();
};

void CScreen::EndLoadingPlaque()
{
	SCR_EndLoadingPlaque();
};

int CScreen::ModalMessage(const char *text)
{
	return 0; // SCR_ModalMessage(text); // TODO
};

#ifdef _WIN32
void CScreen::UpdateWholeScreen()
{
	//SCR_UpdateWholeScreen(); // TODO
};
#endif