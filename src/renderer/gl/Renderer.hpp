/// @file

#pragma once

#include "renderer/IRenderer.hpp"

class CRenderer final : public IRenderer
{
public:
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
};