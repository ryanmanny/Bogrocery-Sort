#include "Object.h"

class Customer : public Object
{
public:
	Customer(Type newType, char newGraphic, const int newElement, Pos position = {0,0}) : Object(newType, newGraphic, position, newElement)
	{
		rand.seed(std::chrono::system_clock::now().time_since_epoch().count());
		dir = std::uniform_int_distribution<int>(0,15); //only move quarter of the time
		nextDir = toDir(dir(rand));
		//printw("does dir work %d", dir(rand));
		//getch();

		//printw("nextDir is %d", nextDir);
		//getch();
	}

	void pickup(const int & newElement)
	{
		//pickup
		element = newElement;
		graphic = '@';
	}

	void move(const Pos & nextPos)
	{
		//moves and calculates next direction
		position = nextPos;
		//printw("nextDir is %d", nextDir);
		//getch();
	}

	void changeDir(void)
	{
		nextDir = toDir(dir(rand));
	}

	Dir getDirection(void) const
	{
		//printw("nextDir is %d", nextDir);
		//getch();
		return nextDir;
	}

	Dir toDir(const int & i) const
	{
		if (i == 0)
			return Dir::NORTH;
		if (i == 1)
			return Dir::SOUTH;
		if (i == 2)
			return Dir::EAST;
		if (i == 3)
			return Dir::WEST;

		return Dir::NONE; //spooky case
	}

private:
	Dir nextDir;

	std::default_random_engine rand;
	std::uniform_int_distribution<int> dir;
};
