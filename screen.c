#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define screenX 149
#define screenY 29

char screen[screenY][screenX];

char pump[9][14] = {
    {' ',' ',' ','.','-','\"','\"','\"','\"','-','.',' ',' ',' '},
    {' ',' ',' ','|','=','=',' ',' ','=','=','|','-','.',' '},
    {' ',' ',' ','|','~','~',' ','~','~','~','|','`','\\','\\'},
    {' ',' ',' ','|','L','I','L','I','L','I','|',' ','|','|'},
    {' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|','/','/',' '},
    {' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|','/',' ',' '},
    {' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' '},
    {' ','_','_','|','_','_','_','_','_','_','|','_','_',' '},
    {'[','_','_','_','_','_','_','_','_','_','_','_','_',']'}
};

char car[5][21] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,' ','.','-','-','-','-','-','-','-','-','.',' ',0,0,0,0},
    {' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '},
    {'O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'},
    {' ','\'','-','-','(','_',')','-','-','-','-','-','-','-','(','_',')','-','-','\'',' '}
};

char reverseCar[5][21] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,' ','.','-','-','-','-','-','-','-','-','.',' ',0,0,0,0,0},
    {' ','_','_','_','/',' ','_','_','_','|','_','_','_','_','_','\\','_','_','_','_',' '},
    {'*',',',' ',' ',' ','_',' ',' ',' ','|',' ','-',' ',' ',' ','_',' ',' ',' ',' ','O'},
    {' ','\'','-','-','(','_',')','-','-','-','-','-','-','-','(','_',')','-','-','\'',' '}
};

char clearCar[5][21] = {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

char P[5][7] = {
    {'#','#','#','#','#','#','#'},
    {'#','#',' ',' ',' ','#','#'},
    {'#','#','#','#','#','#','#'},
    {'#','#',' ',' ',' ',' ',' '},
    {'#','#',' ',' ',' ',' ',' '}
};

char parking_pump[9][27];
char parking[9][27];

void placeScreen(int sizeY, int sizeX, int sY, int sX, char O[sizeY][sizeX]) {
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (O[y][x] != 0) {
                screen[sY + y][sX + x] = O[y][x];
            }
        }
    }
}

void restock() {
    //inicializa a tela
    for (int x = 0; x < 90; x++) {
        screen[10][x] = '-';
        screen[18][x] = '-';
    }
    for (int y = 11; y < 17; y++) {
        for (int x = 33; x < 37; x++) {
            screen[y][x] = '_';
        }
        for (int x = 78; x < 82; x++) {
            screen[y][x] = '_';
        }
    }
    for (int y = 0; y < screenY; y++) {
        screen[y][90] = '|';
        screen[y][148] = '|';
    }

    //parking
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 27; x++) {
            if (x == 0 || x == 26 || y == 0 || y == 8) {
                parking_pump[y][x] = '#';
                parking[y][x] = '+';
            }
        }
    }
    
    //posiciona os objetos
    placeScreen(9, 27, 0, 0, parking_pump);
    placeScreen(9, 27, 20, 0, parking_pump);
    placeScreen(9, 27, 0, 45, parking_pump);
    placeScreen(9, 27, 20, 45, parking_pump);

    placeScreen(9, 14, 0, 28, pump);
    placeScreen(9, 14, 20, 28, pump);
    placeScreen(9, 14, 0, 73, pump);
    placeScreen(9, 14, 20, 73, pump);

    placeScreen(9, 27, 0, 92, parking);
    placeScreen(9, 27, 20, 92, parking);
    placeScreen(9, 27, 0, 120, parking);
    placeScreen(9, 27, 20, 120, parking);
    placeScreen(5, 7, 12, 116, P);
}

void inicialize() {
    restock();    
    initscr();
}

void end() {
    getch();
    endwin();
}

void printScreen() {
    erase();
    for (int y = 0; y < screenY; y++) {
        for (int x = 0; x < screenX; x++) {
            move(y, x);
            printw("%c", screen[y][x]);
        }
    }
    refresh();
    restock();
};

void changeLevel(int tank) {
    int c = tank/100;
    int d = tank/10 - c*10;
    int u = tank - c*100 - d*10;

    car[0][9] = c + '0';
    car[0][10] = d + '0';
    car[0][11] = u + '0';
    car[0][12] = 37;

    reverseCar[0][9] = c + '0';
    reverseCar[0][10] = d + '0';
    reverseCar[0][11] = u + '0';
    reverseCar[0][12] = 37;
}

void ride(int x, int tank, bool reverse) {
    changeLevel(tank);
    if (reverse == true) {
        placeScreen(5, 21, 12, x, reverseCar);
    }
    else {
        placeScreen(5, 21, 12, x, car);
    }
}

void emptyRide(int x) {
    placeScreen(5, 21, 12, x, clearCar);
    restock();
}

void park(int n, int tank) {
    changeLevel(tank);
    switch (n)
    {
    case 0:
        placeScreen(5, 21, 2, 3, car);
        break;
    case 1:
        placeScreen(5, 21, 22, 3, car);
        break;
    case 2:
        placeScreen(5, 21, 2, 48, car);
        break;
    case 3:
        placeScreen(5, 21, 22, 48, car);
        break;
    case 4:
        placeScreen(5, 21, 2, 95, car);
        break;
    case 5:
        placeScreen(5, 21, 22, 95, car);
        break;
    case 6:
        placeScreen(5, 21, 2, 123, car);
        break;
    case 7:
        placeScreen(5, 21, 22, 123, car);
        break;
    default:
        break;
    }
}

void empty(int n) {
    switch (n)
    {
    case 0:
        placeScreen(5, 21, 2, 3, clearCar);
        break;
    case 1:
        placeScreen(5, 21, 22, 3, clearCar);
        break;
    case 2:
        placeScreen(5, 21, 2, 48, clearCar);
        break;
    case 3:
        placeScreen(5, 21, 22, 48, clearCar);
        break;
    case 4:
        placeScreen(5, 21, 2, 95, clearCar);
        break;
    case 5:
        placeScreen(5, 21, 22, 95, clearCar);
        break;
    case 6:
        placeScreen(5, 21, 2, 123, clearCar);
        break;
    case 7:
        placeScreen(5, 21, 22, 123, clearCar);
        break;
    default:
        break;
    }
}
