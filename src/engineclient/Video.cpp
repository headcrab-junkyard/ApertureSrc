#include "quakedef.h"
#include "Video.hpp"

void CVideo::Init(unsigned char *palette)
{
	VID_Init(palette);
};

void CVideo::Shutdown()
{
	VID_Shutdown();
};

void CVideo::CheckChanges()
{
	VID_CheckChanges();
};