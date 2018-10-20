/// @file

#pragma once

#include "engine/ICmdRegistry.hpp"

class CCmdRegistry final : public ICmdRegistry
{
public:
	CCmdRegistry();
	~CCmdRegistry();
	
	void Add(const char *asName, pfnCmdCallback afnCallback) override;
	
	const char *Complete(const char *partial) override;
};