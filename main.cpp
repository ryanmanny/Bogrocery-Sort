#include "includes.h"
#include "Board.h"

int main (void)
{
	initscr();
	cbreak(); //still allows CTRL-C I hope
	noecho(); //user can't see the characters she types (lol)
	keypad(stdscr, TRUE); //arrow keys awesome

	Board store;
	while (true)
	{
		store.update();
		//printw("WAH WAH");
		usleep(5000);
		store.print();
	}

	endwin();
}
