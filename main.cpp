#include "includes.h"
#include "Board.h"

int main (void)
{
	initscr();
	cbreak(); //still allows CTRL-C I hope
	noecho(); //user can't see the characters she types (lol)
	keypad(stdscr, TRUE); //arrow keys awesome

	Board store;

	bool sorted = false;

	std::vector<int> sortedList;

	while (!sorted)
	{
		sorted = store.update();
		//printw("WAH WAH");
		usleep(500);
		store.print();
	}

	sortedList = store.getCheckout();

	printw("LIST SORTED WHAT A TRIUMPH\n");
	for (int i : sortedList)
	{
		printw("%d ", i);
	}

	getch();
	endwin();
}
