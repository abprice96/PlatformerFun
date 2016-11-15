#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QKeyEvent>
#include <QImage>
#include <QObject>
#include <QThread>
#include <QMediaPlayer>

#include "object.h"
#include "player.h"
#include "playingscreen.h"
#include "world.h"
#include "titlescreen.h"

class ObjectLabel : public QLabel {
	Q_OBJECT
	Object * object;
	QWidget * widget;
	int id;

public:
	explicit ObjectLabel(QWidget * parent): QLabel(parent) {
		widget = parent;
		object = NULL;
	}
	~ObjectLabel() { /*do not delete object here. Objects are deleted in the world class*/ }

	int getId() { return id; }
	void setId(int newId) { id = newId; }
	void setObject(Object* initObject)
	{
		object = initObject;
		this->id = initObject->getId();
	}
	Object* getObject(){ return object; }

	// Updates the label position to the position recorded in the model, minus the location of the screen
	void updateLabelPosition()
	{
		PlayingScreen* screen = World::instance().getScreen();
		setGeometry(object->getX() - screen->getX(), object->getY() - screen->getY(), object->getWidth(), object->getHeight());
	}

signals:
	void clicked();
};

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MainWidget(QWidget *parent = 0);
	~MainWidget();

	void loadLevel(QString filename);

	bool canJump(Player* player);
	void death(Player* player);
	void resetPlayer(Player* player);
	void setWalkImage(Player* player);
	void setJumpImage(Player* player);
	void showCoin();
    void moveCoinOrFlag();
	QTimer* getTimer() {return timer;}
	QTimer* getClock() { return clock; }
    QTimer * getRotateTimer() { return RotateTimer; }
    void delay(int);

private:
	Ui::MainWidget *ui;
	QTimer * timer;
    QTimer * RotateTimer;
	QTimer * clock;

	bool right;
	bool left;

	ObjectLabel* labelPlayer;
	PlayingScreen* screen;


public slots:
	void on_loadState(QString filename);

private slots:
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void timerHit();
    void RotateTimerHit();
	void clockHit();
	void normalMove() {}
	void normalImage() {}
	void on_PBpause_clicked();
	void on_resumeFromPause();
    void on_restartFromPause();
};

class MoveThread : public QThread
{
	Q_OBJECT
	Object* object;

protected:
	void run();

public:
	MoveThread(Object* obj) : QThread(), object(obj) {}
};

class CheckPlayerCollisionThread : public QThread {
	Q_OBJECT
	bool death;

protected:
	void run();

public:
	CheckPlayerCollisionThread() : QThread(), death(false){}
	bool getDeath() { return death; }
};

class ScreenMoveThread : public QThread {
    Q_OBJECT
protected:
    void run();
};

#endif // MAINWIDGET_H
