/// @file

#include "quakedef.h"
#include "ModelManager.hpp"

EXPOSE_SINGLE_INTERFACE(CModelManager, IModelManager, MGT_MODELMANAGER_INTERFACE_VERSION);

CModelManager::CModelManager() = default;
CModelManager::~CModelManager() = default;

void CModelManager::TouchModel(const char *name)
{
	Mod_TouchModel(name);
};

model_t *CModelManager::GetByName(const char *name)
{
	return Mod_ForName(name);
};