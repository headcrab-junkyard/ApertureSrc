#include "quakedef.h"
#include "View.hpp"

void CView::Init()
{
	V_Init();
};

void CView::RenderView()
{
	V_RenderView();
};

void CView::UpdatePalette()
{
	V_UpdatePalette();
};

float CView::CalcRoll(vec3_t angles, vec3_t velocity)
{
	return V_CalcRoll(angles, velocity);
};