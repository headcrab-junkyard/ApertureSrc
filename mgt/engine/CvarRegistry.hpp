/// @file

#pragma once

#include "ICvarRegistry.hpp"

class CCvarRegistry final : public ICvarRegistry
{
public:
	CCvarRegistry();
	~CCvarRegistry();
	
	void Add(const char *asName, const char *asDefValue, int anFlags, const char *asDesc) override;
	
	void WriteToFile(IFile *apFile) override;
	
	char *Complete(const char *partial) override;
};