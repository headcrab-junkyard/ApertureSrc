/// @file

#pragma once

#include "engine/IModelManager.hpp"

class CModelManager final : public IModelManager
{
public:
	CModelManager();
	~CModelManager();
	
	void TouchModel(const char *name) override;
	
	model_t *GetByName(const char *name, int) const override;
};