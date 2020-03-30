#include "quakedef.h"
#include "Hunk.hpp"

void *CHunk::Alloc(int size)
{
	return Hunk_Alloc(size);
};

void *CHunk::AllocName(int size, const char *name)
{
	return Hunk_AllocName(size, name);
};

void *CHunk::HighAllocName(int size, const char *name)
{
	return Hunk_HighAllocName(size, name);
};

void *CHunk::TempAlloc(int size)
{
	return Hunk_TempAlloc(size);
};

void CHunk::FreeToHighMark(int mark)
{
	Hunk_FreeToHighMark(mark);
};

void CHunk::FreeToLowMark(int mark)
{
	Hunk_FreeToLowMark(mark);
};

void CHunk::Check()
{
	Hunk_Check();
};

int	CHunk::GetHighMark()
{
	return Hunk_HighMark();
};

int	CHunk::GetLowMark()
{
	return Hunk_LowMark();
};