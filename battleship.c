#include <curses.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>

#define HEIGHT 10
#define WIDTH 20
#define START_Y 8
#define START_X 30
#define N 10

void showMenu(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable);
void drawMenu(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable);
void exitGame(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable);

int check(int y, int x, int length, int vorh, char ** map) {
    
    if (vorh == 1) {
         // daca e cazul sa verificam linia de sus
        if (y > 0) {
            for (int i = x; i < x + length; i++) {
                if (map[y-1][i] != '*') {
                    return 0;
                }
            }
        }

        // daca e cazul sa verificam linia de jos
        if (y < N - 1) {
            for (int i = x; i < x + length; i++) {
                if (map[y+1][i] != '*') {
                    return 0;
                }
            }
        }

        // verificam coloana din fata
        if (x > 0) {
            if (map[y][x-1] != '*') {
                return 0;
            }
        }

        // verificam coloana din spate
        if (x + length  < N ) {
            if (map[y][x + length] != '*') {
                return 0;
            }
        }

        // verificam colt stanga sus
        if (x > 0 && y > 0) {
            if (map[y-1][x-1] != '*') {
                return 0;
            }
        }

        // verificam colt stanga jos
        if (x > 0 && y + 1 < N) {
            if (map[y+1][x-1] != '*') {
                return 0;
            }
        }

        // verificam colt dreapta sus
        if (x + length < N  && y > 0) {
            if (map[y-1][x + length] != '*') {
                return 0;
            }
        }

         // verificam colt dreapta jos
        if (x + length < N  && y + 1 < N) {
            if (map[y+1][x + length] != '*') {
                return 0;
            }
        }

    } else {
        // daca e cazul sa verificam coloana din stanga
        if (x > 0) {
            for (int i = y; i < y + 1; i++) {
                if (map[i][x - 1] != '*') {
                    return 0;
                }
            }
        }

        // daca e cazul sa verificam coloana din dreapta
        if (x < N - 1) {
            for (int i = y; i < y + length; i++) {
                if (map[i][x + 1] != '*') {
                    return 0;
                }
            }
        }

        // verificam linia de sus
        if (y > 0) {
            if (map[y-1][x] != '*') {
                return 0;
            }
        }
        
        // verificam linia de jos
        if (y + length  < N ) {
            if (map[y + length][x] != '*') {
                return 0;
            }
        }

         // verificam colt stanga sus
        if (x > 0 && y > 0) {
            if (map[y-1][x-1] != '*') {
                return 0;
            }
        }

        // verificam colt stanga jos
        if (x > 0 && y + length < N) {
            if (map[y+length][x-1] != '*') {
                return 0;
            }
        }

        // verificam colt dreapta sus
        if (x + 1 < N  && y > 0) {
            if (map[y-1][x + 1] != '*') {
                return 0;
            }
        }

         // verificam colt dreapta jos
        if (x + 1 < N  && y + length < N) {
            if (map[y + length][x + 1] != '*') {
                return 0;
            }
        }

    }

     return 1;
}

void fillMap(int y, int x, int length, int voh, char **map) {
    for (int i = 0; i < length; i++) {
        if (voh == 0) {
            map[y+i][x] = '^';
        } else {
            map[y][x+i] = '<';
        }
    }
}

void generateMap(char ** map) {
     // generam bucata de 4 casute
    int vorh = rand() % 2;
    int x, y;
    x = y = 0;
    
    srand(100);

    // daca e verticala
    if (vorh == 0) {
        y = rand() % (N - 4);
        x = rand() % N;
    } else {
        y = rand() % N; 
        x = rand() % (N - 4);
    }
    fillMap(y, x, 4, vorh, map);

    // generam restul de bucati
    for (int i = 3; i > 0 ; i--) {
        for (int j = 0 ; j < 5 - i; j++) {
            while(1) {
                vorh = rand() % 2;
                if (vorh == 0) {
                    y = rand() % (N - i + 1);
                    x = rand() % N;
                    if (check(y, x, i, vorh, map) == 1) {
                        fillMap(y, x, i, vorh, map);
                        break;
                    }
                } else {
                    y = rand() % N; 
                    x = rand() % (N - i + 1);
                    if (check(y, x, i, vorh, map) == 1) {
                        fillMap(y, x, i, vorh, map);
                        break;
                    }
                }
            }
        }
    }
}

void inspectShot(int y, int x, char ** map) {
    if (map[y][x] == '^' || map[y][x] == '<') {
        map[y][x] = 'D';
    } else {
        map[y][x] = 'L';
    }
}

void clearMap(char ** map) {
     for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            map[i][j] = '*';
        }
    }
}

void freeMap(char **map) {
    for (int j = 0; j < N; j++) {
        free(map[j]);
    }

    free(map);
}

int initColors() {

    if (!has_colors())
    {
        printw("Terminal does not support color");
        getch();
        return -1;
    }

    start_color();

    init_pair(1, COLOR_CYAN, COLOR_WHITE);
    init_pair(2, COLOR_YELLOW, COLOR_MAGENTA);
    init_pair(3, COLOR_RED, COLOR_GREEN);
    init_pair(4, COLOR_BLUE, COLOR_BLUE);
    init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(6, COLOR_WHITE, COLOR_WHITE);
    init_pair(7, COLOR_RED, COLOR_RED);
    init_pair(8, COLOR_GREEN, COLOR_GREEN);

    if (!can_change_color())
    {
        printw("Terminal can not change color");
        getch();
        return -1;
    }

    return 0;

}

int isEmptyMap(char **map) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (map[i][j] != '*') {
                return 0;
            }
        }
    }
    return 1;
}

void drawTables(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable) {
   

    box(leftTable, 0, 0);
    box(rightTable, 0, 0);
               
    refresh();
    wrefresh(leftTable);
    wrefresh(rightTable);

    wattron(rightTable, COLOR_PAIR(4));
 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if(map[j][i] == 'X') {
                mvwprintw(rightTable, j + 1, 2 * i + 1, "XX");
            }
        }
    }

    wattroff(rightTable, COLOR_PAIR(4));
 
    refresh();
    wrefresh(leftTable);
    wrefresh(rightTable);

    keypad(leftTable, true);

    int highlightX = 0;
    int highlightY = 0;

    while(1) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (highlightX == i && highlightY == j) {
                    wattron(leftTable, COLOR_PAIR(6));
                    mvwprintw(leftTable, j + 1, 2 * i + 1, "??");
                    wattroff(leftTable, COLOR_PAIR(6));
                } else {
                    if (randomMap[j][i] == 'D') {
                        wattron(leftTable, COLOR_PAIR(7));
                        mvwprintw(leftTable, j + 1, 2 * i + 1, "??");
                        wattroff(leftTable, COLOR_PAIR(7));
                    } else {
                        
                        if (randomMap[j][i] == 'L') {
                            wattron(leftTable, COLOR_PAIR(8));
                            mvwprintw(leftTable, j + 1, 2 * i + 1, "??");
                            wattroff(leftTable, COLOR_PAIR(8));
                        } else {
                            wattron(leftTable, COLOR_PAIR(5));
                            mvwprintw(leftTable, j + 1, 2 * i + 1, "??");
                            wattroff(leftTable, COLOR_PAIR(5));
                        }
                    }
                }
            }
        }
                    
        int choice = wgetch(leftTable);

        switch(choice)
        {
            case KEY_UP:
                if (highlightY > 0)
                    highlightY--;
                break;
                        
            case KEY_DOWN:
                if (highlightY < N - 1)
                    highlightY++;
                break;

            case KEY_LEFT:
                if (highlightX > 0)
                    highlightX--;
                break;
                        
            case KEY_RIGHT:
                if (highlightX < N - 1)
                    highlightX++;
                break;

            }
            // if user hits enter

            if (choice == (int)'q') {

                wclear(leftTable);
                wrefresh(leftTable);

                wclear(rightTable);
                wrefresh(rightTable);

                refresh();

                showMenu(map, randomMap, menuwin, rightTable, leftTable);
            }

            if (choice == 10) {
                inspectShot(highlightY, highlightX, randomMap);
            }
                    
            refresh();
            wrefresh(leftTable);
            wrefresh(rightTable);
    }                
}


void showMenu(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable) {

   
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    keypad(menuwin, true);
   
    wattron(menuwin, COLOR_PAIR(3));

    int choice;
    int highlight = 0;
    int gamestarted = 0;

    while(!gamestarted)
    {
        if (highlight == 0) {
            wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, 2, 2, "New Game");
            wattroff(menuwin, A_REVERSE);

        }
        else {
            mvwprintw(menuwin, 2, 2, "New Game");
        }

        if (highlight == 1) {
            wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, 4, 2, "Resume Game");
            wattroff(menuwin, A_REVERSE);
        }
        else {
            mvwprintw(menuwin, 4, 2, "Resume Game");
        }
        
        if (highlight == 2) {
            wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, 6, 2, "Quit Game");
            wattroff(menuwin, A_REVERSE);
        }
        else {
            mvwprintw(menuwin, 6, 2, "Quit Game");
        }
        choice = wgetch(menuwin);

        switch(choice)
        {
            case KEY_UP:
                if (highlight > 0)
                    highlight--;
                break;
            
            case KEY_DOWN:
                if (highlight < 2)
                    highlight++;
                break;
        }

        // if user hits enter
        if (choice == 10) {
           
            // if user select quit
            if (highlight == 2) {
                exitGame(map, randomMap, menuwin, rightTable, leftTable);
            }

            if (highlight == 1 && isEmptyMap(randomMap) != 1) {
                keypad(menuwin, false);
                wclear(menuwin);
                clear();
                refresh();
                wrefresh(menuwin);
                gamestarted = 1;
                drawTables(map, randomMap, menuwin, rightTable, leftTable);
            }

            if (highlight == 0) {
                keypad(menuwin, false);
                wclear(menuwin);
                clear();
                refresh();
                wrefresh(menuwin);
                clearMap(randomMap);
                generateMap(randomMap);
                drawTables(map, randomMap, menuwin, rightTable, leftTable);
                gamestarted = 1;
            }
        }
    }

    wattroff(menuwin, COLOR_PAIR(3));
                
}

void exitGame(char **map, char **randomMap, WINDOW *menuwin, WINDOW *rightTable, WINDOW *leftTable) {
   
    freeMap(map);
    freeMap(randomMap);
    werase(menuwin);
    werase(leftTable);
    werase(rightTable);
    delwin(menuwin);
    delwin(rightTable);
    delwin(leftTable);
    endwin();
    exit(0);
}

int main(int argc, char *argv[]) {

    initscr();
    noecho();
    cbreak();

    initColors();

    WINDOW *menuwin = newwin(HEIGHT, WIDTH, START_Y, START_X);
    WINDOW *rightTable = newwin(HEIGHT + 2, WIDTH + 2, START_Y - 1, START_X + 12);
    WINDOW *leftTable = newwin(HEIGHT + 2, WIDTH + 2, START_Y - 1, START_X - 12);

    if (argc == 1) {
        printf("Nu s-au dat argumente de comanda\n");
        return -1;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Nu s-a putut deschide fisierul\n");
        return -1;
    }

    char line[20];

    char **map = malloc(sizeof(char*) * N);
    for (int i = 0; i < N; i++) {
        map[i] = malloc(sizeof(char) * N);
    }

    for (int i = 0; i < N; i++) {
        fscanf(fptr, "%s", line);
        for (int j = 0; j < N; j++) {
            map[i][j] = line[j];
        }
    }
    
    fclose(fptr);

    char **randomMap = malloc(sizeof(char*) * N);
    for (int i = 0; i < N; i++) {
        randomMap[i] = malloc(sizeof(char) * N);
        for (int j = 0; j < N; j++) {
            randomMap[i][j] = '*';
        }
    }

    showMenu(map, randomMap, menuwin, rightTable, leftTable);

    getch();

    endwin();
    
    exitGame(map, randomMap, menuwin, rightTable, leftTable);
    

    return 0;
}