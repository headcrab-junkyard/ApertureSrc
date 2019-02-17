/// @file

#pragma once

#define MAX_SENTENCES 1024

void VOX_Init();

void VOX_LoadSound(const char *sentence);

void VOX_ReadSentenceFile(const char *filename);