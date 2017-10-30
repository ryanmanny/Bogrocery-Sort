#include "includes.h"
#include "Board.h"

#define SPEED 40000 //time to wait between ticks in microseconds

int main (void)
{
	initscr();
	cbreak(); //still allows CTRL-C I hope - it does
	noecho(); //users can't see the characters that they type
	keypad(stdscr, TRUE); //arrow keys awesome

	Board store;

	bool sorted = false;

	std::vector<int> sortedList;

	while (!sorted)
	{
		sorted = store.update();
		//printw("WAH WAH");
		usleep(SPEED);
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
