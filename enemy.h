#ifndef ENEMY_H
#define ENEMY_H

#include <QString>


#include "object.h"
#include "platform.h"

class Enemy : public Object
{
protected:
    Platform* currentPlatform;// used to tell the Enemy when it has reached a ledge.

public:
    Enemy();
    Enemy(int x_, int y_, int width_, int height_, QString image_);

	virtual void load(QString config);
	virtual QString save();

    virtual void move();
    virtual void collide(CollisionDetails* details);

    static void unitTest();
};

class FlyingEnemy: public Enemy
{
    bool up;
    int xCount, yCount;
	int startX, startY;

public:
    FlyingEnemy();
    FlyingEnemy(int x_, int y_, int width_, int height_, QString image_);

    void move();
    void collide(CollisionDetails*);

	virtual void load(QString config);
	virtual QString save();

	int getStartX() {return startX;}
	int getStartY() {return startY;}

	void setStartX(int x) {startX = x;}
	void setStartY(int y) {startY = y;}

    static void unitTest() { }
};

#endif // ENEMY_H
