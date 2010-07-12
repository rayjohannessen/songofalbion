#include "stdafx.h"

#include "UIWindowBase.h"
#include "Globals.h"
#include "BitmapFont.h"
#include "Map.h"
#include "UIQuickBar.h"

//////////////////////////////////////////////////////////////////////////
//	COption
//////////////////////////////////////////////////////////////////////////

void COption::Render(float zPos, float scale, DWORD color, bool IsCurrentlyTransparent)
{
	if (m_bIsHovered && !IsCurrentlyTransparent)	// only change to hovered color if this window is NOT transparent (meaning the mouse is not in the window currently)
		color = HoveredColor;
	else if (IsCurrentlyTransparent)
	{
		// TODO:: add alpha to color
	}
	point strPos = Globals::g_pBitMapFont->DrawStringAutoCenterBox(Name.c_str(), m_Rect.right - m_Rect.left, m_Rect.left, m_Rect.top, zPos, scale, color);
	// draw the ability's icon to the left of the string
	if (m_pQBObj)
		Globals::g_pTM->DrawWithZSort(m_pQBObj->ImageID, strPos.x - 42, strPos.y - 10, zPos, 1.0f, 1.0f, &m_pQBObj->SrcRect);
}

//////////////////////////////////////////////////////////////////////////
//	CWindowVariablesBase
//////////////////////////////////////////////////////////////////////////

// CTOR
CWindowVariablesBase::CWindowVariablesBase(const point& srcRectTopLeft, string title, OptionsList& options, 
										   const OptionProps& optionProps, pointf scale /* = pointf(1.0f, 1.0f) */, 
										   eClosePosition closePos /* = CP_TOP_RIGHT */, DWORD color /* = BLUE */, 
										   float zPos /* = 0.0f */, float titleScale /*= 1.2f*/, DWORD titleColor /*= WHITE*/,
										   bool centerWindow /* = false */)
	:	
m_dwColor(color), 
m_fZPos(zPos), 
m_ptImageScale(scale), 
m_strTitle(title), 
m_vOptions(options), 
m_eClosePos(closePos), 
m_bIsTransparent(false), 
m_bCentered(centerWindow),
m_OptionProps(optionProps),
m_fTitleScale(titleScale),
m_dwTitleColor(titleColor)
{
	pointf size;
	int numCols = 1;
	unsigned numOptions = options.size();
	int optionHeight = Globals::g_pBitMapFont->GetSize().y;

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
		pos = point(m_rRect.left, m_nTitleY + (int)((float)(optionHeight << 1) * m_fTitleScale) + optionProps.Spacing);
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
								   pointf &size, pointf &scale, int optionHeight, const point &srcRectTopLeft, int& maxWidth )
{
	// need to find the largest string, that will be used in determining the box width
	maxWidth = m_strTitle.size();
	for (unsigned i = 0; i < numOptions; ++i)
	{
		if (m_vOptions[i].Name.size() > (unsigned)maxWidth)
			maxWidth = m_vOptions[i].Name.size();
	}

	if (m_fTitleScale > scale.x)
		maxWidth *= int((float)Globals::g_pBitMapFont->GetSize().x * m_fTitleScale);
	else
		maxWidth *= int((float)Globals::g_pBitMapFont->GetSize().x * scale.x);

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
		return m_fpOptionAction(this);
	return NULL;	
}