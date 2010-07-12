#ifndef UNIT_H
#define UNIT_H

#include "Object.h"
#include "Animation.h"
#include "ObjectDefines.h"
class CUnit;

//////////////////////////////////////////////////////////////////////////
// each unit will have a set of animations. They will simply be a set of
// pointers to the various types of animations the unit has. 
//////////////////////////////////////////////////////////////////////////
class CUnit : public CObject
{
	typedef map<string, CAnimation*> UnitAnims;

	int m_nUnitType;
	int m_nMaxVitality;
	int m_nVitality;		// referred to as vitality
	int m_nAtt;
	int m_nDef;
	int m_nMaxStamina;	// referred to as Stamina
	int m_nStamina;
	int m_nMaxMP;
	int m_nMagPts;
	int m_nRange;		// in tiles
	int m_nNumAnims;
	string m_strCurrAnim;
	pointf m_ptOriginalScrnOS;
	pointf m_ptMoveToScreenPos;
	UnitAnims m_mAnimations;

	void ClearAnims();
public:
	CUnit();
	CUnit(CUnit& unit);
	CUnit( int nUnitType, int type, point& coord, point& sPos, 
		string name, const char* faction = "Llwyddi", int factionID = 0);
	~CUnit();

	CUnit& operator= ( CUnit& unit);
	void DeepCopyAll( CUnit &unit );

	// draws the unit at its current frame
	void Render(const rect& viewPort);

	// performs a map move, only if it has enough movement points remaining
	// returns true if move was made
	bool Move(point& coord, point& screenPos, int cost, eAnimationDirections facing);
	void ChangeAnim(const string& animName);

	void Update(double fTimeStep, const pointf* moveAmt = NULL);

	//////////////////////////////////////////////////////////////////////////
	//	For simply facing the target when attacking, movement is not covered,
	//		as pathfinding would be necessary
	//
	// returns true if target was within range
	//////////////////////////////////////////////////////////////////////////
	bool FaceTarget(const point& targetPt, int* newDir = NULL, bool setFacing = true);

	bool MouseInRect(point& mousePt);

	//////////////////////////////////////////////////////////////////////////
	// accessors
 	inline int GetNumAnims()	const		{ return m_nNumAnims;					}
	inline int GetUnitType()	const		{ return m_nUnitType;					}
	inline int GetVitality()	const		{ return m_nVitality;					}
	inline int GetMaxVit()		const		{ return m_nMaxVitality;				}
	inline int GetStamina()		const		{ return m_nStamina;					}
	inline int GetMaxStamina()	const		{ return m_nMaxStamina;					}
	inline int GetAttk()		const		{ return m_nAtt;						}
	inline int GetDef()			const		{ return m_nDef;						}
	inline int GetMP()			const		{ return m_nMagPts;						}
	inline int GetMaxMagPts()   const    	{ return m_nMaxMP;						}
	inline int GetRange()		const		{ return m_nRange;						}
	inline eAnimationDirections GetOppositeFacing()	{ return (eAnimationDirections)gOppositeDirs[m_mAnimations[m_strCurrAnim]->CurrDir()]; }
	inline string GetCurrAnimString()const	{ return m_strCurrAnim;					}
	inline pointf GetOrigScOS() const		{ return m_ptOriginalScrnOS;			}
	inline pointf GetMoveToPos()const		{ return m_ptMoveToScreenPos;			}
	inline UnitAnims& GetAnims()			{ return m_mAnimations;					}
	inline CAnimation& GetCurrAnim()		{ return *m_mAnimations[m_strCurrAnim]; }

	//////////////////////////////////////////////////////////////////////////
	// mutators
	inline void SetFacing(eAnimationDirections facing)	{ m_mAnimations[m_strCurrAnim]->CurrDir(facing);			}
	inline void AddToVitality(int amt)			{ m_nVitality += amt; if (m_nVitality < 0) m_nVitality = 0; }
	inline void DecrementStamina(int decAmt)	{ m_nStamina -= decAmt;			}
	inline void ResetStamina()					{ m_nStamina = m_nMaxStamina;	}
	inline void SetScreenPosWithoutOS(const pointf& pt);
	void SetScrnPos(point& sPos);

	CTriggerTimer m_Timer;
};

#endif