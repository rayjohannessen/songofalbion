#ifndef STRUCTS_H
#define STRUCTS_H

struct pointf;
struct point 
{
	int x;
	int y;
	point() {x=0;y=0;}
	point(const point& pt)
	{ x = (int)pt.x; y = (int)pt.y; }
	point(const POINT& pt)
	{ x = (int)pt.x; y = (int)pt.y; }
	point(int XorWidth, int YorHeight)
	{ x = XorWidth; y = YorHeight; }
	inline point& operator= (const point& pt)
	{ if (this == &pt) return *this; x = (int)pt.x; y = (int)pt.y; return *this;}
	inline const point& operator+= (const point& pt)
	{ if (this == &pt) return *this; x += pt.x; y += pt.y; return *this; }
	bool operator== (const point& pt) 
	{ return (pt.x == x && pt.y == y);}
	bool operator!= (const point& pt)
	{ return (pt.x != x || pt.y != y);}
	operator pointf() const;
	void Offset(int osx, int osy) {x += osx; y += osy;}
	void Offset(point os) {x += os.x; y += os.y;}
};
struct pointf
{
	float x;
	float y;
	pointf() {x=0.0f;y=0.0f;}
	pointf(float X, float Y)
	{ x = X; y = Y; }
	inline pointf& operator=(const pointf& pt)
	{ 
		if (this == &pt) 
			return *this; 
		x = pt.x; y = pt.y; 
		return *this; 
	}
	pointf operator+ (const pointf& pt)
	{ 
		pointf result = *this;
		result.x += pt.x; result.y += pt.y; 
		return result; 
	}
	pointf operator- (const pointf& pt)
	{
		pointf result = *this;
		result.x -= pt.x; result.y -= pt.y; 
		return result;
	}
	pointf operator* (float multiplier)
	{
		pointf result = *this;
		result.x *= multiplier; result.y *= multiplier;
		return result;
	}
	pointf& operator+=(const pointf& pt)
	{ x += pt.x; y += pt.y; return *this; }
	bool operator== (const pointf& pt) 
	{ return (pt.x == x && pt.y == y);}
	bool operator!= (const pointf& pt)
	{ return (pt.x != x || pt.y != y);}
	operator point() const;
	inline bool IsWithinRange(const pointf& pt, float range = 5.0f) const
	{
		return (pt.x - range < x && pt.x + range > x &&
				pt.y - range < y && pt.y + range > y);
	}
	void Offset(float osx, float osy) {x += osx; y += osy;}
	void Offset(const pointf& os) {x += os.x; y += os.y;}
	inline float Length() const { return sqrt(x * x + y * y);	}
	inline void Normalize()
	{
		float mag = Length(); 
		x = x / mag;
		y = y / mag;
	}
};
#define PTF_DEF_ONE (pointf(1.0f, 1.0f))
struct rect 
{
	int top;
	int bottom;
	int left;
	int right;
	rect() {top = -1; bottom = -1; left = -1; right = -1;}
	rect(int t, int b, int l, int r)
	{
		top		= t;
		bottom	= b;
		left	= l;
		right	= r;
	}
	rect(const pointf&pos, int size)
	{
		top		= (int)pos.y;
		bottom	= (int)pos.y + size;
		left	= (int)pos.x;
		right	= (int)pos.x + size;
	}
	rect(const pointf& pos, const point& size)
	{
		top		= (int)pos.y;
		bottom	= (int)pos.y + size.y;
		left	= (int)pos.x;
		right	= (int)pos.x + size.x;
	}
	rect(const point& pos, const point& size)
	{
		top		= pos.y;
		bottom	= pos.y + size.y;
		left	= pos.x;
		right	= pos.x + size.x;
	}
	rect(const point& pos, const point& size, const point& offset)
	{
		top		= pos.y;
		bottom	= pos.y + size.y;
		left	= pos.x;
		right	= pos.x + size.x;
		Offset(offset);
	}
	inline rect& operator= (const rect& r)
	{
		top		= r.top;
		bottom	= r.bottom;
		left	= r.left;
		right	= r.right;
		return *this;
	}
	rect& operator+= (const pointf& change)
	{ top += (int)change.y; bottom += (int)change.y; left += (int)change.x; right += (int)change.x; return *this; }
	inline bool IsPointInRect(const point& pt) const
	{
		return (pt.x >= left && pt.x <= right
			&& pt.y >= top && pt.y <= bottom);
	}
	bool IsRectIntersecting(const rect& r) const
	{
		return ( IsPointInRect(point(r.left, r.top)) || IsPointInRect(point(r.right, r.top)) || 
				 IsPointInRect(point(r.right, r.bottom)) || IsPointInRect(point(r.left, r.bottom)) );
	}
	void Offset(const point& offset)
	{
		top += offset.y;
		bottom += offset.y;
		left += offset.x;
		right += offset.x;
	}
};

struct rectf
{
	float top;
	float bottom;
	float left;
	float right;
	rectf() {top = -1.0f; bottom = -1.0f; left = -1.0f; right = -1.0f;}
	rectf(float t, float b, float l, float r)
	{
		top		= t;
		bottom	= b;
		left	= l;
		right	= r;
	}
	rectf(const pointf& pos, const pointf& size)
	{
		top		= pos.y;
		bottom	= pos.y + size.y;
		left	= pos.x;
		right	= pos.x + size.x;
	}
	rectf(const pointf& pos, const point& size)
	{
		top		= pos.y;
		bottom	= pos.y + (float)size.y;
		left	= pos.x;
		right	= pos.x + (float)size.x;
	}
	inline rectf& operator= (const rectf& r)
	{
		top		= r.top;
		bottom	= r.bottom;
		left	= r.left;
		right	= r.right;
		return *this;
	}
	rectf& operator+= (const pointf& change)
	{ top += change.y; bottom += change.y; left += change.x; right += change.x; return *this; }
	operator rect() { return rect((int)top, (int)bottom, (int)left, (int)right); }
	inline bool IsPointInRect(pointf& pt)
	{
		return (pt.x >= left && pt.x <= right
			&& pt.y >= top && pt.y <= bottom);
	}
	bool IsRectIntersecting(const rectf& r) 
	{
		return ( IsPointInRect(pointf(r.left, r.top)) || IsPointInRect(pointf(r.right, r.top)) || 
			IsPointInRect(pointf(r.right, r.bottom)) || IsPointInRect(pointf(r.left, r.bottom)) );
	}
};



// struct DEPTH
// {
// 	float GROUND;
// 	float GROUNDL2;
// 	float SELECTION;
// 	float CHARACTER_BEHIND;
// 	float CHARACTER_AHEAD;
// 	float OBJECTS;
// 	float ARROW;
// 	float MENUS;
// 	DEPTH()
// 	{
// 		GROUND = 1.0f;
// 		GROUNDL2 = 0.9f;
// 		SELECTION = 0.8f;
// 		CHARACTER_BEHIND = 0.7f;
// 		OBJECTS = 0.6f;
// 		CHARACTER_AHEAD = 0.5f;
// 		ARROW = 0.49f;
// 		MENUS = 0.11f;
// 	}
// };
// 

#endif