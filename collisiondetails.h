#ifndef COLLISIONDETAILS_H
#define COLLISIONDETAILS_H

#include "object.h"
class Object;

class CollisionDetails
{
    int xStopCollide;
    int yStopCollide;
    Object* collided;
public:
    CollisionDetails(int xDist, int yDist, Object* collideWith) :
        xStopCollide(xDist), yStopCollide(yDist), collided(collideWith) {}

    int getXStopCollide() { return xStopCollide; }
    int getYStopCollide() { return yStopCollide; }
    Object* getCollided() { return collided;}
};

#endif // COLLISIONDETAILS_H
