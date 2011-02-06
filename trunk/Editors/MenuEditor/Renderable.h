#pragma once

class Renderable
{
public:
	Renderable(void);
	virtual ~Renderable(void) = 0;

	virtual void Render() const = 0;
};
