/// @file

#include "quakedef.h"
#include "CmdBuffer.hpp"
#include "system/ISystem.hpp"

/*
=============================================================================

						COMMAND BUFFER

=============================================================================
*/

CCmdBuffer::CCmdBuffer(ISystem *apSystem) : mpSystem(apSystem)
{
	Init();
};

CCmdBuffer::~CCmdBuffer()
{
	delete cmd_text;
};

/*
============
Cbuf_AddText

Adds command text at the end of the buffer
============
*/
void CCmdBuffer::AddText(const char *text)
{
	int l{Q_strlen(text)};

	if(cmd_text->cursize + l >= cmd_text->maxsize)
	{
		mpSystem->Printf("Cbuf_AddText: overflow\n");
		return;
	};

	cmd_text->Write(text, Q_strlen(text));
};

/*
============
Cbuf_InsertText

Adds command text immediately after the current command
Adds a \n to the text
FIXME: actually change the command buffer to do less copying
============
*/
void CCmdBuffer::InsertText(const char *text)
{
	char *temp{nullptr};

	// copy off any commands still remaining in the exec buffer
	int templen{cmd_text->cursize};
	if(templen)
	{
		temp = (char *)Z_Malloc(templen);
		Q_memcpy(temp, cmd_text->data, templen);
		cmd_text->Clear();
	};

	// add the entire text of the file
	AddText(text);

	// add the copied off data
	if(templen)
	{
		cmd_text->Write(temp, templen);
		Z_Free(temp);
	};
};

/*
============
Cbuf_Execute
============
*/
void CCmdBuffer::Execute()
{
	int i;
	char *text;
	char line[1024];
	int quotes;

	while(cmd_text->cursize)
	{
		// find a \n or ; line break
		text = (char *)cmd_text->data;

		quotes = 0;
		for(i = 0; i < cmd_text->cursize; i++)
		{
			if(text[i] == '"')
				quotes++;
			if(!(quotes & 1) && text[i] == ';')
				break; // don't break if inside a quoted string
			if(text[i] == '\n')
				break;
		};

		Q_memcpy(line, text, i);
		line[i] = 0;

		// delete the text from the command buffer and move remaining commands down
		// this is necessary because commands (exec, alias) can insert data at the
		// beginning of the text buffer

		if(i == cmd_text->cursize)
			cmd_text->cursize = 0;
		else
		{
			i++;
			cmd_text->cursize -= i;
			Q_memcpy(text, text + i, cmd_text->cursize);
		};

		// execute the command line
		Cmd_ExecuteString(line, src_command);

		if(cmd_wait)
		{
			// skip out while text still remains in buffer, leaving it
			// for next frame
			--cmd_wait;
			break;
		};
	};
};

/*
============
Cbuf_Init
============
*/
void CCmdBuffer::Init()
{
	cmd_text = new CSizeBuffer(mpSystem, 8192); // space for commands and script files
};