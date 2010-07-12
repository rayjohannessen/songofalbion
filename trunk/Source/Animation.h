
#ifndef ANIMATION_H
#define ANIMATION_H

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
#include "Timer.h"
#include "AnimationDefinesEnums.h"

//////////////////////////////////////////////////////////////////////////
//	-Each CAnimation is composed of all like animations, as long as they fit
//		on a 2048x2048 image file. "like animations" being all attack, running,
//		idle, etc... they are simply divided into all the directions
//
//	-m_nCurrAnimDir corresponds to the eAnimationDirections enum values, find the current frame's ID
//		by: m_nCurrAnimDir * m_nNumFrames + m_nCurrFrame
//
//	-m_nCurrFrame represents how many frames into the current animation, from 0 -> m_nNumFrames	
//
//  -we know how many frames in each new animation set (i.e. each new direction) there are, 
//		and how many columns the image has, and how wide/high each frame is,
//		so with that we are able to calculate each animation's starting frame at runtime
//////////////////////////////////////////////////////////////////////////
class CAnimation
{
	AnimProperties m_srcVariables;
	CTimer	m_timer;
	rect	m_rCurrSrc;			// the current frame's image source rect
	float	m_fMiddlePoint;		// the time value that is the middle of the animation (when an attack's actual strike would appear to occur)
	float   m_fCurrTimeInAnim;	// how much time has passed since the animation's play function was called
	int		m_nCurrAnimDir;		// keeps track of which "set" of frames are being used which make up the animation
								// east-facing starts at 0, north = 1, so on and so forth
	int		m_nCurrFrame;		// which frame (src rect ID) in the current animation is it on?
	int		m_nNumTimesToPlay;	// how many times to cycle through all the frames
	bool	m_bIsPlaying;
	bool	m_bLoop;

public:
	~CAnimation();
	CAnimation();
	CAnimation(eAnimationDirections animDir, AnimProperties& srcImageVars);

	//////////////////////////////////////////////////////////////////////////
	// returns true if the animation is done playing
	//////////////////////////////////////////////////////////////////////////
	bool Update(double dElapsedTime);

	void CalcSrcRect();

	//////////////////////////////////////////////////////////////////////////
	// Play() also resets to the beginning frame
	// pass in a number to play that many times, update will return true
	// when the anim has been played that many times
	//////////////////////////////////////////////////////////////////////////
	void Play(int numTimesToPlay = 1, bool loop = false);

	// Stop() resets timer and sets current frame to 0
	void Stop();

	//////////////////////////////////////////////////////////////////////////
	// mutators
	inline void CurrDir(eAnimationDirections val)	{ m_nCurrAnimDir = (int)val; CalcSrcRect();	}
	inline void NumTimesToPlay(int val) { m_nNumTimesToPlay = val;	}
	inline void Loop(bool val)			{ m_bLoop = val;			}

	//////////////////////////////////////////////////////////////////////////
	// accessors
	inline bool   Loop()		const	{ return m_bLoop;			}
	inline int    NumTimesToPlay()const	{ return m_nNumTimesToPlay; }
	inline eAnimationDirections    CurrDir()	const	{ return (eAnimationDirections)m_nCurrAnimDir;	}
	inline bool   IsPlaying()	const	{ return m_bIsPlaying;		}
	inline int    GetCurrFrame()const	{ return m_nCurrFrame;		}
	inline int    GetNumFrames()const	{ return m_srcVariables.NumFrames;	}
	inline int    GetImageID()	const	{ return m_srcVariables.ImageID;	}
	inline string GetName()		const	{ return m_srcVariables.Name;		}
	inline rect&  GetSourceRect()		{ return m_rCurrSrc;		}
	inline const AnimProperties& GetAnimProperties() const { return m_srcVariables; }
	inline bool IsAtMiddlePoint() const { return (m_fCurrTimeInAnim >= m_fMiddlePoint);	}
};

#define NUM_UMKNIGHT_ATTACK_FRAMES

#endif