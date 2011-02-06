#include "StdAfx.h"
#include "EditorRenderer.h"
#include "Renderable.h"
#include "Win32Window.h"
#include "Globals.h"

EditorRenderer::EditorRenderer(void)
{
}

EditorRenderer::~EditorRenderer(void)
{
}

void EditorRenderer::Render()
{
	for (unsigned i = 0; i < NUM_EDITOR_WINDOWS; ++i)
	{
		ClearClr clr = Globals::g_pWin32Windows[i]->GetClearClr();
		Globals::g_pD3D->SetBackBuffer((eSwapChain)i);
		Globals::g_pD3D->Clear(clr.R, clr.G, clr.B);
		Globals::g_pD3D->DeviceBegin();
		Globals::g_pD3D->SpriteBegin();

		while (!m_RenderQueue[i].empty())
		{
			const Renderable* obj = m_RenderQueue[i].back();
			obj->Render();
			m_RenderQueue[i].pop();
		}

		Globals::g_pD3D->SpriteEnd();
		Globals::g_pD3D->DeviceEnd();
		Globals::g_pD3D->Present(Globals::g_pWin32Windows[i]->GetHWND(), (eSwapChain)i);
	}
}

void EditorRenderer::AddObject(const Renderable* const _obj, eEditorWindows _wnd)
{
	// TODO:: do sorting if necessary:
	m_RenderQueue[_wnd].push(_obj);
}