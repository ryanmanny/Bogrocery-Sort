#include "includes.h"

class Board;

struct Pos
{
	int x;
	int y;
};

enum Dir
{
	NORTH, SOUTH, EAST, WEST, NONE
};

bool operator==(const Pos &l, const Pos &r)
{
	if (l.x == r.x && l.y == r.y)
		return true;
	else
		return false;
}

typedef enum type
{
	EMPTY, WALL, CUSTOMER, SHELF, CHECKOUT
	//empty is returned when no item at position, lack of action
} Type;

class Object
{
public:
	Object(const Type newType, const char newGraphic, const Pos newPos, const int newElement = -1)
	{
		element = newElement; //will only sort positive integers :( for now :)
		position = newPos;
		graphic = newGraphic;
		type = newType;
	}

	int getElement(void) const
	{
		return element;
	}

	Pos getPos(void) const
	{
		return position;
	}

	Type getType(void) const
	{
		return type;
	}

	const char getGraphic(void) const
	{

		return graphic;
	}

protected:
	int element;
	Pos position;
	Type type;
	char graphic;
};

bool operator==(const Object &l, const Object &r)
{
	if (l.getPos() == r.getPos())
		return true;
	else
		return false;
}
