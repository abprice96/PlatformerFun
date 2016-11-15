#include "coin.h"

//collectible constructor
Coin::Coin(int x_, int y_, int width_, int height_, QString image_, QString type, bool visibility_,bool isCollectible_, int newAmount): Collectible(x_,y_,width_,height_,image_,type, visibility_, isCollectible_)
{
   amount = newAmount;
}

void Coin::load(QString config)
{
	try
	{
		QList<QString> params = config.split(",");
		Object::load(config);
		this->setAmount(Object::getQListElement(params, 6).toInt());
		this->setVisibility(Object::getQListElement(params, 7) == "true");
		this->setisCollectible(Object::getQListElement(params, 7) == "true");
	}
	catch (exception& ex)
	{
		throw invalid_argument(ex.what());
	}
}

QString Coin::save()
{
	QString out = Object::save();
	out += "," + QString::number(this->getAmount());
	out += "," + QString::fromStdString(this->getVisibility() ? "true" : "false");
	return out;
}

// instead of moving the coin, changes the animation frame.
void Coin::move()
{
	count += 10;
	int frame = count / 10;

	image = ":/images/goldCoin/goldCoin" + QString::number(frame) + ".png";
	if (frame == 8)
	{
		image = ":/images/goldCoin/goldCoin1.png";
		count = 10;
	}
}
