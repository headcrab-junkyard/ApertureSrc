#include "quakedef.h"
#include "SizeBuffer.hpp"

void CSizeBuffer::Alloc(sizebuf_t *buf, int startsize)
{
	SZ_Alloc(buf, startsize);
};

void CSizeBuffer::Free(sizebuf_t *buf)
{
	SZ_Free(buf);
};

void CSizeBuffer::Clear(sizebuf_t *buf)
{
	SZ_Clear(buf);
};

void CSizeBuffer::Write(sizebuf_t *buf, const void *data, int length)
{
	SZ_Write(buf, data, length);
};

void CSizeBuffer::Print(sizebuf_t *buf, char *data)
{
	SZ_Print(buf, data);
};

void *CSizeBuffer::GetSpace(sizebuf_t *buf, int length)
{
	return SZ_GetSpace(buf, length);
};