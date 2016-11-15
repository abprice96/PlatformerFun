#include <QFile>
#include "world.h"

World World::world;

// takes a string <toSplit> and returns a vector of strings delimited by a sequence of characters <delim>
vector<string> split(string toSplit, string delim)
{
	size_t pos = 0;
	size_t oldPos = 0;
	pos = toSplit.find(delim, pos);
	vector<string> items;
	if (pos == string::npos)
	{
		// delimiter not found, return vector with toSplit as the only item
		items.push_back(toSplit);
		return items;
	}
	while (true)
	{
		items.push_back(toSplit.substr(oldPos, pos - oldPos));
		pos += delim.length();
		oldPos = pos;
		pos = toSplit.find(delim, pos);
		if (pos == string::npos)
		{
			items.push_back(toSplit.substr(oldPos, toSplit.find_last_not_of("\n") - oldPos));
			break;
		}
	}
	return items;
}

// returns object with <id>, or NULL if none
Object *World::getById(int id) {
	for (size_t i = 0; i < objects.size(); ++i) {
		Object *obj = objects.at(i);
		if (obj->getId() == id) {
			return obj;
		}
	}
	return NULL;
}

//erases the object with the specified <id> pointer from the world vector, and returns the
//pointer if it is found, or nullptr if not
Object *World::destroy(int id) {
	Object *obj = NULL;

	auto iter = objects.begin();
	while (iter != objects.end() && (*iter)->getId() != id) {
		++iter;
	}

	if (iter != objects.end()) {
		// found it
		obj = *iter;
		objects.erase(iter);
	}
	return obj;
}

void World::incScore(int addAmount)
{
	if (player->powerScore())
		addAmount *= 2;
	score += addAmount;
}

//Resets the world, both by deleting memory and erasing the pointers
//to that memory
void World::reset() {
	
	for (size_t i = 0; i < objects.size(); ++i) {
		delete objects.at(i);
	}
	delete player;
	score = 0;
	seconds = startSeconds;
	objects.clear();
	Object::resetNextId();
	delete screen;

}

//World destructor
World::~World() {
	reset();
}

// factory: creates and returns an Object or derived class, determined by
// <type>. Returns NULL if <type> is not recognized.
Object *World::createObject(const string& type) {
	if (type.find("ob") == 0)
		return new Object;
    //else if (type.find("it") == 0)
    //	return new Item;
	else if (type.find("col") == 0)
		return new Collectible;
	else if (type.find("coi") == 0)
		return new Coin;
	else if (type.find("play") == 0)
		return new Player;
	else if (type.find("plat") == 0)
		return new Platform;
	else if (type.find("ene") == 0)
		return new Enemy;
	else if (type.find("flyingen") == 0)
		return new FlyingEnemy;
	else if (type.find("ob") == 0)
		return new Object;
	else if (type.find("end") == 0)
		return new EndGameObject;
	else
		return NULL;
}
