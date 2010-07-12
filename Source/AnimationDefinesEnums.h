#pragma once

//////////////////////////////////////////////////////////////////////////
// this file defines all of the animation-related constants and enums, 
// functions, properties, etc. etc.
//////////////////////////////////////////////////////////////////////////

// animation loading enums
enum eLoading { UNIT_NAME, ANIM_NAME, NUM_FRAMES, FRAME_SIZE, };

// direction-related
enum eAnimationDirections {DIR_E, DIR_N, DIR_NE, DIR_NW, DIR_S, DIR_SE, DIR_SW, DIR_W, NUM_DIRECTIONS};
typedef eAnimationDirections eDirections;
const static int gOppositeDirs[] = 
{
	DIR_W, DIR_S, DIR_SW, DIR_SE, DIR_N, DIR_NW, DIR_NE, DIR_E
};

// animation types
enum eAnimTypes {AT_ATTACK, AT_RUN, AT_HIT, AT_DYING, NUM_ANIM_TYPES};
const static string gAnimNames[NUM_ANIM_TYPES] = 
{
	"Attack",
	"Run",
	"Hit",
	"Death"
};
#define ANIM_DELAY 0.055f
//////////////////////////////////////////////////////////////////////////
// AnimProperties
//
// all properties associated with the image the anims are from
// these are what's stored in the AnimationsManager, which units create
//		their appropriate anims with
//////////////////////////////////////////////////////////////////////////
struct AnimProperties 
{
	int		ImageID;
	int		NumCols;
	int		NumFrames;	// how many images the animation is made up of
	point	AnimFrameSize;	// each anim source rect size
	string	Name;

	// TODO:: allow a new type of animation to start in the middle of an image ... a start ID offset or something
	AnimProperties() :
	ImageID(-1),
		NumCols(0),
		NumFrames(0),
		AnimFrameSize(0, 0),
		Name("NONE")
	{
	}
	AnimProperties(int _imageID, int _numCols, int _numFrames, string& _name, point& _frameSize) : 
	ImageID(_imageID), 
		NumCols(_numCols), 
		AnimFrameSize(_frameSize),
		NumFrames(_numFrames),
		Name(_name)
	{}
	AnimProperties& operator=(const AnimProperties& vars)
	{
		ImageID			= vars.ImageID;
		NumCols			= vars.NumCols;
		NumFrames		= vars.NumFrames;
		AnimFrameSize	= vars.AnimFrameSize;
		Name			= vars.Name;
		return *this;
	}
};

// functions to determine if a certain anim is playing
inline bool IsAnimAttack(string& animName) { return (animName == gAnimNames[AT_ATTACK]);	}
inline bool IsAnimRun(string& animName)    { return (animName == gAnimNames[AT_RUN]);		}
inline bool IsAnimHit(string& animName)	   { return (animName == gAnimNames[AT_HIT]);		}
inline bool IsAnimDying(string& animName)  { return (animName == gAnimNames[AT_DYING]);		}


// typedef's
typedef map<string, vector<AnimProperties>> AnimationsInfo;
typedef AnimationsInfo::iterator AnimMapIter;
typedef vector<AnimProperties> ObjAnimations;