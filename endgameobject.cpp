#include "endgameobject.h"

void EndGameObject::load(QString config)
{
	try
	{
		Object::load(config);
	}
	catch (exception& ex)
	{
		throw invalid_argument(ex.what());
	}
}

QString EndGameObject::save()
{
	return Object::save();
}

void EndGameObject::move() {
    count += 1;

    if (count == 1) {
        image = ":/images/flag.png";
    }
    if (count == 2) {
        image = ":/images/flag2.png";
        count = 0;
    }
}
