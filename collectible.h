#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include"object.h"

#include <QString>

#include <string>

class Collectible : public Object {
protected:
	QString type;
	bool isCollectible;

public:
	Collectible () : Object() {}
	Collectible(int x_, int y_, int width_, int height_, QString image_, QString type_, bool visibility_, bool isCollectible_);

	QString getType() { return type; }
	void setType(QString newType) { type = newType; }
	bool getisCollectible() { return isCollectible; }
	void setisCollectible(bool newCollectibility) { isCollectible = newCollectibility; }

	virtual void load(QString config);
	virtual QString save();
};

#endif // COLLECTIBLE_H
