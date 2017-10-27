#include "includes.h"

struct Pos
{
	int x;
	int y;
};

typedef enum type
{
	EMPTY, WALL, CUSTOMER, ITEM, CHECKOUT
	//empty is returned when no item at position, lack of action
}Type;

class Object
{
public:
	Object(Type newType, char newGraphic, Pos newPos)
	{
		position = newPos;
		graphic = newGraphic;
		type = newType;
	}
private:
	Pos position;
	Type type;
	char graphic;
}
