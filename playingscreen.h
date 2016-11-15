/*
 * Class storing data for screen location relative to level
 */

#ifndef PLAYINGSCREEN_H
#define PLAYINGSCREEN_H

class PlayingScreen
{
	int x;
	int y;
	int width, height;
	int lvWidth, lvHeight;

public:
	PlayingScreen() {}
	PlayingScreen(int x_, int y_, int w_, int h_, int lvW_, int lvH_) :
		x(x_), y(y_), width(w_), height(h_), lvWidth(lvW_), lvHeight(lvH_){}

	static void unitTest();

	// getters and setters
	int getX() {return x;}
	int getY() {return y;}
	int getScreenWidth() {return width;}
	int getScreenHeight() {return height;}
	int getLevelWidth() {return lvWidth;}
	int getLevelHeight() {return lvHeight;}
	int getCenterX(int objWidth);
	int getCenterY(int objHeight);

	void setX(int x_) {x = x_;}
	void setY(int y_) {y = y_;}
	void setLocation(int x_, int y_)
	{
		x = x_;
		y = y_;
	}

	void setScreenWidth(int w_) {width = w_;}
	void setScreenHeight(int h_) {height = h_;}
	void setScreenSize(int w_, int h_)
	{
		width = w_;
		height = h_;
	}
	void setLevelWidth(int w_) {lvWidth = w_;}
	void setLevelHeight(int h_) {lvHeight = h_;}
	void setLevelSize(int w_, int h_)
	{
		lvWidth = w_;
		lvHeight = h_;
	}
};

#endif // PLAYINGSCREEN_H
