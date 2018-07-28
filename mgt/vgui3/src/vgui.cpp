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

namespace vgui3
{

EXPOSE_SINGLE_INTERFACE(CVGui, IVgui, MGT_VGUI3_INTERFACE_VERSION);

CVGui::CVGui() = default;
CVGui::~CVGui() = default;

bool CVGUI::Init(CreateInterfaceFn *factoryList, int numFactories) // TODO: Startup?
{
	// TODO
	// * CEGUI::OpenGLRenderer for r_gl
	// * CEGUI::OpenGL3Renderer for r_gl3

	*gpRenderer = CEGUI::OpenGLRenderer::bootstrapSystem();
};

void CVGUI::Shutdown()
{
	//CEGUI::System::destroy();
	//CEGUI::OpenGLRenderer::destroy(gpRenderer);
	CEGUI::OpenGLRenderer::destroySystem();
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

void CVGui::Frame() // TODO: RunFrame?
{
	CEGUI::System::renderAllGUIContexts();
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

void CVGui::AddTickSignal(VPANEL panel, int intervalMilliseconds = 0)
{
};

void CVGui::RemoveTickSignal(VPANEL panekl)
{
};

void CVGui::PostMessage(VPANEL target, KeyValues *params, VPANEL from, float delaySeconds = 0.0f)
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