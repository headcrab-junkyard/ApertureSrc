/// @file

#pragma once

#include "engine/ICmdArgs.hpp"

class CCmdArgs final : public ICmdArgs
{
public:
	CCmdArgs(const char *text);
	~CCmdArgs();
	
	const char *GetByIndex(int anArg) const override;
	
	int CheckParm(const char *parm) const override;
	
	const char *GetArgValue(const char *asArg) const override;
	
	int GetCount() const override;
	
	const char *ToString() const override;
};