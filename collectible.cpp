#include "collectible.h"

//collectible constructor
Collectible::Collectible(int x_, int y_, int width_, int height_, QString image_, QString type_, bool visibility_, bool isCollectible_): Object(x_,y_,width_,height_,image_)
{
   type = type_;
   visibility = visibility_;
   isCollectible = isCollectible_;
}

void Collectible::load(QString config)
{
	try
	{
		Object::load(config);
		QList<QString> params = config.split(",");
		this->setType(Object::getQListElement(params, 6));
		this->setVisibility(Object::getQListElement(params, 7) == "true");
	}
	catch (exception& ex)
	{
		throw invalid_argument(ex.what());
	}
}

QString Collectible::save()
{
	QString out = Object::save();
	out += "," + this->getType();
	out += "," + QString::fromStdString(this->getVisibility() ? "true" : "false");
	return out;
}
