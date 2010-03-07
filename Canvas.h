#ifndef CANVAS_H
#define CANVAS_H

class Canvas
{
public:
	virtual void moveTo(float x, float y) = 0;
	virtual void lineTo(float x, float y) = 0;
	virtual void circle(float x, float y, float r) = 0;
	virtual void stroke() = 0;
};

#endif // CANVAS_H
