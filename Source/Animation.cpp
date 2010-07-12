#include "stdafx.h"

#include "Animation.h"

void CAnimation::Play(int numTimesToPlay /* = 1 */, bool loop /* = false */)
{
	m_bLoop = loop;
	m_nNumTimesToPlay = numTimesToPlay;
	m_timer.ResetElapsed();
	m_nCurrFrame = 0;
	m_fCurrTimeInAnim = 0.0f;
	m_bIsPlaying = true;
}

void CAnimation::Stop()
{
	m_timer.ResetElapsed();
	--m_nCurrFrame;
	m_fCurrTimeInAnim = 0.0f;
	m_bIsPlaying = false;
	CalcSrcRect();
}

bool CAnimation::Update(double dElapsedTime)
{
	m_timer.Update();
	if (m_bIsPlaying)
	{
		m_fCurrTimeInAnim += (float)dElapsedTime;
		if (m_timer.GetElapsed() >= ANIM_DELAY)
		{
			++m_nCurrFrame;
			if (m_nCurrFrame == m_srcVariables.NumFrames)
			{
				if (!m_bLoop)
				{
					--m_nNumTimesToPlay;
					if (m_nNumTimesToPlay == 0)
					{ 
						Stop(); 
						return true; // finished == true
					}
				}
				Play(1, true);	// play it again!
			} 
			
			// calculate the source rect of the current frame
			CalcSrcRect();
			
			m_timer.ResetElapsed();
		}
	}
	return false;
}

CAnimation::CAnimation() :
	m_rCurrSrc(0, 128, 0, 128),
	m_fMiddlePoint(0.0f),
	m_fCurrTimeInAnim(0.0f),
	m_nCurrAnimDir(-1),
	m_nCurrFrame(-1),
	m_nNumTimesToPlay(0),
	m_bIsPlaying(false),
	m_bLoop(false)
{
}
CAnimation::CAnimation(eAnimationDirections animDir, AnimProperties& srcImageVars) :
	m_srcVariables(srcImageVars),
	m_fMiddlePoint(srcImageVars.NumFrames * ANIM_DELAY * 0.5f),
	m_fCurrTimeInAnim(0.0f),
	m_nCurrFrame(0),
	m_nNumTimesToPlay(0),
	m_bIsPlaying(false),
	m_bLoop(false)
{
	CurrDir(animDir);
}
CAnimation::~CAnimation()
{
}
inline
void CAnimation::CalcSrcRect()
{
	int frameID = m_nCurrAnimDir * m_srcVariables.NumFrames + m_nCurrFrame;
	m_rCurrSrc = rect(point((frameID % m_srcVariables.NumCols) * m_srcVariables.AnimFrameSize.x, 
					  (frameID / m_srcVariables.NumCols) * m_srcVariables.AnimFrameSize.y), 
					   m_srcVariables.AnimFrameSize);
}