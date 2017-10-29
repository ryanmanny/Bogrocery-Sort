# Grogosort
A combination of a grocery store simulator and bogosort. Used to learn curses implementation.

# Installation
To run, you must have <ncurses.h> installed on your machine. For Windows you will need to install pdcurses.
To install ncurses on Linux, type

sudo apt-get install ncurses-dev

in your terminal.

# Features
- Successfully sorts lists of positive integers
- Graphical display
- Can sort a four-item list in only 15 minutes!
- O(N... ot guaranteed to halt in finite) time complexity!

# TODO
- Add GUI wrapper and allow users to enter their own datasets
- Add real time mode. Shelves are restocked on Tuesdays
- Template for use with negative integers and other data types
- Change way customers pick items up off the shelf
- Make it clear which customers are holding items (replace '$' with the number, I guess)
- Change sleep function depending on operating system for portability
- Simplify and add combine into one portable .h file for use in other projects, using ncurses windows
