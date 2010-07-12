#include "stdafx.h"

#include "AnimationsManager.h"

CAnimationsManager* CAnimationsManager::GetInstance()
{
	static CAnimationsManager instance;
	return &instance;
}

void CAnimationsManager::AddAnim(const string& animName, AnimProperties& anim)
{
	m_mAnimationsInfo[animName].push_back(anim);
}

void CAnimationsManager::Shutdown()
{
	m_mAnimationsInfo.clear();
}

