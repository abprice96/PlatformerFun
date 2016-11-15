
#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <vector>

#include "object.h"
#include "collectible.h"
#include "coin.h"
#include "player.h"
#include "platform.h"
#include "enemy.h"
#include "playingscreen.h"
#include "endgameobject.h"
#include "QRect"

using namespace std;

class World {
	vector<Object*> objects;
	Player* player;
	PlayingScreen* screen;
    Object* OtherPlayer;

	// singleton instance
	static World world;

	// singleton: private ctors
	World() {}
	World(const World& that) = delete;  // don't allow copying

	int score;
	int seconds;
	int startSeconds;
	QString currentLevel;
	string levelName;
	QString backgroundPath;
	bool cheat;
    bool isPlaying;
    QRect currentScreen;

public:
	Object *createObject(const string& type);

	// adds <obj> to objects in world
	void add(Object *obj) {
		auto it = objects.begin();
		for (; it != objects.end(); ++it) {
			if (obj->getId() < (*it)->getId()) {
				objects.insert(it, obj);
				break;
			}
		}
		if (it == objects.end()) {
			objects.push_back(obj);
		}
	}

	// returns object with <id>, or NULL if none
	Object *getById(int id);

	vector<Object*>& getObjects() { return objects; }
	Player* getPlayer() { return player; }

	void setPlayer(Player* initPlayer) { player = initPlayer;}
	void setSeconds(int initSeconds) { seconds = initSeconds; }
	int getSeconds() { return seconds; }
	void setStartSeconds(int initSeconds) { startSeconds = initSeconds; }
	int getStartSeconds() { return startSeconds; }

	QString getCurrentLevel() {return currentLevel;}
	void setCurrentLevel(QString filename) {currentLevel = filename;}

    bool getIsPlaying(){ return isPlaying; }
    void setIsPlaying(bool newState) { isPlaying = newState; }

    string getLevelName() { return levelName; }
	void setLevelName(string newLevelName) { levelName = newLevelName; }

	QString getBackgroundPath() {return backgroundPath;}
	void setBackgroundPath(QString filename) {backgroundPath = filename;}

    QRect getCurrentScreen() { return currentScreen; }
    void setCurrentScreen(QRect rect) { currentScreen = rect; }


	// Removes object with <id> from objects and returns it, or returns NULL if not found
	Object *destroy(int id);

	// Resets world
	void reset();

	// Destroy objects
	~World();

	// Get singleton
	static World& instance() { return world; }

	PlayingScreen* getScreen() {return screen;}
	void setScreen(PlayingScreen* scr) {screen = scr;}

	int getScore() {return score;}
	void incScore(int addAmount);
	void setScore(int newAmount) {score = newAmount; }

	bool getCheat() { return cheat; }
	void setCheat(bool b) { cheat = b; }
};

#endif // WORLD_H
