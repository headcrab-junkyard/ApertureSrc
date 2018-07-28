/*
 * This file is part of Magenta Engine
 *
 * Copyright (C) 2018 BlackPhrase
 *
 * Magenta Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Magenta Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Magenta Engine. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "Localize.hpp"

namespace vgui3
{

CLocalize::CLocalize() = default;
CLocalize::~CLocalize() = default;

bool CLocalize::AddFile(IFileSystem *fileSystem, const char *fileName)
{
	return false;
};

void CLocalize::RemoveAll()
{
};

wchar_t *CLocalize::Find(char const *tokenName)
{
	return nullptr;
};

int CLocalize::ConvertANSIToUnicode(const char *ansi, wchar_t *unicode, int unicodeBufferSizeInBytes)
{
	return 0;
};

int CLocalize::ConvertUnicodeToANSI(const wchar_t *unicode, char *ansi, int ansiBufferSize)
{
	return 0;
};

StringIndex_t CLocalize::FindIndex(const char *tokenName)
{
	return 0;
};

void CLocalize::ConstructString(wchar_t *unicodeOuput, int unicodeBufferSizeInBytes, wchar_t *formatString, int numFormatParameters, ...)
{
};

const char *CLocalize::GetNameByIndex(StringIndex_t index)
{
	return "";
};

wchar_t *CLocalize::GetValueByIndex(StringIndex_t index)
{
	return nullptr;
};

StringIndex_t CLocalize::GetFirstStringIndex()
{
	return 0;
};

StringIndex_t CLocalize::GetNextStringIndex(StringIndex_t index)
{
	return 0;
};

void CLocalize::AddString(const char *tokenName, wchar_t *unicodeString, const char *fileName)
{
};

void CLocalize::SetValueByIndex(StringIndex_t index, wchar_t *newValue)
{
};

bool CLocalize::SaveToFile(IFileSystem *fileSystem, const char *fileName)
{
	return false;
};

int CLocalize::GetLocalizationFileCount()
{
	return 0;
};

const char *CLocalize::GetLocalizationFileName(int index)
{
	return "";
};

const char *CLocalize::GetFileNameByIndex(StringIndex_t index)
{
	return "";
};

void CLocalize::ReloadLocalizationFiles()
{
};

void CLocalize::ConstructString/*ByName*/(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, const char *tokenName, KeyValues *localizationVariables)
{
};

void CLocalize::ConstructStringByIndex(wchar_t *unicodeOutput, int unicodeBufferSizeInBytes, StringIndex_t unlocalizedTextSymbol, KeyValues *localizationVariables)
{
};

}; // namespace vgui3