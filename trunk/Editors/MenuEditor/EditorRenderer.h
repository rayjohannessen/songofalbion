#pragma once

//////////////////////////////////////////////////////////////////////////
// EditorRenderer - Renders any objects added to its render queue 
//					it deals strictly with non-win32 rendering
//////////////////////////////////////////////////////////////////////////
#include "EditorWindowDetails.h"

class Renderable;

class EditorRenderer
{
	typedef queue<const Renderable* const> RenderQueue;

	RenderQueue m_RenderQueue[NUM_EDITOR_WINDOWS];

public:
	EditorRenderer(void);
	~EditorRenderer(void);

	void Render();

	void AddObject(const Renderable* const _obj, eEditorWindows _wnd);
};
