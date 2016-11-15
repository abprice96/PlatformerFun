/*
 * loadsave.cpp
 */

#include "world.h"
#include "loadsave.h"
#include "object.h"
#include "highscore.h"

QString getQListElement(QList<QString> theList, int index)
{
    if (index >= theList.size() || index < 0)
        throw invalid_argument("Index out of range.");
    else
        return theList.at(index);
}

LoadSave* LoadSave::loadsave = NULL;

LoadSave& LoadSave::instance()
{
    if (loadsave == NULL)
        loadsave = new LoadSave();
    return *loadsave;
}

// loads a save state or level file and configures objects
void LoadSave::load(QString filename)
{
    World::instance().reset();
    HighScore::instance();
    HighScore::teardown();
    QFile file(filename);
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        size_t numObjs = 0;
        try
        {
            // read lines and configure objects
            QString line = "";
            QTextStream in(&file);

            //read in level name
            line = in.readLine();
            string levelName = line.toStdString();
            levelName += "scores.txt";
            World::instance().setLevelName(levelName);

            //time
            line = in.readLine();
            QList<QString> timeData = line.split(",");
            World::instance().setStartSeconds(getQListElement(timeData, 0).toInt());
            World::instance().setSeconds(getQListElement(timeData, 1).toInt());

            // set up screen object
            line = in.readLine();
            QList<QString> levelDim = line.split(",");
            line = in.readLine();
            QList<QString> screenCoord = line.split(",");
            World::instance().setScreen(new PlayingScreen(
                                            getQListElement(screenCoord, 0).toInt(),
                                            getQListElement(screenCoord, 1).toInt(),
                                            0, // don't know the dimensions of the screen here
                                            0, // will set that in mainwidget.cpp
                                            getQListElement(levelDim, 0).toInt(),
                                            getQListElement(levelDim, 1).toInt()));

            // score
            line = in.readLine();
            World::instance().setScore(line.toInt());

            // background image
            World::instance().setBackgroundPath(in.readLine());

            // player
            Player* player = new Player();
            player->load(in.readLine());
            World::instance().setPlayer(player);

            // loop to get platforms and other objects
            loadObjects(in, numObjs);
        }
        catch (exception& ex)
        {
            QString message = "The level file " + filename + " is not configured properly.";
            throw runtime_error(message.toStdString());
        }

        if (World::instance().getObjects().size() != numObjs)
            throw runtime_error("The program did not load all of the objects!");

        file.close();
        World::instance().setCurrentScreen(QRect(0,0,944,688));
    }
    else
    {
        throw runtime_error("Failure to open level file");
    }
}

// configures objects from a file <in>
// called at beginning of series of objects in a save file
void LoadSave::loadObjects(QTextStream& in, size_t& num)
{
    num = 0;
    QString line = "";
    Object* obj = NULL;
    try
    {
        while (!in.atEnd())
        {
            line = in.readLine();
            obj = World::instance().createObject(getQListElement(line.split(","), 0).toStdString());
            if (obj != NULL)
            {
                obj->load(line);
                World::instance().add(obj);
                num++;
            }
        }
    }
    catch (exception& ex)
    {
        throw runtime_error(ex.what());
    }
}

// reads object data and saves a save state file
void LoadSave::save(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        // level name
        QString levelName = QString::fromStdString(World::instance().getLevelName());
        QString suffix = "scores.txt";
        out << levelName.left(levelName.length() - suffix.length()) << "\n";
        // time
        out << World::instance().getStartSeconds() << "," << World::instance().getSeconds() << "\n";
        // screen
        PlayingScreen* scr = World::instance().getScreen();
        out << scr->getLevelWidth() << "," << scr->getLevelHeight() << "\n";
        out << scr->getX() << "," << scr->getY() << "\n";
        // score
        out << World::instance().getScore() << "\n";
        // background image
        out << World::instance().getBackgroundPath() << "\n";

        // write lines from Object.save() method
        size_t numObjects = 0;
        out << World::instance().getPlayer()->save() << "\n";
        for (Object* obj : World::instance().getObjects())
        {
            out << obj->save() << "\n";
            numObjects++;
        }
        file.close();
        if (numObjects != World::instance().getObjects().size())
        {
            throw runtime_error("The program did not save all of the objects!");
            file.remove();
            return;
        }
    }
    if (!file.exists())
        throw runtime_error("The save file was not created successfully!");
}

void LoadSave::teardown()
{
	if (loadsave != NULL)
	{
		delete loadsave;
		loadsave = NULL;
	}
}
