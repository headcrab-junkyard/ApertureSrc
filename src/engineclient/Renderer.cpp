/// @file

#include "quakedef.h"

#include "Renderer.hpp"

#include "renderer/IRenderer.hpp"

void CRenderer::Init()
{
	//if(!LoadRenderModule("r_gl"))
		//mpRenderer = nullptr; //new CRenderNull();
	
	LoadRendererModule("r_gl");
	
	if(!mpRenderer->Init(Sys_GetFactoryThis())
		mpSystem->Error("Couldn't initialize the renderer module!");
};

void CRenderer::Shutdown()
{
	UnloadRendererModule();
};

void CRenderer::InitTextures()
{
};

/*
bool CRenderer::LoadRendererModule(const char *name)
{
	if(mpRendererLib)
		UnloadRendererModule();
	
	mpRendererLib = Sys_LoadModule(name);
	
	if(!mpRendererLib)
		return false;
	
	auto fnRendererFactory{Sys_GetFactory(mpRendererLib)};
	
	if(!fnRendererFactory)
		return false;
	
	mpRenderer = reinterpret_cast<IRenderer*>fnRendererFactory(MGT_RENDERER_INTERFACE_VERSION, nullptr);
	
	if(!mpRenderer)
		return false;
	
	return true;
};
*/

void CVideo::LoadRendererModule(const char *asName)
{
	UnloadRendererModule();
	
	mpRendererModule = Sys_LoadModule(asName);
	
	if(!mpRendererModule)
		mpSystem->Error("Couldn't load the renderer module (%s)!", asName);
	
	auto fnRendererFactory{Sys_GetFactory(mpRendererModule)};
	
	if(!fnRendererFactory)
		mpSystem->Error("The loaded renderer module (%s) doesn't seem to be a valid module!", asName);
	
	mpRenderer = reinterpret_cast<IRenderer*>(fnRendererFactory(OGS_RENDERER_INTERFACE_VERSION, nullptr));
	
	if(!mpRenderer)
		mpSystem->Error("The loaded renderer module (%s) doesn't seem to have the required interface implemented!", asName);
};

void CRenderer::UnloadRendererModule()
{
	if(mpRenderer)
	{
		mpRenderer->Shutdown();
		mpRenderer = nullptr;
	};
	
	if(mpRendererModule)
	{
		Sys_UnloadModule(mpRendererModule);
		mpRendererModule = nullptr;
	};
};