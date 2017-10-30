#include "Customer.h"
#include "includes.h"

#define BOARDHEIGHT 12
#define BOARDWIDTH 24

#define GWALL (char) 177
#define GDOOR     ' '
#define GCHECKOUT 'a'
#define GCUSTOMER '$'
#define GSHELF    '#'

#define CHECKOUTPOS {3, BOARDHEIGHT - 2}
//#define CUSTOMERPOS {BOARDWIDTH - 2, BOARDHEIGHT / 2}
#define CUSTOMERPOS {22,6}
#define DOORPOS {23,7} //customers come out of the door

#define SPAWNLIKELIHOOD 50 //likelihood that a customer will spawn every tick. 0 is guaranteed every tick
#define MAXCUSTOMERS 200 //((BOARDWIDTH - 2) * (BOARDHEIGHT - 2)) / 2 //fire marshall code

class Board
{
public:
	Board()
	{
		rand.seed(std::chrono::system_clock::now().time_since_epoch().count());
		spawn = std::uniform_int_distribution<int>(0,SPAWNLIKELIHOOD);

		numCustomers = 0;

		originalList = {0,1}; //default list
		unsortedList = originalList;

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

		//adds the door (that blank space in the right wall), still collidable
		board.push_back(new Object(Type::WALL, GDOOR, DOORPOS));
		//adds checkout counter (the 'a')
		board.push_back(new Object(Type::CHECKOUT, GCHECKOUT, CHECKOUTPOS));

		//adds both rows of shelves
		for (int i = BOARDWIDTH/3; i < 2 * (BOARDWIDTH/3); i++)
		{
			board.push_back(new Object(Type::SHELF, GSHELF, {i, BOARDHEIGHT/3}));
			board.push_back(new Object(Type::SHELF, GSHELF, {i, 2 * (BOARDHEIGHT/3)}));
		}

		// printw("About to add customer");
		// getch();

		//the simulation starts with one customer
		board.push_back(new Customer(Type::CUSTOMER, GCUSTOMER, -1, CUSTOMERPOS));

		// printw("Added customer");
		// getch();
	}

	//returns true if list was successfully sorted
	bool update(void)
	{
		//updates all objects
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

		//spawns a new customer in the store as long as some conditions are satisfied
		spawnCustomer(CUSTOMERPOS);

		//check to see if we're done
		if (checkout.size() == originalList.size())
		{
			if (isSorted())
			{
				//checkout is now the sorted list
				return true;
			}
			else
			{
				unsortedList = originalList; //start over
				checkout.clear();
			}
		}
		return false;
	}

	//only customers can move - I would pass in objects with virtuals but only customers know how to even think about moving
	void moveObject(Customer * obj)
	{
		//next position will store the position that the object intends to move into
		Pos nextPos = obj->getPos();
		Type at;
		int element = -1;

		// printw("moving someone");
		// getch();

		//actually calculates nextPos based on current position and direction
		if (obj->getDirection() == Dir::NORTH)
			nextPos.y += 1;
		if (obj->getDirection() == Dir::SOUTH)
			nextPos.y -= 1;
		if (obj->getDirection() == Dir::EAST)
			nextPos.x += 1;
		if (obj->getDirection() == Dir::WEST)
			nextPos.x -= 1;
		if (obj->getDirection() == Dir::NONE)
			nextPos.x -= 0;
			nextPos.y -= 0;

		//check to see what is at the new position
		if (atPos(nextPos) != nullptr)
			at = atPos(nextPos)->getType();

		//we ain't moving into the wall, OR another customer
		if (at != Type::WALL && at != Type::CUSTOMER && at != Type::SHELF)
		{
			// printw("Moving");
			// getch();
			//actually moves the character
			obj->move(nextPos);

			//customer leaves the store if they reach checkout
			if (at == Type::CHECKOUT)
			{
				destroy(static_cast<Customer *>(obj)); //removes object from the game board
			}
		}

		//used to pick up item off the shelf BUT not move into it
		if (at == Type::SHELF && obj->getElement() == -1 && !unsortedList.empty()) //customer can't pick up two items
		{
			//customer picks last item off the shelf - CHANGE THIS TO RANDOM LATER
			element = unsortedList.back();
			unsortedList.pop_back();
			obj->pickup(element);
		}

		//calculates the object's next direction
		obj->changeDir();
	}

	//prints every object on the board
	void print(void)
	{
		//clears current window
		erase(); //conflicts with namespace std

		for (Object *o : board)
		{
			//no protection against crazy values may cause trouble
			mvaddch(o->getPos().y, o->getPos().x, o->getGraphic());
		}

		//add sorted list to bottom of screen
		move(BOARDHEIGHT + 1, 0);
		for (int i : checkout)
			printw("%d ", i);

		refresh();
	}

	//returns object at a location, used for movement checks
	Object * atPos(const Pos & position)
	{
		int i = 0;
		for (Object *o : board)
		{
			if (o->getPos() == position)
			{
				return board.at(i); //returns the object that was there for further examination
			}
			i++;
		}
		return nullptr; //nothing was found there
	}

	//destroys the customer and adds the item it was holding to the checkout list
	void destroy(const Customer * obj)
	{
		numCustomers--; //customer left the store

		//adds element to sorted list
		if (obj->getElement() != -1)
		{
			// if (obj->getElement() == 33) //don't ask about this bug please
			// {
			// 	printw("33 ALERT!!!");
			// 	getch();
			// }
			checkout.push_back(obj->getElement());
		}

		//removes customer from the board
		auto it = find(board.begin(), board.end(), obj);
		board.erase(it);

		//frees the customer's memory also. we can't let them remember what happened here...
		delete obj;
	}

	//checks to see if the checkout vector is sorted. HUR
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
		bool success = false;
		if (spawn(rand) == 1) //this tick will spawn a customer - changed to 33 for posterity
		{
			// printw("Spawn tick");
			// getch();
			if (numCustomers < MAXCUSTOMERS && atPos(position) == nullptr) //store isn't full and someone isn't already standing by the door
			{
				// printw("Trying to spawn customer");
				// getch();
				board.push_back(new Customer(Type::CUSTOMER, GCUSTOMER, -1, position));
				numCustomers++;
				success = true;
			}
		}
		return success;
	}

	//used to return the sorted list at the end
	std::vector<int> getCheckout(void)
	{
		return checkout;
	}

	//frees all the pointers in the board vector
	~Board()
	{
		auto it = board.begin();
		while (it != board.end())
		{
			delete *it; //frees ye objects
			it++;
		}
	}

private:
	//two dimensional array of objects
	//Object board[BOARDHEIGHT][BOARDWIDTH]; //WRONGO DONGO

	std::vector<Object *> board;
	std::vector<int> originalList; //list to sort
	std::vector<int> unsortedList; //current items remaining in original list
	std::vector<int> checkout; //where the items go

	std::default_random_engine rand;
	std::uniform_int_distribution<int> items;
	std::uniform_int_distribution<int> spawn;

	int numCustomers;
};
