#ifndef OBJECT_H
#define OBJECT_H

#include<QString>
#include <QMediaPlayer>
#include <QDebug>
#include "collisiondetails.h"

using namespace std;

class CollisionDetails;

class Object
{
protected:
    int x;
    int y;
    int id;
    int width;
    int height;
    QString image;
    static int nextID;
    bool onPlatform;
	bool visibility;

    int xSpeed;
    int ySpeed;

    bool right;

	int count = 10;// used to animate images. thought it'd be better to move it here from mainwindow.

public:
    Object();
	Object(int x_, int y_, int width_, int height_, QString image_): x(x_), y(y_), width(width_), height(height_), image(image_), onPlatform(false), xSpeed(0), ySpeed(0), count(0) {
		id = nextID++;
    }
	Object(QString config);

	// method for safely retrieving elements from a QList<QString>
	static QString getQListElement(QList<QString>, int index);

	virtual ~Object();

	CollisionDetails* checkCollision(Object * that);
	static void resetNextId();

	virtual void load(QString config);
	virtual QString save();

    virtual void move() { }

	// accessors & mutators
	virtual int getX() {return x;}
	virtual int getY() {return y;}
	virtual int getWidth() {return width;}
	virtual int getHeight() {return height;}
    virtual int getRightPoint() {return (x + width);}
    virtual int getBottomPoint() {return (y + height);}
	virtual QString getImage() {return image;}
	virtual int getXSpeed() { return xSpeed; }
	virtual int getYSpeed() { return ySpeed; }
	int getId() {return id;}
    bool getVisibility() { return visibility; }
    void setVisibility(bool newVisibility) { visibility = newVisibility; }

	virtual void setX(int x_) {x = x_;}
	virtual void setY(int y_) {y = y_;}
	virtual void setWidth(int w_) {width = w_;}
	virtual void setHeight(int h_) {height = h_;}
	virtual void setImage(QString image_) {image = image_;}
	virtual void setXSpeed(int speed) {xSpeed = speed;}
	virtual void setYSpeed(int speed) {ySpeed = speed;}
	void setId(int newId) { id = newId; }

    virtual void collide(CollisionDetails*) {}

    bool isOnObject(Object* that);

    static void unitTest();

    virtual void setAmount(int) { }

    // virtual setter methods for enemies.
    virtual void setDamage(int) { }

    virtual bool isRight() { return right; }
	virtual void setRight(bool newRight) { right = newRight; }

    int getCount() { return count; }
    void advanceCount() { count++; }
    void setCount(int num) { count = num; }
};

#endif // OBJECT_H
