#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "platform.h"
#include <QSoundEffect>

class Player : public Object
{
    bool jumpOnMove;
    int numLives;
    bool canMove;
    bool reachedEndGame;
    int startX, startY;
    int countJump;

    int enableMoveCount;
    int kickingCount;

    bool kicking;
    bool cankick;
    int xSpeedLimit;
    int standCount;
    bool jumping;

    Platform* currentPlatform;

    // powerup variables
    bool powerjump;
    int powerjumpTime;
    bool powerspeed;
    int powerspeedTime;
    bool powershield;
    int powershieldTime;
    bool powerscore;
    int powerscoreTime;

    QSoundEffect * coinSound;
    QSoundEffect * victorySound;
    QSoundEffect * hurtSound;
    QSoundEffect * attackSound;
    QSoundEffect * shieldSound;
    QSoundEffect * runSound;
    QSoundEffect * flySound;
    QSoundEffect * doublePointsSound;


public:
    Player() : Object(), jumpOnMove(false), numLives(3), canMove(true), reachedEndGame(false), kicking(false), cankick(true), jumping(false), currentPlatform(NULL) {
        coinSound = new QSoundEffect;
        coinSound->setSource(QUrl::fromLocalFile(":/images/coinsound.wav"));
        victorySound = new QSoundEffect;
        victorySound->setSource(QUrl::fromLocalFile(":/images/victorysound.wav"));
        hurtSound = new QSoundEffect;
        hurtSound->setSource(QUrl::fromLocalFile(":/images/punchsound.wav"));
        attackSound = new QSoundEffect;
        attackSound->setSource(QUrl::fromLocalFile(":/images/punchsound.wav"));
        shieldSound = new QSoundEffect;
        shieldSound->setSource(QUrl::fromLocalFile(":/images/shieldpowerupsound.wav"));
        runSound = new QSoundEffect;
        runSound->setSource(QUrl::fromLocalFile(":/images/runpowerupsound.wav"));
        flySound = new QSoundEffect;
        flySound->setSource(QUrl::fromLocalFile(":/images/flyingsound.wav"));
        doublePointsSound = new QSoundEffect;
        doublePointsSound->setSource(QUrl::fromLocalFile(":/images/doublepointssound.wav"));

        enableMoveCount = 0;
        kickingCount = 0;

        powerspeed = false;
        powerscore = false;
        powerjump = false;
        powershield = false;
    }
    Player(int x_, int y_, int width_, int height_, QString image_) :
        Object(x_, y_, width_, height_, image_) {Player();}

	virtual void load(QString config);
    virtual QString save();

    void jump();
    void moveRight();
    void moveLeft();
    void slowToStop();
    void move();
    virtual void collide(CollisionDetails *details);

    int getNumLives() { return numLives; }
    void setNumLives(int newNumLives) { numLives = newNumLives; }
    bool canJumpOnMove() { return jumpOnMove; }
    void setJumpOnMove(bool initJump) { jumpOnMove = initJump; }

    bool getCanMove() { return canMove; }
    void setCanMove(bool b) { canMove = b; }

    bool getIsAtEndOfLevel() { return reachedEndGame; }
    void setAtEndOfLevel(bool stateOfPlayer) { reachedEndGame = stateOfPlayer; }

    int getStartX() {return startX;}
    int getStartY() {return startY;}
    void setStartX(int newX) {startX = newX;}
    void setStartY(int newY) {startY = newY;}
    void setStartPoint(int newX, int newY) {startX = newX; startY = newY;}

    bool powerJump() { return powerjump; }
    bool powerSpeed() { return powerspeed; }
    bool powerShield() { return powershield; }
    bool powerScore() { return powerscore; }
    void setPower(string, bool);
    int& getPowerTime(string);

    bool isKicking() { return kicking; }
    void setKicking(bool b) { kicking = b; }
    bool canKick() { return cankick; }
    void setCanKick(bool b) { cankick = b; }

    void setWalkImage();

    void setJumping(bool b) { jumping = b; }
};

#endif // PLAYER_H
