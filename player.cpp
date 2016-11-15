#include "player.h"
#include "platform.h"
#include "enemy.h"
#include "coin.h"
#include "world.h"

#include <QMediaPlayer>
#include <QTimer>

QString Player::save()
{
    QString out = Object::save();
    out += "," + QString::number(this->getStartX());
    out += "," + QString::number(this->getStartY());
    out += "," + QString::number(this->getXSpeed());
    out += "," + QString::number(this->getYSpeed());
    out += "," + QString::number(this->getNumLives());
    out += "," + QString::fromStdString(this->powerJump() ? "true" : "false");
    out += "," + QString::number(this->getPowerTime("jump"));
    out += "," + QString::fromStdString(this->powerJump() ? "true" : "false");
    out += "," + QString::number(this->getPowerTime("speed"));
    out += "," + QString::fromStdString(this->powerJump() ? "true" : "false");
    out += "," + QString::number(this->getPowerTime("shield"));
    out += "," + QString::fromStdString(this->powerJump() ? "true" : "false");
    out += "," + QString::number(this->getPowerTime("score"));
    return out;
}

void Player::load(QString config)
{
    try
    {
        Object::load(config);
        QList<QString> params = config.split(",");
        // set object properties specific to Player
        this->setStartPoint(Object::getQListElement(params, 6).toInt(), Object::getQListElement(params, 7).toInt());
        this->setXSpeed(Object::getQListElement(params, 8).toInt());
        this->setYSpeed(Object::getQListElement(params, 9).toInt());
        this->setCanMove(true);
        this->setNumLives(Object::getQListElement(params, 10).toInt());
        this->setPower("jump",Object::getQListElement(params, 11) == "true");
        this->getPowerTime("jump") = Object::getQListElement(params, 12).toInt();
        this->setPower("speed",Object::getQListElement(params, 13) == "true");
        this->getPowerTime("speed") = Object::getQListElement(params, 14).toInt();
        this->setPower("shield",Object::getQListElement(params, 15) == "true");
        this->getPowerTime("shield") = Object::getQListElement(params, 16).toInt();
        this->setPower("score",Object::getQListElement(params, 17) == "true");
        this->getPowerTime("score") = Object::getQListElement(params, 18).toInt();
        this->setCanKick(true);
        this->setKicking(false);
        this->setVisibility(true);
    }
    catch (exception& ex)
    {
        throw invalid_argument(ex.what());
    }
}

void Player::jump()
{
    // player jumps if jumpOnMove is true
    if (jumpOnMove) {
        if (powerjump)
            ySpeed -= 10;
        else
            ySpeed -= 13;
        currentPlatform = NULL;
    }
}

void Player::moveRight()
{
    if (!canMove)
    {
        slowToStop();
        return;
    }

    // player accelerates up to a speed of 8 pixels per timer hit
    if (xSpeed < 9) {
        xSpeed += 1;
    }

}

void Player::moveLeft()
{
    if (!canMove)
    {
        slowToStop();
        return;
    }

    // player accelerates up to a speed of 8 pixels per timer hit
    if (xSpeed > -9) {
        xSpeed += -1;
    }
}

void Player::slowToStop()
{
    // slows the player down to an xSpeed of zero
    if (xSpeed == 0) {
        return;
    } else if(xSpeed > 0) {
        --xSpeed;
        return;
    } else {
        ++xSpeed;
    }
}

void Player::move()
{
    // if a player is on a platform and jumpmove is true the player jumps
    if (jumpOnMove) {
        if (!powerjump)
        {
            if (onPlatform || World::instance().getCheat())
                jump();
        }
        else
            jump();
    }

    // if onPlatform, get that platform and add its speed to player's
    // for moving platforms
    if (currentPlatform != NULL)
    {
        x += currentPlatform->getXSpeed();
        y += currentPlatform->getYSpeed();
    }

    // gravity accelerates the player down 1 pixel per timer hit
    ++ySpeed;
    if (ySpeed > 20) {
        ySpeed = 20;
    } else if (ySpeed < -20) {
        ySpeed = -20;
    }

    double multiplier;
    // updates the x and y coordinates for the player
    if (powerspeed)
        multiplier = 1.5;
    else
        multiplier = 1;
    if (kicking)
        multiplier /= 2;
    if (!canMove)
        multiplier /= 1.5;

    x += xSpeed * multiplier;
    y += ySpeed;

    // sets to false so that a player cannot jump while not on a platform
    jumpOnMove = false;
    onPlatform = false;

    if (y + ySpeed <= 0)
    {
        y = 0;
        ySpeed = 0;
    }
    if (World::instance().getCheat() && getBottomPoint() + ySpeed >= World::instance().getScreen()->getLevelHeight())
    {
        y = World::instance().getScreen()->getLevelHeight() - height;
        ySpeed = 0;
    }

    if (x < 0)
    {
        x = 0;
        xSpeed = 0;
    }
    if (getRightPoint() > World::instance().getScreen()->getLevelWidth())
    {
        x = World::instance().getScreen()->getLevelWidth() - width;
        xSpeed = 0;
    }

    if (!cankick)
    {
        kickingCount ++;
        if (kickingCount == 10)
        {
            kicking = false;
        }
        if (kickingCount == 20)
        {
            cankick = true;
            kickingCount = 0;
        }
    }

    count ++;
    if (count == 15)
        count = 0;
}

void Player::collide(CollisionDetails *details)
{
    if (dynamic_cast<Platform*>(details->getCollided()) != NULL) {
        //BUG: when the player jumps into the right side of a platform, he pops back on top instead of getting blocked.
        //^Small issue, but something to fix pre-production.^

        // if it the Object from the collision is a Platform update the position of the
        // player based on the xStopCollide and yStopCollide values
        if (details->getXStopCollide() != 0) {
            xSpeed = 0;
            x += details->getXStopCollide();
        }
        if (details->getYStopCollide() != 0) {
            ySpeed = 0;
            y += details->getYStopCollide() + details->getCollided()->getYSpeed();
            if (details->getYStopCollide() < 0) {
                // the player is on a platform so onPlatform is true
                onPlatform = true;
                currentPlatform = dynamic_cast<Platform*>(details->getCollided());
            }
        }
        jumping = false;
    } else if (dynamic_cast<Coin*>(details->getCollided()) != NULL) {
        //set coin visibility to false and add to high score in world
        Coin * c = dynamic_cast<Coin*>(details->getCollided());
        c->setVisibility(false);
        if(c->getisCollectible()) {
            coinSound->play();
            World::instance().incScore(c->getAmount());
            c->setisCollectible(false);
        }
    }
    else if (dynamic_cast<Enemy*>(details->getCollided()) != NULL)
    {
        Enemy* en = dynamic_cast<Enemy*>(details->getCollided());

        if (details->getYStopCollide() != 0 && en->getVisibility() == true)
        {
            World::instance().incScore(15);
            ySpeed = -10;
            if (details->getYStopCollide() > 0)
                ySpeed = 5;
            en->setVisibility(false);

           hurtSound->play();

            return;
        }

        if (details->getXStopCollide() > 0 && en->getVisibility() == true)
        {
            if (kicking == true)
            {
                World::instance().incScore(15);
                en->setVisibility(false);
                xSpeed /= 2;
                attackSound->play();
                return;
            }
            if (World::instance().getCheat())
                return;
            x += 5;
            ySpeed = -8;
            xSpeed = 12;
            canMove = false;
            en->setRight(false);
        }
        else if (details->getXStopCollide() < 0 && en->getVisibility() == true)
        {
            if (kicking == true)
            {
                World::instance().incScore(15);
                en->setVisibility(false);
                xSpeed /= 2;
                attackSound->play();
                return;
            }
            if (World::instance().getCheat())
                return;
            x -= 5;
            ySpeed = -8;
            xSpeed = -12;
            canMove = false;
            en->setRight(true);
        }
        else
            canMove = true;

    } else if (dynamic_cast<EndGameObject*>(details->getCollided()) != NULL){
        setAtEndOfLevel(true);
        victorySound->play();

    }
    else if (dynamic_cast<Collectible*>(details->getCollided()) != NULL)
    {
        Collectible* item = dynamic_cast<Collectible*>(details->getCollided());
        if (item->getVisibility() && (item->getType() == "jump" || item->getType() == "speed" || item->getType() == "shield" || item->getType() == "score"))
        {
            if (item->getType() == "jump" && item->getVisibility()) {
                flySound->play();
            } else if (item->getType() == "speed" && item->getVisibility()) {
                runSound->play();
            } else if (item->getType() == "score" && item->getVisibility()) {
                doublePointsSound->play();
            } else if (item->getVisibility()){
                shieldSound->play();
            }
            setPower(item->getType().toStdString(), true);
            item->setVisibility(false);
        }

    }
}

void Player::setPower(string pow, bool is)
{
    int powerTime = 20;
    if (pow == "jump")
    {
        powerjump = is;
        if (is)
            powerjumpTime = powerTime;
    }
    else if (pow == "speed")
    {
        powerspeed = is;
        if (is)
            powerspeedTime = powerTime;
    }
    else if (pow == "shield")
    {
        powershield = is;
        if (is)
            powershieldTime = powerTime;
    }
    else if (pow == "score")
    {
        powerscore = is;
        if (is)
            powerscoreTime = powerTime;
    }
}

int& Player::getPowerTime(string pow)
{
    if (pow == "jump")
        return powerjumpTime;
    else if (pow == "speed")
        return powerspeedTime;
    else if (pow == "shield")
        return powershieldTime;
    else if (pow == "score")
        return powerscoreTime;
    else
        throw runtime_error("Requested time of a powerup that doesn't exist");
}

void Player::setWalkImage()
{
    if (!canMove)
    {
        image = ":/images/maincharacter/hurt";
        if (powershield)
            image += "shield";
        if (!right)
            image += "left";
        image += ".png";

        enableMoveCount ++;
        if (enableMoveCount == 10)
        {
            canMove = true;
            enableMoveCount = 0;
        }

        return;
    }

    if (xSpeed != 0)
    {
        image = ":/images/maincharacter/walk";
        if (count < 7)
        {
            image += "1";
        }
        else
        {
            image += "2";
        }
        if (powerspeed)
            image += "speed";
        if (xSpeed > 0)
            right = true;
        if (xSpeed < 0)
            right = false;
        if (!right)
            image += "left";
        image += ".png";
    }

    if (xSpeed == 0)
    {
        image = ":/images/maincharacter/stand";
        if (powerscore)
            image += "score";
        if (!right)
            image += "left";
        image += ".png";
    }

    if (jumping)
    {
        image = ":/images/maincharacter/jump2";
        if (powerjump)
            image += "wings";
        if (!right)
            image += "left";
        image += ".png";
    }

    if (kicking)
    {
        image = ":/images/maincharacter/kick.png";
        if (!right)
            image = ":/images/maincharacter/kickleft.png";

        return;
    }
}
