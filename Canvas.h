#ifndef CANVAS_H
#define CANVAS_H

class Canvas
{
public:
	virtual void setColour(float r, float g, float b) = 0;
	virtual void moveTo(float x, float y) = 0;
	virtual void lineTo(float x, float y) = 0;
	virtual void circle(float x, float y, float r) = 0;
	virtual void stroke() = 0;
	virtual void fill() = 0;
};

#endif // CANVAS_H
