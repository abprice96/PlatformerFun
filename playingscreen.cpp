/*
 * Class storing data for screen location relative to level
 */

#include <cassert>
#include "playingscreen.h"

void PlayingScreen::unitTest()
{
	PlayingScreen* scr = new PlayingScreen(0, 0, 100, 100, 200, 100);
	assert(scr->getX() == 0);
	assert(scr->getY() == 0);
	assert(scr->getCenterX(10) == 45);
	assert(scr->getCenterY(40) == 30);

	scr->setLocation(10, 20);
	assert(scr->getX() == 10);
	assert(scr->getY() == 20);

	scr->setScreenSize(50, 50);
	assert(scr->getCenterX(40) == 5);

	delete scr;
}

int PlayingScreen::getCenterX(int objWidth)
{
	return (int) (0.5 * (width - objWidth));
}

int PlayingScreen::getCenterY(int objHeight)
{
	return (int) (0.5 * (height - objHeight));
}
