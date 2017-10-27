#include <ncurses.h>

int main (void)
{
	initscr();
	printw("Hello dingo");
	refresh();
	endwin();
}
