#include "stdafx.h"

#include "Structs.h"

point::operator pointf()
{
	return pointf((float)x, (float)y);
}


pointf::operator point()
{
	return point((int)x, (int)y);
}