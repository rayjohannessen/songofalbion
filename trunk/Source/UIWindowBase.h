#pragma once

//////////////////////////////////////////////////////////////////////////
//	the base for all windows, defines components of the UI window base,
//	functionality, and any default values
//////////////////////////////////////////////////////////////////////////

#include "ButtonDefines.h"
class CUIWindowBase;
class CQuickBarObject;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// eReturnCode - corresponds to an option in the optionsList...says what to do when clicked
enum eReturnCode 
{
	RC_CLOSE = -1,					// return value for a window who's "close" button has been clicked
	RC_SELECTION = 1,
	RC_TO_MENU,					// exit from gameplay to main menu
	RC_NO_SELECTION,			// no selection has been clicked
	RC_OUTSIDE_WINDOW,				// the mouse is not in the window
};

// COption - represents an interactable option to be contained in a window
class COption 
{
	friend class CWindowVariablesBase;
	friend class CUIWindowBase;
	friend class CUIOptionsWindow;

	eReturnCode m_eRetCode;
	bool		m_bIsHovered;
	int			m_nTopBtmOffset;	// offset for mouse input area
	rect		m_Rect;				// acts as position and mouse-input detection rect
	CQuickBarObject*	m_pQBObj;
	OptionBtnActionFP	m_fpOptionAction;

	// all parameters determined dynamically by the owner of this option
	void Render(float zPos, float scale, DWORD color);
	bool IsMouseOverOption(const point& mouse)
	{
		rect adjustRect = m_Rect;
		adjustRect.top -= m_nTopBtmOffset; adjustRect.bottom += m_nTopBtmOffset;
		return (m_bIsHovered = adjustRect.IsPointInRect(mouse));
	}

	// Accessors
	inline void SetRect(const rect& r) { m_Rect = r; }
	CUIWindowBase* ExecuteOptionAction();	

public:
	string	Name;	// what is displayed
	DWORD	HoveredColor;

	COption(const string& name, eReturnCode retCode, OptionBtnActionFP fp = NULL, CQuickBarObject* const qbObj = NULL, rect* const _rect = NULL, DWORD hoverColor = RED) : 
			m_eRetCode(retCode), Name(name), m_fpOptionAction(fp), HoveredColor(hoverColor), m_bIsHovered(false), m_pQBObj(qbObj)	
			{
				if (_rect)
					m_Rect = *_rect;
			}
	inline CQuickBarObject* const GetQBObj() const	{ return m_pQBObj;		}
	inline eReturnCode GetRetCode()	const			{ return m_eRetCode;	}
};

// COption DEFAULTS (OD)
#define OD_WIDTH 100
#define OD_SPACING 16
#define OD_MAXROWS 8
#define OD_EDGEPAD 16
#define OD_INPUTRECT_OS (OD_SPACING>>1)

typedef struct _OptionProps
{
	int			Width;
	int			Spacing;
	unsigned	MaxRows;
	int			EdgePadding;
	int			TopBtmInputRectOffset;

	_OptionProps( int tbOffset = OD_INPUTRECT_OS, int _width = OD_WIDTH, int _spacing = OD_SPACING, unsigned _maxRows = OD_MAXROWS, int _edgePad = OD_EDGEPAD) : 
			Width(_width), Spacing(_spacing), MaxRows(_maxRows), EdgePadding(_edgePad), TopBtmInputRectOffset(tbOffset)
	{ }

}OptionProps;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// CWindowVariablesBase - provides definition of what a window contains,
//						  what it's properties are, how it behaves, and
//						  displays any input options 
//
//	to be overwritten by child classes of CUIWindowBase when necessary
//////////////////////////////////////////////////////////////////////////

enum eClosePosition {CP_TOP_LEFT, CP_TOP_RIGHT, CP_BOTTOM_LEFT, CP_BOTTOM_RIGHT, NUM_CLOSE_POS };
class CUIWindowBase;
typedef vector<COption> OptionsList;
typedef vector<COption>::iterator OptionsListIter;

const static int CLOSE_BTN_EDGE_DIST = 25;
const static short TRANSPARENCY_LEVEL = 55;

class CWindowVariablesBase 
{
	friend class CUIWindowBase;

	// variables for the window box itself
	bool			m_bCanAlpha;
	bool			m_bCentered;
	eClosePosition	m_eClosePos;
	DWORD   m_dwTextColor;
	float	m_fZPos;
	DWORD	m_dwBgClr;
	DWORD	m_dwFrameClr;
	float	m_fFrameTextZPos;
	pointf	m_ptImageScale;
	rect	m_rRect;	// position and size ( size is not for rendering purposes, used for input )

	// contents variables
	int				m_nTitleY;
	DWORD			m_dwTitleColor;
	float			m_fTitleScale;
	string			m_strTitle;
	OptionsList		m_vOptions;
	OptionProps		m_OptionProps;

	int SetRect( unsigned numOptions, const OptionProps& optionProps, int numCols, pointf& size, const pointf& scale, int optionHeight, const point& srcRectTopLeft, int& maxWidth );
	void SetOptionPositions( const OptionProps &optionProps, int optionHeight, unsigned numOptions );

public:
						// srcRectTopLeft == the top left screen pos of the button that was pressed
	CWindowVariablesBase( const point& srcRectTopLeft, string title, OptionsList& options, const OptionProps& optionProps, bool canAlpha = true,
						  const pointf& scale = pointf(1.0f, 1.0f), eClosePosition closePos = CP_TOP_RIGHT, DWORD bgClr = 0xff000000,
						  DWORD color = YELLOW_WHITE, float zPos = DEPTH_WNDOPTIONS, float titleScale = 1.2f, DWORD titleColor = YELLOW_WHITE, bool centerWindow = false );

	~CWindowVariablesBase() {}
};

class CUIWindowBase
{
	inline void SetAlphaBits( DWORD& clr, short a );

protected:
	int		m_nImageID;
	CWindowVariablesBase* m_pVariables;

	// accessors (including variables in m_pVariables)
	inline bool canAlpha() const					{ return m_pVariables->m_bCanAlpha;		}
	inline const rect& windowRect() const			{ return m_pVariables->m_rRect;			}
	inline OptionsList& optionsList()				{ return m_pVariables->m_vOptions;		}
	inline DWORD bgClr()	const					{ return m_pVariables->m_dwBgClr;		}
	inline DWORD frameClr() const					{ return m_pVariables->m_dwFrameClr;	}
	inline string title()	const					{ return m_pVariables->m_strTitle;		}
	inline DWORD titleClr() const					{ return m_pVariables->m_dwTitleColor;	}
	inline float titleScale() const					{ return m_pVariables->m_fTitleScale;	}
	inline const pointf& scale() const				{ return m_pVariables->m_ptImageScale;	}
	inline float zPos() const						{ return m_pVariables->m_fZPos;			}
	inline float zPosTxtFrame() const				{ return m_pVariables->m_fFrameTextZPos;}
	inline DWORD textClr() const					{ return m_pVariables->m_dwTextColor;	}
	inline OptionProps& optionProperties()			{ return m_pVariables->m_OptionProps;	}
	inline eClosePosition closePos() const			{ return m_pVariables->m_eClosePos;		}

	// mutators
	void SetAlpha(short a);

public:
	CUIWindowBase() : m_nImageID(-1), m_pVariables(NULL) {}
	CUIWindowBase( int imageID, CWindowVariablesBase* variables ) : 
		m_nImageID(imageID)
	{ 
		m_pVariables = variables;
	}
	virtual ~CUIWindowBase() { SAFE_DELETE(m_pVariables); }

	// return the index of the button pressed
	virtual eReturnCode  Input(POINT mouse, CUIWindowBase* window = NULL) = 0;
	virtual void Update(float dTimeStep) = 0;
	virtual void Render() = 0;
};
