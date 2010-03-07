#ifndef CANVAS_H
#define CANVAS_H

class Canvas
{
public:
	virtual void setColour(double r, double g, double b) = 0;
	virtual void moveTo(double x, double y) = 0;
	virtual void lineTo(double x, double y) = 0;
	virtual void circle(double x, double y, double r) = 0;
	virtual void stroke() = 0;
	virtual void fill() = 0;
};

#endif // CANVAS_H
