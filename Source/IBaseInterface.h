////////////////////////////////////////////////////////
//   File Name	:	"IBaseInterface.h"
//
//   Author		:	Matthew Di Matteo (MD)
//
//   Purpose	:	To provide a base interface for all game objects
//					to inherit from
////////////////////////////////////////////////////////

#ifndef IBASEINTERFACE_H
#define IBASEINTERFACE_H

class IBaseInterface
{
public:
	/////////////////////////////
	//	Function: virtual destructor
	//////////////////////////////
	virtual ~IBaseInterface(void) = 0 {}

	/////////////////////////////////
	//	virtual functions 
	//	-to be defined in child classes
	//////////////////////////////////
	virtual void Update(float fElapsedTime) = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;
};

#endif