#pragma once
//////////////////////////////////////////////////////////////////////////
//	singleton class that holds onto all animations
//////////////////////////////////////////////////////////////////////////
#include "AnimationDefinesEnums.h"

class CAnimationsManager
{
public:

private:

	CAnimationsManager(void){}
	~CAnimationsManager(void){}
	CAnimationsManager(CAnimationsManager&);
	CAnimationsManager& operator= (CAnimationsManager&);

	AnimationsInfo m_mAnimationsInfo;

public:

	static CAnimationsManager* GetInstance();

	void AddAnim(const string &animName, const AnimProperties& anim);
	void Shutdown();

	//////////////////////////////////////////////////////////////////////////
	// accessors
	//////////////////////////////////////////////////////////////////////////

	//	returns all the animations for a given object
	inline ObjAnimations& GetAnimations(const string objName) { return m_mAnimationsInfo[objName]; }
};
