//////////////////////////////////////////////////////////////////////////
// this class will be used as the base for all non-terrain objects rendered
// on the map
//
//////////////////////////////////////////////////////////////////////////
#ifndef BUILDING_H
#define BUILDING_H

#include "Structs.h"
#include "Assets.h"
#include "ButtonDefines.h"
#include "AbilityObjectBase.h"
#include "Pathfinding.h"
using namespace Pathfinding;
#include "ObjectDefines.h"
using namespace ObjectDefines;

enum eAbSelType { AST_ATTACK = MOUSE_LEFT, AST_DEFENSE = MOUSE_RIGHT, };
enum eInputStatus { IS_HOVER, IS_SELECT_L, IS_SELECT_R, IS_DESELECT, IS_NONE, };

class CCombatSkill;

class CObject
{
	typedef map<eButtonName, vector<CAbilityObjectBase*>> UnlockedAbilitiesMap;

	int		m_nImageID;
	int		m_nType;	// city, unit, building, etc..
	rect	m_rSrc;		// source from the city images sheet

protected:

	bool	m_bDisplayInfo;	// the info screen master display switch

	eButtonName m_eDefaultAbilityType;
	eInputStatus m_eCurrInputStatus;	// if this object is hovered/selected...
	// selection variables (used for units primarily)
	string		m_strName;		// an actual name (unit = Llew, Tegid..) (city = Sycharth..)
	string		m_strTypeName;	// UMKnight, Village, etc.. acts as map key for abilities
	pointf		m_ptScreenPos;	// where the top-left of the selection rect starts
	point		m_ptCoord;		// grid coordinates
	point		m_ptSize;		// width and height (x, y)
	const char* m_szFaction;	// who owns this object (player, pc1, pc2...)
	short		m_nFactionID;
	rectf		m_rSelectionRect;// selectable area of the object for the mouse

	// drawing variables
	point m_ptOffset; // for rendering, make sure the object is in the center of the tile
	float m_fScaleX;
	float m_fScaleY;
	float m_fZDepth;
	DWORD m_dwColor;

	CObject* m_pNeighborEnemy;	// the enemy obj that shares the same tile (two units interlocked in battle)
	CCombatSkill*  m_pCurrAttackAbility;
	CCombatSkill*  m_pCurrDefenseAbility;
	CQuickBarObject*	 m_arrQBSlots[NUM_QB_SLOTS];
	UnlockedAbilitiesMap m_mAbilities;

	void CopyAll( CObject &obj );
	void ClearQBSlotORSlots();	
public:

	virtual ~CObject();
	CObject();
	CObject(int type, point coord, point wPos, string name, const char* faction, int factionID, string typeName);
	CObject(CObject& obj);


	//////////////////////////////////////////////////////////////////////////
	// FUNCTION: Render
	//////////////////////////////////////////////////////////////////////////
	virtual void Render(const rect& viewPort);

	//////////////////////////////////////////////////////////////////////////
	// FUNCTION: Update
	//
	// moveAmt is the final amount to move by (already modified by timeStep)
	//////////////////////////////////////////////////////////////////////////
	virtual void Update(double dTimeStep, const pointf* moveAmt = NULL);

	//////////////////////////////////////////////////////////////////////////
	//	FUNCTION: Input
	//
	//
	//////////////////////////////////////////////////////////////////////////
	virtual void Input(const POINT& mouse) {}

	// uses the map's viewport to determine if the object needs to be rendered
	bool TestOnScreen(const rect& viewPort);
	bool IsOnQuickBar(const CQuickBarObject& qbObj);
	void FindPathToTarget(const point& tileCoord, Path& path);

	//////////////////////////////////////////////////////////////////////////
	// accessors
	inline int GetImageID()	const			{ return m_nImageID;	}
	inline int GetType()	const			{ return m_nType;		}
	inline const point& GetOS() const		{ return m_ptOffset;	}
	inline float GetScaleX()const			{ return m_fScaleX;		}
	inline float GetScaleY()const			{ return m_fScaleY;		}
	inline point GetCoord()	const			{ return m_ptCoord;		}
	inline pointf GetSPos()	const			{ return m_ptScreenPos;	}
	inline point GetSize()	const			{ return m_ptSize;		}
	inline rect* GetSrc()					{ return &m_rSrc;		}
	inline rectf  GetSelectionRect()const	{ return m_rSelectionRect;}
	inline string GetName()			const	{ return m_strTypeName;	}
	inline const char* GetFaction()	const	{ return m_szFaction;   }
	inline short GetFactionID()		const	{ return m_nFactionID;	}
	inline bool GetIsHovered()		const	{ return m_eCurrInputStatus == IS_HOVER; }
	inline DWORD GetColor()			const	{ return m_dwColor;		}
	inline float GetZDepth()		const	{ return m_fZDepth;		}
	inline eButtonName GetDefAbilityType()	{ return m_eDefaultAbilityType;	}
	inline eInputStatus GetCurrInputStatus() const { return m_eCurrInputStatus;}
	inline CCombatSkill* GetDefCombatAbility()		{ return (CCombatSkill*)m_mAbilities[BN_COMBAT_SKILLS][0]; }
	inline CCombatSkill* GetCurrAttackAbility()		{ if (m_pCurrAttackAbility) return m_pCurrAttackAbility; else return (m_pCurrAttackAbility = GetDefCombatAbility());	}
	inline CCombatSkill* GetCurrDefenseAbility()	{ if (m_pCurrDefenseAbility) return m_pCurrDefenseAbility; else return (m_pCurrDefenseAbility = GetDefCombatAbility()); }
	inline CCombatSkill* GetCurrAbilOfType(eAbSelType type) { if (type == AST_ATTACK) return m_pCurrAttackAbility; else return m_pCurrDefenseAbility; }
	CCombatSkill* GetCurrDefCombatAbility(eButtonName& defType);
	inline CQuickBarObject** GetQBSlots()						{ return &m_arrQBSlots[0];	}
	// returns the unlocked abilities for this object and the passed in eButtonName
	inline UnlockedAbilitiesMap& GetAbilitiesMap()							{ return m_mAbilities;				}
	inline const Abilities& GetUnlockedAbilities(eButtonName buttonName)	{ return m_mAbilities[buttonName];	}


	//////////////////////////////////////////////////////////////////////////
	// mutators
	inline void SetImageID(int id)			{m_nImageID = id;	}
	inline void SetSrcRect(rect& r)			{m_rSrc = r;		}
	inline void SetColor(DWORD clr)			{m_dwColor = clr;	}
	inline void SetCoord(const point& coord){m_ptCoord = coord; }
	virtual void SetScrnPos(point& sPos)	{m_ptScreenPos.x = (float)sPos.x + (float)m_ptOffset.x; m_ptScreenPos.y = (float)sPos.y + (float)m_ptOffset.y;}
	inline void SetFaction(char* faction)	{m_szFaction = faction;}
	// if slot == NULL, all QB slots are cleared instead of the one, does NOT check for invalid index
	inline void SetQBSlot(int slot, CQuickBarObject* qbObj)	{ m_arrQBSlots[slot] = qbObj; }
	inline void SetCurrAttackAbility(CCombatSkill* ability) { m_pCurrAttackAbility = ability; }
	inline void SetCurrDefenseAbility(CCombatSkill* ability){ m_pCurrDefenseAbility= ability;	}
	inline void SetCurrAbilOfType(eAbSelType type, CCombatSkill* ability)	
			{ if (type == AST_ATTACK) m_pCurrAttackAbility = ability; else if (type == AST_DEFENSE) m_pCurrDefenseAbility = ability; }
	inline void SetInputStatus(eInputStatus status, DWORD clr) { m_eCurrInputStatus = status; m_dwColor = clr; }	
	inline void ToggleDisplay()						{ m_bDisplayInfo = !m_bDisplayInfo;	}
	inline void SetNeighbor(CObject* const obj)		{ m_pNeighborEnemy = obj;		}
};

#endif