#include "quakedef.h"
#include "Keys.hpp"

void CKeys::Init()
{
	Key_Init();
};

void CKeys::ClearStates()
{
	Key_ClearStates();
};

void CKeys::Key_Event(int key, bool down)
{
	Key_Event(key, down);
};

void CKeys::SetBinding(int keynum, const char *binding)
{
	Key_SetBinding(keynum, binding);
};

void CKeys::WriteBindings(FILE *f)
{
	Key_WriteBindings(f);
};