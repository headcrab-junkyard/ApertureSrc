#include "quakedef.h"
#include "Memory.hpp"

void CMemory::Init(void *buf, int size)
{
	Memory_Init(buf, size);
};