#include <QLabel>
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <QKeyEvent>
#include <cmath>
#include <QTime>
#include <QSoundEffect>

#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "world.h"
#include "titlescreen.h"
#include "endgame.h"
#include "highscore.h"
#include "highscorepage.h"
#include "loadsave.h"
#include "pausescreen.h"
#include <sstream>
#include <QDir>

MainWidget::MainWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MainWidget)
{
	ui->setupUi(this);
	ui->lblLife1->raise();
	ui->lblLife2->raise();
	ui->lblLife3->raise();
	ui->lblScore->raise(); // these components should not be under the world objects
	ui->lblTimeLeft->raise();

	timer = new QTimer(this);
	timer->setInterval(50);
	connect(timer, SIGNAL(timeout()), this, SLOT(timerHit()));

	clock = new QTimer(this);
	clock->setInterval(1000);
	connect(clock, SIGNAL(timeout()), this, SLOT(clockHit()));


        RotateTimer = new QTimer(this);
        RotateTimer->setInterval(200);
        connect(clock, SIGNAL(timeout()), this, SLOT(RotateTimerHit()));


	right = false;
	left = false;

	// ensure existence of 'data' folder in executable directory
	QDir dataFolder("data");
	if (!dataFolder.exists())
		QDir().mkdir("data");

	TitleScreen* titleScrn = new TitleScreen(this);
	titleScrn->show();
	titleScrn->raise();
}

void MainWidget::loadLevel(QString filename)
{
	ObjectLabel* lblPlayer = NULL;

	//Deletes all objects from the last game
	for (int i = 0; i < ui->worldWidget->children().size(); ++i) {
		if (dynamic_cast<ObjectLabel*>(ui->worldWidget->children().at(i)) != NULL){
			ui->worldWidget->children().at(i)->deleteLater();
		}
	}

	try
	{
		LoadSave::instance().load(filename);
	}
	catch (exception& ex)
	{
		throw runtime_error(ex.what());
	}
	HighScore::instance().LoadScore("data/" + World::instance().getLevelName());
	World::instance().getScreen()->setScreenSize(ui->worldWidget->geometry().width(), ui->worldWidget->geometry().height());
	ui->lblBackground->setPixmap(QPixmap(World::instance().getBackgroundPath()));

	Player* player = World::instance().getPlayer();
	lblPlayer = new ObjectLabel(ui->worldWidget);
	lblPlayer->setObject(player);
	lblPlayer->setPixmap(QPixmap(player->getImage()));
	lblPlayer->setScaledContents(true);
	lblPlayer->show();
	lblPlayer->updateLabelPosition();

	for (Object* worldObj : World::instance().getObjects())
	{
		ObjectLabel* label = new ObjectLabel(ui->worldWidget);
		label->setObject(worldObj);
		label->updateLabelPosition();
		label->setScaledContents(true);
		label->setPixmap(QPixmap(worldObj->getImage()));
		if (label->getObject()->getVisibility())
			label->show();
	}

	if (lblPlayer != NULL)
	{
		lblPlayer->raise();
		labelPlayer = lblPlayer;
	}
    ui->lblPowerups->raise();
    ui->lblPowerJump->raise();
    ui->lblPowerScore->raise();
    ui->lblPowerShield->raise();
    ui->lblPowerSpeed->raise();
	ui->lblLife1->show();
	ui->lblLife2->show();
	ui->lblLife3->show();

    ui->lblHealth->raise();
	ui->lblLife1->raise();
	ui->lblLife2->raise();
	ui->lblLife3->raise();
	ui->lblScore->raise();
    ui->lblTimeLeft->raise();
    ui->PBpause->raise();

	ui->lblCheat->hide();

	World::instance().setSeconds(World::instance().getSeconds());
	World::instance().setCurrentLevel(filename);

	ui->lblTimeLeft->setText(QDateTime::fromTime_t(World::instance().getSeconds()).toUTC().toString("m:ss"));
}

void MainWidget::RotateTimerHit() {

    foreach (QObject * o , ui->worldWidget->children()) {
        QCoreApplication::processEvents();
        ObjectLabel * object = dynamic_cast<ObjectLabel*>(o);
        if (object != NULL){
            Object * obj = object->getObject();
            if (dynamic_cast<Coin*>(obj) != nullptr || dynamic_cast<EndGameObject*>(obj) != nullptr) {
                if (!QRect(obj->getX(),obj->getY(),obj->getWidth(),obj->getHeight()).intersects(World::instance().getCurrentScreen())) {
                    continue;
                }
                //replace moveCoin
                obj->move();
                object->setPixmap(QPixmap(obj->getImage()));
            }
         }
     }
}
void MainWidget::timerHit(){

	//program 4 code below (for reference)
    World& world = World::instance();
	Player* player = world.getPlayer();

	if ((right && left) || (!right && !left)) {
		// if both right and left arrows are held down or both are released slow the player to a stop
		player->slowToStop();
		player->setCount(0);
	} else if (right) {
		// if the right arrow is pressed the player goes right
		if (player->getCanMove())
			player->moveRight();
	} else if (left) {
		// if the left arrow is pressed the player goes left
		if (player->getCanMove())
			player->moveLeft();
	}

	// updates player's position in the model
    MoveThread * playerMove = new MoveThread(player);
    playerMove->start();
    player->setWalkImage();

	ui->lblPowerJump->setVisible(player->powerJump());
	ui->lblPowerSpeed->setVisible(player->powerSpeed());
	ui->lblPowerShield->setVisible(player->powerShield());
	ui->lblPowerScore->setVisible(player->powerScore());

	vector<MoveThread*> moveThreads;
	for (size_t i = 0; i < world.getObjects().size(); i ++)
	{
        QCoreApplication::processEvents();
        Object* obj = world.getObjects().at(i);
        if(dynamic_cast<Enemy*>(obj) != NULL || dynamic_cast<Platform*>(obj) != NULL){
            MoveThread* currentThread = new MoveThread(world.getObjects().at(i));
            moveThreads.push_back(currentThread);
            currentThread->start();
        }
	}

    playerMove->wait();
    delete playerMove;

    // update screen location based on player location
    ScreenMoveThread * screenMove = new ScreenMoveThread();
    screenMove->start();

	for (size_t i = 0; i < moveThreads.size(); ++i) {
		QCoreApplication::processEvents();
		MoveThread* currentThread = moveThreads.at(i);
		currentThread->wait();
		delete currentThread;
	}

	CheckPlayerCollisionThread* playerCollide = new CheckPlayerCollisionThread();
	playerCollide->start();

    screenMove->wait();
    delete screenMove;

	for (int i = 0; i < ui->worldWidget->children().length(); i++)
	{
		QCoreApplication::processEvents();
		ObjectLabel * guiObject = dynamic_cast<ObjectLabel*>(ui->worldWidget->children().at(i));
		if (guiObject != NULL) {
            Object * obj = guiObject->getObject();
            if(QRect(obj->getX(),obj->getY(),obj->getWidth(),obj->getHeight()).intersects(world.getCurrentScreen())){
                // updates the position of each label to the position of its object in the model
                guiObject->updateLabelPosition();
                // showCoin method replacement
                if (guiObject->getObject()->getVisibility() == true) {
                    guiObject->show();
                } else {
                    guiObject->hide();
                }
            }
		}
	}
	playerCollide->wait();
	if (playerCollide->getDeath()) {
		death(player);
	}
	delete playerCollide;

	ui->lblScore->setText(QString::number(World::instance().getScore()));

	if (player->getIsAtEndOfLevel()) {
		death(player);
	}

	if (player->getBottomPoint() > World::instance().getScreen()->getLevelHeight() || World::instance().getSeconds() == 0 )
	{
		if (world.getCheat())
			return;
		death(player);
		if (player->getNumLives() != 0)
			resetPlayer(player);
	}

    labelPlayer->setPixmap(player->getImage());
}

void MainWidget::clockHit()
{
	if (!World::instance().getCheat())
		World::instance().setSeconds(World::instance().getSeconds() - 1);
	ui->lblTimeLeft->setText(QDateTime::fromTime_t(World::instance().getSeconds()).toUTC().toString("m:ss"));
	ui->lblCheat->hide();
	if (World::instance().getCheat())
	{
		ui->lblTimeLeft->setText(QString(""));
		ui->lblTimeLeft->setPixmap(QString(":/images/infinity.png"));
		ui->lblTimeLeft->setScaledContents(true);

		ui->lblCheat->show();
	}
	if (World::instance().getSeconds() == 0)
	{
		death(World::instance().getPlayer());
		if (World::instance().getPlayer()->getNumLives() != 0)
			resetPlayer(World::instance().getPlayer());
	}

	// decrement powerup times
	if (!World::instance().getCheat())
	{
		Player* player = World::instance().getPlayer();
		if (player->powerJump())
		{
			player->getPowerTime("jump") -= 1;
			if (player->getPowerTime("jump") <= 0)
				player->setPower("jump", false);
		}
		if (player->powerSpeed())
		{
			player->getPowerTime("speed") -= 1;
			if (player->getPowerTime("speed") <= 0)
				player->setPower("speed", false);
		}
		if (player->powerShield())
		{
			player->getPowerTime("shield") -= 1;
			if (player->getPowerTime("shield") <= 0)
				player->setPower("shield", false);
		}
		if (player->powerScore())
		{
			player->getPowerTime("score") -= 1;
			if (player->getPowerTime("score") <= 0)
				player->setPower("score", false);
		}
	}
}

void MainWidget::resetPlayer(Player* player)
{
	player->setX(player->getStartX());
	player->setY(player->getStartY());
	if (player->getNumLives() > 0)
		World::instance().setScore(0);
	ui->lblScore->setText("0");
	World::instance().getScreen()->setLocation(0, 0);
	clock->stop();
    RotateTimer->stop();
	World::instance().setSeconds(World::instance().getStartSeconds() + 1);
	clockHit();
	clock->start();
    RotateTimer->start();

	for (Object* worldObj : World::instance().getObjects()) {

		Coin * coin = dynamic_cast<Coin*>(worldObj);
		if (coin != NULL) {
			coin->setVisibility(true);
			coin->setisCollectible(true);
		}

        Enemy* enemy = dynamic_cast<Enemy*>(worldObj);
        if (enemy != NULL)
            enemy->setVisibility(true);

        Collectible* col = dynamic_cast<Collectible*>(worldObj);
        if (col != NULL)
            col->setVisibility(true);
    }

    player->setPower("speed",false);
    player->setPower("shield",false);
    player->setPower("jump",false);
    player->setPower("score",false);

    for (int i = 0; i < ui->worldWidget->children().length(); i++)
    {
        QCoreApplication::processEvents();
        ObjectLabel * guiObject = dynamic_cast<ObjectLabel*>(ui->worldWidget->children().at(i));
		if (guiObject != NULL) {
            // updates the position of each label to the position of its object in the model
            guiObject->updateLabelPosition();
            // showCoin method replacement
            if (guiObject->getObject()->getVisibility() == true) {
                guiObject->show();
            } else {
                guiObject->hide();
            }
        }
    }
}

void MainWidget::death(Player* player)
{
   QSoundEffect * deathSound = new QSoundEffect;
	player->setNumLives(player->getNumLives() - 1);

	if (player->getNumLives() > 0 && !player->getIsAtEndOfLevel()) {
		if (player->getNumLives() == 2){
			ui->lblLife3->hide();
		} else if (player->getNumLives() == 1){
			ui->lblLife2->hide();
		}
        deathSound->setSource(QUrl::fromLocalFile(":/images/gruntsound.wav"));
        if (player->getBottomPoint() > World::instance().getScreen()->getLevelHeight())
             deathSound->setSource(QUrl::fromLocalFile(":/images/deathsound.wav"));
		deathSound->play();

	} else {
		if (!player->getIsAtEndOfLevel())
		{
			ui->lblLife1->hide();
			if (player->isRight())
				player->setImage(":/images/maincharacter/hurt.png");
			else
				player->setImage(":/images/maincharacter/hurtleft.png");
		}
        deathSound->setSource(QUrl::fromLocalFile(":/images/deathsound.wav"));
        deathSound->play();
		timer->stop();
		clock->stop();
        RotateTimer->stop();
		EndGame * e = new EndGame(this, !player->getIsAtEndOfLevel());
		e->show();
		e->raise();
    }
}

// displays all the coins in the world if the player has lives left
void MainWidget::showCoin() {
	for (Object* worldObj : World::instance().getObjects()) {

		Coin * coin = dynamic_cast<Coin*>(worldObj);
		if (coin != NULL) {

			int coinId = worldObj->getId();
			ObjectLabel * lbl;

			for (int i = 0; i < ui->worldWidget->children().length(); i++ ) {
				QCoreApplication::processEvents();
				lbl = dynamic_cast<ObjectLabel*>(ui->worldWidget->children().at(i));

				if (lbl != NULL) {
					if (lbl->getId() == coinId){
						if (coin->getVisibility() == true) {
							lbl->show();
						} else {
							lbl->hide();
						}
					}
				}
			}
		}
	}
}

void MainWidget::moveCoinOrFlag() {
    for (Object* worldObj : World::instance().getObjects()) {
        Coin * coin = dynamic_cast<Coin*>(worldObj);
        if (coin != nullptr) {
            coin->move();
        }
        EndGameObject * flag = dynamic_cast<EndGameObject*>(worldObj);
        if(flag != nullptr) {
            flag->move();
        }
    }
}

MainWidget::~MainWidget() {
	ui->worldWidget->deleteLater();
	delete ui;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
	Player* player = World::instance().getPlayer();

	if (event->key() == Qt::Key_Left) {
		this->left = true;
        player->setRight(false);
        if (player->isRight())
            player->setXSpeed(-1);
	} else if (event->key() == Qt::Key_Right) {
		this->right = true;
        player->setRight(true);
        if (!player->isRight())
            player->setXSpeed(-1);
	} else if (event->key() == Qt::Key_Space || event->key() == Qt::Key_Up) {
		player->setJumping(true);
        player->setJumpOnMove(true);
	}
	else if (event->key() == Qt::Key_A)
	{
		if (player->canKick())
		{
			player->setKicking(true);
			player->setCanKick(false);
        }
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Left) {
        this->left = false;
	} else if (event->key() == Qt::Key_Right) {
        this->right = false;
    }
}

void MoveThread::run()
{
	object->move();
}


void CheckPlayerCollisionThread::run()
{
    Object* obj;
    for(size_t i = 0; i < World::instance().getObjects().size(); ++i) {
        obj = World::instance().getObjects().at(i);
        if (!QRect(obj->getX(),obj->getY(),obj->getWidth(),obj->getHeight()).intersects(World::instance().getCurrentScreen())) {
            continue;
        }
        // checks to see if player the player collides with each object
        CollisionDetails* collision = World::instance().getPlayer()->checkCollision(obj);
		if (collision != NULL) {
			World::instance().getPlayer()->collide(collision);
			if (dynamic_cast<Enemy*>(collision->getCollided()))
				if (collision->getCollided()->getVisibility() && World::instance().getPlayer()->powerShield() == false && World::instance().getCheat() == false)
					death = true;
		}
		delete collision;
	}
}

void MainWidget::on_PBpause_clicked()
{
	if (!timer->isActive())
		// animation timer stopped means already paused
		return;
	timer->stop();
	clock->stop();
    RotateTimer->stop();
	PauseScreen* pause = new PauseScreen(this);
	connect(pause, &PauseScreen::resumeClicked, this, &MainWidget::on_resumeFromPause);
	connect(pause, &PauseScreen::restartClicked, this, &MainWidget::on_restartFromPause);
	pause->show();
	pause->raise();
}

void MainWidget::on_resumeFromPause()
{
	ui->worldWidget->setFocus();
	timer->start();
	clock->start();
    RotateTimer->start();
}

void MainWidget::on_restartFromPause()
{
	loadLevel(World::instance().getCurrentLevel());
	ui->worldWidget->setFocus();
	timer->start();
	clock->start();
    RotateTimer->start();
}

void MainWidget::on_loadState(QString filename)
{
	loadLevel(filename);
}


void ScreenMoveThread::run()
{
    Player* player = World::instance().getPlayer();
    // update screen location based on player location
    PlayingScreen* screen = World::instance().getScreen();
    if (player->getX() - screen->getX() > screen->getCenterX(player->getWidth())
            && (screen->getX() + screen->getScreenWidth()) < screen->getLevelWidth())
    {
        screen->setX(min(player->getX() - screen->getCenterX((player->getWidth())), screen->getLevelWidth() - screen->getScreenWidth()));
    }
    else if (player->getX() - screen->getX() < screen->getCenterX(player->getWidth())
             && screen->getX() > 0)
    {
        screen->setX(max(player->getX() - screen->getCenterX((player->getWidth())), 0));
    }

    if (player->getY() - screen->getY() > screen->getCenterY(player->getHeight())
            && (screen->getY() + screen->getScreenHeight()) < screen->getLevelHeight())
    {
        screen->setY(min(player->getY() - screen->getCenterY((player->getHeight())), screen->getLevelHeight() - screen->getScreenHeight()));
    }
    else if (player->getY() - screen->getY() < screen->getCenterY(player->getHeight())
             && screen->getY() > 0)
    {
        screen->setY(max(player->getY() - screen->getCenterY((player->getHeight())), 0));
    }

    World::instance().setCurrentScreen(QRect(screen->getX() - 20, screen->getY() - 20, screen->getScreenWidth() + 40, screen->getScreenHeight() + 40));
}
