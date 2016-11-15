#ifndef COIN_H
#define COIN_H

#include"object.h"
#include"collectible.h"

class Coin : public Collectible
{
private:
   int amount;
public:

	Coin () : Collectible() {}
	Coin(int x_, int y_, int width_, int height_, QString image_, QString type_, bool visibility_, bool isCollectible_, int newAmount);


	virtual void load(QString config);
	virtual QString save();
    void move();

	int getAmount() { return amount; }
	void setAmount(int newAmount) { amount = newAmount; }
};

#endif // COIN_H
