#include "Customer.h"
#include "includes.h"

#define BOARDHEIGHT 12
#define BOARDWIDTH 24

#define GWALL (char) 177
#define GDOOR (char) 176
#define GCHECKOUT 'a'
#define GCUSTOMER '$'

#define CHECKOUTPOS {3, BOARDHEIGHT - 2}
//#define CUSTOMERPOS {BOARDWIDTH - 2, BOARDHEIGHT / 2}
#define CUSTOMERPOS {22,6}
#define DOORPOS {23,6} //customers come out of the door
#define MAXCUSTOMERS ((BOARDWIDTH - 2) * (BOARDHEIGHT - 2)) / 2 //fire marshall code

#define SPAWNLIKELIHOOD 200 //likelihood that a customer will spawn every tick. 0 is guaranteed every tick

class Board
{
public:
	Board()
	{
		rand.seed(std::chrono::system_clock::now().time_since_epoch().count());
		spawn = std::uniform_int_distribution<int>(0,SPAWNLIKELIHOOD);

		numCustomers = 0;

		//adds walls around the border of the board
		for (int i = 0; i < BOARDWIDTH; i++)
		{
			board.push_back(new Object(Type::WALL, GWALL, {i,0}));
			board.push_back(new Object(Type::WALL, GWALL, {i,BOARDHEIGHT - 1}));
		}
		//there's a little overlap but that's okay
		for (int j = 0; j < BOARDHEIGHT; j++)
		{
			board.push_back(new Object(Type::WALL, GWALL, {0, j}));
			board.push_back(new Object(Type::WALL, GWALL, {BOARDWIDTH - 1, j}));
		}

		board.push_back(new Object(Type::WALL, GDOOR, DOORPOS));

		board.push_back(new Object(Type::CHECKOUT, GCHECKOUT, CHECKOUTPOS));

		// printw("About to add customer");
		// getch();

		board.push_back(new Customer(Type::CUSTOMER, GCUSTOMER, -1, CUSTOMERPOS));

		// printw("Added customer");
		// getch();
	}

	void update(void)
	{
		for (Object *o : board)
		{
			//only customers move
			if (o->getType() == Type::CUSTOMER)
			{
				//printw("Updating");
				//getch();
				//all customers do their next action

				moveObject(static_cast<Customer *> (o));

			}
		}

		spawnCustomer(CUSTOMERPOS);
	}

	//only customers can move - I would pass in objects with virtuals but only customers know how to even think about moving
	void moveObject(Customer * obj)
	{
		//calculates next position based on direction and current position
		Pos nextPos = obj->getPos();
		Type at;

		// printw("moving someone");
		// getch();

		//printw("customer is at (%d,%d)", nextPos.x, nextPos.y);

		if (obj->getDirection() == Dir::NORTH)
			nextPos.y += 1;
		if (obj->getDirection() == Dir::SOUTH)
			nextPos.y -= 1;
		if (obj->getDirection() == Dir::EAST)
			nextPos.x += 1;
		if (obj->getDirection() == Dir::WEST)
			nextPos.x -= 1;

		// printw("next pos is (%d,%d)", nextPos.x, nextPos.y);

		//check to see what happens at this new position
		if (atPos(nextPos) != nullptr)
			at = atPos(nextPos)->getType();

		// printw("(%d,%d) has type %d", nextPos.x, nextPos.y, at);
		//
		// printw("About to move");
		// getch();

		//we ain't moving into the wall, OR another customer
		if (at != Type::WALL && at != Type::CUSTOMER)
		{
			// printw("Moving");
			// getch();
			//actually moves the character
			obj->move(nextPos);

			if (at == Type::ITEM)
			{
				obj->pickup(itemAt(nextPos));
			}

			if (at == Type::CHECKOUT)
			{
				checkout.push_back(obj->getElement()); //customer leaves, his item gets
				destroy(obj); //removes object from the game board
			}
		}
		obj->changeDir();
	}

	void print(void)
	{
		erase(); //overlap?
		for (Object *o : board)
		{
			//no protection against negative values may cause trouble
			mvaddch(o->getPos().y, o->getPos().x, o->getGraphic());
		}
		move(BOARDHEIGHT - 1, BOARDWIDTH - 1);
		refresh();
	}

	Object * atPos(const Pos & position)
	{
		int i = 0;
		for (Object *o : board)
		{
			if (o->getPos() == position)
			{
				//printw("finding object");
				//getch();
				return board.at(i);
			}
			i++;
		}
		return nullptr; //nothing was found there
	}

	const int itemAt(const Pos & position)
	{
		for (Object *o : board)
		{
			if (o->getPos() == position)
			{
				return o->getElement();
			}
		}
	}

	void destroy(const Object * obj)
	{
		numCustomers--;
		auto it = find(board.begin(), board.end(), obj);
		delete obj;
		board.erase(it);
	}

	bool isSorted(void)
	{
		if (is_sorted(checkout.begin(), checkout.end()))
			return true;
		else
			return false;
	}

	//returns true if customer successfully spawned
	bool spawnCustomer(Pos position)
	{
		if (spawn(rand) == 1) //this tick will spawn a customer
		{
			// printw("Spawn tick");
			// getch();
			if (numCustomers < MAXCUSTOMERS && atPos(position) == nullptr) //store isn't full and someone isn't already standing by the door
			{
				// printw("Trying to spawn customer");
				// getch();
				board.push_back(new Customer(Type::CUSTOMER, GCUSTOMER, -1, position));
				numCustomers++;
				return true;
			}
		}
		else
			return false;
	}

	~Board()
	{
		auto it = board.begin();
		while (it != board.end())
		{
			delete *it; //frees ye objects
		}
	}

private:
	//two dimensional array of objects
	//Object board[BOARDHEIGHT][BOARDWIDTH]; //WRONGO DONGO

	std::vector<Object *> board;
	std::vector<int> checkout; //where the items go

	std::default_random_engine rand;
	std::uniform_int_distribution<int> spawn;

	int numCustomers;
};
