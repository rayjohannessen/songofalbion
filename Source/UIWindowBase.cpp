#include "stdafx.h"

#include "UIWindowBase.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Map.h"
#include "UIQuickBar.h"

//////////////////////////////////////////////////////////////////////////
//	CUIWindowBase
//////////////////////////////////////////////////////////////////////////
void CUIWindowBase::SetAlpha(short a)				 
{ 
	SetAlphaBits(m_pVariables->m_dwTitleColor, a);
	SetAlphaBits(m_pVariables->m_dwTextColor, a);
	SetAlphaBits(m_pVariables->m_dwFrameClr, a);
	SetAlphaBits(m_pVariables->m_dwBgClr, a);
}
void CUIWindowBase::SetAlphaBits( DWORD& clr, short a )
{
	clr &= (~0xFF000000);
	clr |= (a << 24);
}

//////////////////////////////////////////////////////////////////////////
//	CWindowVariablesBase
//////////////////////////////////////////////////////////////////////////

// CTOR
CWindowVariablesBase::CWindowVariablesBase(const point& srcRectTopLeft, string title, OptionsList& options, 
										   const OptionProps& optionProps, bool canAlpha /*= true*/, const pointf& scale /* = pointf(1.0f, 1.0f) */, 
										   eClosePosition closePos /* = CP_TOP_RIGHT */, DWORD bgClr /*= 0xff000000*/, DWORD textClr /* = YELLOW_WHITE */, 
										   float zPos /* = DEPTH_WNDOPTIONS */, float titleScale /*= 1.2f*/, DWORD titleColor /*= YELLOW_WHITE */,
										   bool centerWindow /* = false */)
	:	
m_bCanAlpha(canAlpha),
m_dwTextColor(textClr), 
m_dwBgClr(bgClr),
m_fZPos(zPos), 
m_fFrameTextZPos(zPos+DEPTH_OS_WNDFRAME),
m_ptImageScale(scale), 
m_strTitle(title), 
m_vOptions(options), 
m_eClosePos(closePos), 
m_bCentered(centerWindow),
m_OptionProps(optionProps),
m_fTitleScale(titleScale),
m_dwTitleColor(titleColor),
m_dwFrameClr(0xFFFFFFFF)
{
	pointf size;
	int numCols = 1;
	unsigned numOptions = options.size();
	int optionHeight = Globals::g_pBitMapFont->GetSize();

	// determine how many columns are needed
	// TODO:: make sure return is really needed...
	int maxWidth;
	numCols = SetRect(numOptions, optionProps, numCols, size, scale, optionHeight, srcRectTopLeft, maxWidth);

	m_nTitleY = m_rRect.top + optionProps.EdgePadding;

	// place the options at the correct positions in the window
	SetOptionPositions(optionProps, optionHeight, numOptions);
}

void CWindowVariablesBase::SetOptionPositions( const OptionProps &optionProps, int optionHeight, unsigned numOptions )
{
	point pos;
	if (m_strTitle.size())
		pos = point(m_rRect.left, m_nTitleY + (int)((float)(optionHeight << 1) * m_fTitleScale * 0.65f) + optionProps.Spacing);
	else // no title
		pos = point(m_rRect.left, m_rRect.top + optionProps.EdgePadding);

	if (numOptions <= optionProps.MaxRows)
	{
		for (unsigned i = 0; i < numOptions; ++i)
		{
			m_vOptions[i].m_Rect = rect(pos, point((int)m_ptImageScale.x, optionHeight));
			pos.y += (optionHeight + optionProps.Spacing);
			m_vOptions[i].m_nTopBtmOffset = optionProps.TopBtmInputRectOffset;
		}
	} 
	else
	{
		unsigned rowCount = 1;
		int origY = pos.y;
		for (unsigned i = 0; i < numOptions; ++i, ++rowCount)
		{
			m_vOptions[i].m_Rect = rect(pos, point((int)m_ptImageScale.x, optionHeight));
			pos.y += (optionHeight + optionProps.Spacing);
			m_vOptions[i].m_nTopBtmOffset = optionProps.TopBtmInputRectOffset;
			if (rowCount == optionProps.MaxRows)
			{
				rowCount = 0;
				pos.y = origY;
			}
		}
	}
}

int CWindowVariablesBase::SetRect( unsigned numOptions, const OptionProps &optionProps, int numCols, 
								   pointf &size, const pointf &scale, int optionHeight, const point &srcRectTopLeft, int& maxWidth )
{
	// need to find the largest string, that will be used in determining the box width
	maxWidth = m_strTitle.size();
	for (unsigned i = 0; i < numOptions; ++i)
	{
		if (m_vOptions[i].Name.size() > (unsigned)maxWidth)
			maxWidth = m_vOptions[i].Name.size();
	}

	if (m_fTitleScale > scale.x)
		maxWidth *= int((float)Globals::g_pBitMapFont->GetSize() * m_fTitleScale);
	else
		maxWidth *= int((float)Globals::g_pBitMapFont->GetSize() * scale.x);

	// get width
	if (numOptions > optionProps.MaxRows) // need more columns to fit all the options
	{
		if (numOptions % optionProps.MaxRows == 0)
			numCols = numOptions / optionProps.MaxRows;
		else
			numCols += numOptions / optionProps.MaxRows;

		if (scale.x > m_fTitleScale)
			size.x = float(optionProps.EdgePadding * (numCols + 1) + maxWidth * numCols + (optionProps.Spacing << 1)) * scale.x;
		else
			size.x = float(optionProps.EdgePadding * (numCols + 1) + maxWidth * numCols + (optionProps.Spacing << 1)) * m_fTitleScale;
	}
	else
	{
		if (scale.x > m_fTitleScale)
			size.x = float((optionProps.EdgePadding << 1) + maxWidth + (optionProps.Spacing << 1)) * scale.x;
		else
			size.x = float((optionProps.EdgePadding << 1) + maxWidth + (optionProps.Spacing << 1)) * m_fTitleScale;
	}
	// get height
	if (m_strTitle.size())
	{
		size.y = ((optionProps.EdgePadding << 1) + 
			((numOptions + 1) * optionProps.Spacing) + 
			(numOptions * optionHeight) +
			(optionHeight << 1)) * scale.y;
	}
	else	// no title
	{
		size.y = ((optionProps.EdgePadding << 1) + 
			((numOptions - 1) * optionProps.Spacing) + 
			(numOptions * optionHeight)) * scale.y;
	}
	m_ptImageScale = size;

	if (m_bCentered)
	{
		m_rRect = rect(point((Globals::g_pMap->GetScreenWidth() >> 1) - ((int)m_ptImageScale.x >> 1), 
							 (Globals::g_pMap->GetScreenHeight() >> 1) - ((int)m_ptImageScale.y >> 1)),
							 m_ptImageScale);
	}
	else // create rect based on OptionsList and srcRectTopLeft
		m_rRect = rect(srcRectTopLeft.y-(int)m_ptImageScale.y, srcRectTopLeft.y, srcRectTopLeft.x-(int)m_ptImageScale.x, srcRectTopLeft.x); 	
	
	return numCols;
}

//////////////////////////////////////////////////////////////////////////
//	COption
//////////////////////////////////////////////////////////////////////////

CUIWindowBase* COption::ExecuteOptionAction() 
{
	if(m_fpOptionAction) // some options do not execute a function
		m_fpOptionAction(this);
	return NULL;	
}

void COption::Render(float zPos, float scale, DWORD color)
{
	if (m_bIsHovered)
		color = HoveredColor;	// TODO::only set RGB (not alpha) components

// 	rect rDest(m_Rect); rDest.left += 75; rDest.right = 0;
	point strPos = Globals::g_pBitMapFont->DrawStringAutoCenter(Name.c_str(), m_Rect, zPos, scale, color);
	// draw the ability's icon to the left of the string
	if (m_pQBObj)
		Globals::g_pTM->Render(m_pQBObj->ImageID, strPos.x - 42, strPos.y - 5, zPos, 1.0f, 1.0f, &m_pQBObj->SrcRect, 0.0f, 0.0f, 0.0f, color);
}