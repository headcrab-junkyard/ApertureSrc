/// @file

#include "Renderer.hpp"

EXPOSE_SINGLE_INTERFACE(CRenderer, IRenderer, OGS_RENDERER_INTERFACE_VERSION);

bool CRenderer::Init(CreateInterfaceFn afnEngineFactory)
{
	return true;
};

void CRenderer::Shutdown()
{
};