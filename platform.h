#ifndef PLATFORM_H
#define PLATFORM_H

#include"object.h"

class Platform : public Object
{
	int xRange, yRange;
	int startX, startY;

public:
    Platform() : Object() {}
    Platform(int x_, int y_, int width_, int height_, QString image_):
		Object(x_, y_, width_, height_, image_), xRange(0), yRange(0) {}

	virtual void load(QString config);
	virtual QString save();

	void move();

	int getXRange() {return xRange;}
	int getYRange() {return yRange;}
	int getStartX() {return startX;}
	int getStartY() {return startY;}

	void setXRange(int range) {xRange = range;}
	void setYRange(int range) {yRange = range;}
	void setStartX(int x) {startX = x;}
	void setStartY(int y) {startY = y;}
};

#endif // PLATFORM_H
