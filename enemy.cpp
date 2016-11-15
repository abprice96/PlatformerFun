#include "enemy.h"
#include "world.h"
#include "platform.h"
#include <QApplication>

#include <cassert>
#include <iostream>

using namespace std;

void Enemy::unitTest()
{
    Enemy* enemy = new Enemy(0,0,20,20,"enemy");
    Player* player = new Player(10,0,20,20,"player");
    CollisionDetails* collision = player->checkCollision(enemy);

    player->setPower("shield",false);
    player->setWalkImage();
    assert(player->getImage() == ":/images/maincharacter/hurtleft.png");

    collision = player->checkCollision(enemy);
    assert (enemy->getVisibility() == false);
    delete player;

    Platform* plat = new Platform(10,0,20,20,"player");
    enemy->setRight(true);
    collision = enemy->checkCollision(plat);
    assert (enemy->isRight() == true);

    delete plat;
    delete enemy;
    delete collision;
}

Enemy::Enemy()
{
    right = true;
    onPlatform = true;
    xSpeed = 3;
    ySpeed = 1;
    count = 0;
    currentPlatform = new Platform();

    //unitTest();
}
Enemy::Enemy(int x_, int y_, int width_, int height_, QString image_): Object(x_,y_,width_,height_,image_)
{
    right = true;
    onPlatform = true;
    xSpeed = 3;
    ySpeed = 1;
    count = 0;
    currentPlatform = new Platform();

    //unitTest();
}

void Enemy::load(QString config)
{
	try
	{
		QList<QString> params = config.split(",");
		Object::load(config);
		this->setVisibility(Object::getQListElement(params, 6) == "true");
		this->setDamage(10);
		this->setXSpeed(2);
		this->setRight(true);
	}
	catch (exception& ex)
	{
		throw invalid_argument(ex.what());
	}
}

QString Enemy::save()
{
	QString out = Object::save();
	out += "," + QString::fromStdString(this->getVisibility() ? "true" : "false");
	return out;
}

void Enemy::move()
{
    // if the enemy's at a cliff, it turns around.
    if (getRightPoint() + xSpeed >= currentPlatform->getRightPoint() || x - xSpeed <= currentPlatform->getX())
        right = !right;

    if (right)
    {
        x += xSpeed;
    }
    else
    {
        x -= xSpeed;
    }

    y += ySpeed;
    for (size_t i = 0; i < World::instance().getObjects().size(); i ++)
    {
        CollisionDetails* col = checkCollision(World::instance().getObjects().at(i));
        if (col != NULL)
        {
            collide(col);
            delete col;
        }
    }
}

void Enemy::collide(CollisionDetails *details)
{
    if (dynamic_cast<Platform*>(details->getCollided()) != NULL || dynamic_cast<Player*>(details->getCollided()) != NULL) {
        if (details->getXStopCollide() != 0) {
            x += details->getXStopCollide();
            if (details->getXStopCollide() > 0)
            {
                right = true;
            }
            if (details->getXStopCollide() < 0)
            {
                right = false;
            }
        }
        if (details->getYStopCollide() != 0) {
            y += details->getYStopCollide();
            currentPlatform = dynamic_cast<Platform*>(details->getCollided());
        }
    }
}

// ======FlyingEnemy=====


FlyingEnemy::FlyingEnemy(): Enemy(), up(true), xCount(0), yCount(0)
{
    ySpeed = 2;
}

FlyingEnemy::FlyingEnemy(int x_, int y_, int width_, int height_, QString image_): Enemy(x_,y_,width_,height_,image_), up(true), xCount(0), yCount(0)
{
    ySpeed = 2;
}

void FlyingEnemy::load(QString config)
{
	try
	{
		QList<QString> params = config.split(",");
		Enemy::load(config);
		this->setStartX(Object::getQListElement(params, 7).toInt());
		this->setStartY(Object::getQListElement(params, 8).toInt());
	}
	catch (exception& ex)
	{
		throw invalid_argument(ex.what());
	}
}

QString FlyingEnemy::save()
{
	QString out = Enemy::save();
	out += "," + QString::number(this->getStartX());
	out += "," + QString::number(this->getStartY());
	return out;
}

void FlyingEnemy::move()
{
    if (isRight())
    {
        x += xSpeed;
    }
    else
    {
        x -= xSpeed;
    }

    if (up)
        y -= ySpeed;
    else
        y += ySpeed;

    // used to turn the FlyingEnemy after it travels a certain distance.
    yCount ++;
    xCount ++;
    if (yCount == 10)
    {
        up = !up;
        yCount = 0;
    }
    if (xCount == 50)
    {
        right = !right;
        xCount = 0;
    }
    for (size_t i = 0; i < World::instance().getObjects().size(); i ++)
    {
        CollisionDetails* col = checkCollision(World::instance().getObjects().at(i));
        if (col != NULL)
        {
            collide(col);
            delete col;
        }
    }
}

void FlyingEnemy::collide(CollisionDetails *details)
{
    if (dynamic_cast<Platform*>(details->getCollided()) != NULL || dynamic_cast<Player*>(details->getCollided()) != NULL) {
        if (details->getXStopCollide() != 0) {
            x += details->getXStopCollide();
            if (details->getXStopCollide() > 0)
            {
                setRight(true);
            }
            if (details->getXStopCollide() < 0)
            {
                setRight(false);
            }
        }
        if (details->getYStopCollide() != 0) {
            y += details->getYStopCollide();
            if (details->getYStopCollide() < 0)
            {
                up = true;
            }
            if (details->getYStopCollide() > 0)
            {
                up = false;
            }
        }
    }
}
