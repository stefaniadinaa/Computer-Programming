build: battleship.c
	gcc -lcurses -Wall -pedantic -o battleship battleship.c -g
clean: 
	rm battleship