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

#include "VGUI.hpp"

#include <Gwork/Gwork.h>
#include <Gwork/Platform.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>

#ifdef USE_DEBUG_FONT
#   include <Gwork/Renderers/OpenGL_DebugFont.h>
#else
#   include <Gwork/Renderers/OpenGL.h>
#endif

//using namespace vgui3;

namespace vgui3
{

EXPOSE_SINGLE_INTERFACE(CVGui, IVgui, MGT_VGUI3_INTERFACE_VERSION);

CVGui::CVGui() = default;
CVGui::~CVGui() = default;

constexpr auto GWORK_RESOURCE_DIR{"."};

bool CVGui::Init(CreateInterfaceFn *factoryList, int numFactories) // TODO: Startup?
{
	const Gwk::Point screenSize(1024,768);
	
	Gwk::Platform::RelativeToExecutablePaths paths(GWORK_RESOURCE_DIR);

    // Create a Gwork OpenGL Renderer
#ifdef USE_DEBUG_FONT
	renderer = new Gwk::Renderer::OpenGL_DebugFont();
#else
	renderer = new Gwk::Renderer::OpenGL(paths, Gwk::Rect(Gwk::Point(0,0), screenSize));
#endif
    renderer->Init();

    // Create a Gwork skin
    skin = new Gwk::Skin::TexturedBase(renderer);
    skin->Init("DefaultSkin.png");
    skin->SetDefaultFont("OpenSans.ttf", 11);

    // Create a Canvas (it's root, on which all other Gwork panels are created)
    canvas = new Gwk::Controls::Canvas(skin);
    canvas->SetSize(screenSize.x, screenSize.y);
    canvas->SetDrawBackground(true);
    canvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    unit = new TestFrame(canvas);
};

void CVGui::Shutdown()
{
	delete unit;
    delete canvas;
    delete skin;
    delete renderer;
};

void CVGui::Start()
{
};

void CVGui::Stop()
{
};

bool CVGui::IsRunning()
{
};

void CVGui::RunFrame() // TODO: Frame?
{
	canvas->RenderCanvas();
	//Gwk::Platform::Sleep(0);
};

void CVGui::ShutdownMessage(unsigned int shutdownID)
{
};

VPANEL CVGui::AllocPanel()
{
	return 0;
};

void CVGui::FreePanel(VPANEL panel)
{
};

void CVGui::DPrintf(const char *format, ...)
{
};

void CVGui::DPrintf2(const char *format, ...)
{
};

void CVGui::SpewAllActivePanelNames()
{
};

HPanel CVGui::PanelToHandle(VPANEL panel)
{
	return 0;
};

VPANEL CVGui::HandleToPanel(HPanel index)
{
	return 0;
};

void CVGui::MarkPanelForDeletion(VPANEL panel)
{
};

void CVGui::AddTickSignal(VPANEL panel, int intervalMilliseconds)
{
};

void CVGui::RemoveTickSignal(VPANEL panekl)
{
};

void CVGui::PostMessage(VPANEL target, KeyValues *params, VPANEL from, float delaySeconds)
{
};

HContext CVGui::CreateContext()
{
	return 0;
};

void CVGui::DestroyContext(HContext context)
{
};

void CVGui::AssociatePanelWithContext(HContext context, VPANEL pRoot)
{
};

void CVGui::ActivateContext(HContext context)
{
};

void CVGui::SetSleep(bool state)
{
};

bool CVGui::GetShouldVGuiControlSleep()
{
	return false;
};

void CVGui::DispatchMessages()
{
};

}; // namespace vgui3