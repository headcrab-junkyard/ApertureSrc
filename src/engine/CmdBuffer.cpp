#include "quakedef.h"
#include "CmdBuffer.hpp"

void CCmdBuffer::Init()
{
	Cbuf_Init();
};

void CCmdBuffer::AddText(const char *text)
{
	Cbuf_AddText(text);
};

void CCmdBuffer::InsertText(const char *text)
{
	Cbuf_InsertText(text);
};

void CCmdBuffer::Execute()
{
	Cbuf_Execute();
};