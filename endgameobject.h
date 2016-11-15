#ifndef ENDGAMEOBJECT_H
#define ENDGAMEOBJECT_H

#include "object.h"

class EndGameObject : public Object
{
    int count = 0;
public:
    EndGameObject() : Object() {}
    EndGameObject(int x_, int y_, int width_, int height_, QString image_):
        Object(x_, y_, width_, height_, image_) {}

	virtual void load(QString config);
	virtual QString save();
    void move();
};

#endif // ENDGAMEOBJECT_H
