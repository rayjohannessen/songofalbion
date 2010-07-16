#include "stdafx.h"

#include "Structs.h"

point::operator pointf() const
{
	return pointf((float)x, (float)y);
}


pointf::operator point() const
{
	return point((int)x, (int)y);
}