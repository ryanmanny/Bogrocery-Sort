#include "Object.h"


typedef enum dir
{
	NORTH, SOUTH, EAST, WEST;
} Dir;


class Customer : public Object
{
public:
	Customer(Type newType, char newGraphic, position = {0,0}) : Object(newType, newGraphic, position)
	{
		dir = uniform_int_distribution<int>(0,3); //four directions
	}

	void pickup(int newElement)
	{
		//pickup
		element = newElement;
	}

	void update(Board & store)
	{
		move();
		dir = newDir();
	}

	void move(Board & store)
	{
		//calculates next position based on direction and current position
		Pos nextPos = this->position;

		if (direction == Dir::NORTH)
			nextPos.y += 1;
		if (direction == Dir::SOUTH)
			nextPos.y -= 1;
		if (direction == Dir::EAST)
			nextPos.x += 1;
		if (direction == Dir::WEST)
			nextPos.x -= 1;

		//check to see what happens at this new position
		Type at = store.at(nextPos);

		//we ain't moving into the wall, OR another customer
		if (at != Type::WALL && at != Type::CUSTOMER)
		{
			if (at == Type::ITEM)
			{
				pickup(store.itemAt(nextPos));
			}
			if (at == Type::CHECKOUT)
			{
				store.checkout.push_back(this->destroy()); //customer leaves, his item gets
			}

			//actually moves the character
			position = nextPos;
		}

	}

	newDir();
	{
		nextDir = dir(rand);
	}

private:
	Dir nextDir;
	int element;

	default_random_engine rand;
	uniform_int_distribution<int> dir;
}
