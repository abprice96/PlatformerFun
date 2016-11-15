/*
 * loadsave.h
 */

#ifndef LOADSAVE_H
#define LOADSAVE_H

#include <QFile>
#include <QTextStream>

using namespace std;

class LoadSave
{
	static LoadSave* loadsave;
	LoadSave() {}

public:
	static LoadSave& instance();

	// loads a save state and configures objects
	void load(QString filename);

	// configures objects from a file <in>
	// called at beginning of series of objects in a save file
	void loadObjects(QTextStream& in, size_t &num);

	// reads object data and saves a save state file
	void save(QString filename);

	static void teardown();
};

#endif // LOADSAVE_H
