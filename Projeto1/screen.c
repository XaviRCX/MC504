#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define screenX 147
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
    {0,0,0,0,0,0,0,0,' ',' ',' ',' ',' ',' ',0,0,0,0,0,0,0},
    {0,0,0,0,0,' ','.','-','-','-','-','-','-','-','-','.',' ',0,0,0,0},
    {' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '},
    {'O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'},
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
    for (int x = 60; x < 147; x++) {
        screen[10][x] = '-';
        screen[18][x] = '-';
    }
    for (int y = 11; y < 17; y++) {
        for (int x = 93; x < 97; x++) {
            screen[y][x] = '_';
        }
        for (int x = 138; x < 142; x++) {
            screen[y][x] = '_';
        }
    }
    for (int y = 0; y < screenY; y++) {
        screen[y][0] = '|';
        screen[y][58] = '|';
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
    placeScreen(9, 27, 0, 60, parking_pump);
    placeScreen(9, 27, 20, 60, parking_pump);
    placeScreen(9, 27, 0, 105, parking_pump);
    placeScreen(9, 27, 20, 105, parking_pump);

    placeScreen(9, 14, 0, 88, pump);
    placeScreen(9, 14, 20, 88, pump);
    placeScreen(9, 14, 0, 133, pump);
    placeScreen(9, 14, 20, 133, pump);

    placeScreen(9, 27, 0, 2, parking);
    placeScreen(9, 27, 20, 2, parking);
    placeScreen(9, 27, 0, 30, parking);
    placeScreen(9, 27, 20, 30, parking);
    placeScreen(5, 7, 12, 26, P);
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

void inicialize() {
    restock();    
    initscr();
    printScreen();
}

void end() {
    printScreen();
    getch();
    endwin();
}

void changeLevel(int tank) {
    int c = tank/100;
    int d = tank/10 - c*10;
    int u = tank - c*100 - d*10;

    car[0][9] = c + '0';
    car[0][10] = d + '0';
    car[0][11] = u + '0';
    car[0][12] = 37;
}

void ride(int x, int tank) {
    changeLevel(tank);
    placeScreen(5, 21, 12, x, car);
    printScreen();
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
        placeScreen(5, 21, 2, 5, car);
        break;
    case 1:
        placeScreen(5, 21, 22, 5, car);
        break;
    case 2:
        placeScreen(5, 21, 2, 33, car);
        break;
    case 3:
        placeScreen(5, 21, 22, 33, car);
        break;
    case 4:
        placeScreen(5, 21, 2, 63, car);
        break;
    case 5:
        placeScreen(5, 21, 22, 63, car);
        break;
    case 6:
        placeScreen(5, 21, 2, 108, car);
        break;
    case 7:
        placeScreen(5, 21, 22, 108, car);
        break;
    default:
        break;
    }
    printScreen();
}

void empty(int n) {
    switch (n)
    {
    case 0:
        placeScreen(5, 21, 2, 5, clearCar);
        break;
    case 1:
        placeScreen(5, 21, 22, 5, clearCar);
        break;
    case 2:
        placeScreen(5, 21, 2, 33, clearCar);
        break;
    case 3:
        placeScreen(5, 21, 22, 33, clearCar);
        break;
    case 4:
        placeScreen(5, 21, 2, 63, clearCar);
        break;
    case 5:
        placeScreen(5, 21, 22, 63, clearCar);
        break;
    case 6:
        placeScreen(5, 21, 2, 108, clearCar);
        break;
    case 7:
        placeScreen(5, 21, 22, 108, clearCar);
        break;
    default:
        break;
    }
}

int main() {
    /*
    inicialize();
    for(int i = 0; i < 126; i++){
        ride(i, i);
        usleep(50000);
        emptyRide(i);
    }
    for(int i = 0; i < 8; i++){
        park(i, i*10);
        sleep(1);
        empty(i);
    }
    end();
    */
    return 0;
}#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define screenX 147
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
    {0,0,0,0,0,0,0,0,' ',' ',' ',' ',' ',' ',0,0,0,0,0,0,0},
    {0,0,0,0,0,' ','.','-','-','-','-','-','-','-','-','.',' ',0,0,0,0},
    {' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '},
    {'O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'},
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
    for (int x = 60; x < 147; x++) {
        screen[10][x] = '-';
        screen[18][x] = '-';
    }
    for (int y = 11; y < 17; y++) {
        for (int x = 93; x < 97; x++) {
            screen[y][x] = '_';
        }
        for (int x = 138; x < 142; x++) {
            screen[y][x] = '_';
        }
    }
    for (int y = 0; y < screenY; y++) {
        screen[y][0] = '|';
        screen[y][58] = '|';
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
    placeScreen(9, 27, 0, 60, parking_pump);
    placeScreen(9, 27, 20, 60, parking_pump);
    placeScreen(9, 27, 0, 105, parking_pump);
    placeScreen(9, 27, 20, 105, parking_pump);

    placeScreen(9, 14, 0, 88, pump);
    placeScreen(9, 14, 20, 88, pump);
    placeScreen(9, 14, 0, 133, pump);
    placeScreen(9, 14, 20, 133, pump);

    placeScreen(9, 27, 0, 2, parking);
    placeScreen(9, 27, 20, 2, parking);
    placeScreen(9, 27, 0, 30, parking);
    placeScreen(9, 27, 20, 30, parking);
    placeScreen(5, 7, 12, 26, P);
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

void inicialize() {
    restock();    
    initscr();
    printScreen();
}

void end() {
    printScreen();
    getch();
    endwin();
}

void changeLevel(int tank) {
    int c = tank/100;
    int d = tank/10 - c*10;
    int u = tank - c*100 - d*10;

    car[0][9] = c + '0';
    car[0][10] = d + '0';
    car[0][11] = u + '0';
    car[0][12] = 37;
}

void ride(int x, int tank) {
    changeLevel(tank);
    placeScreen(5, 21, 12, x, car);
    printScreen();
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
        placeScreen(5, 21, 2, 5, car);
        break;
    case 1:
        placeScreen(5, 21, 22, 5, car);
        break;
    case 2:
        placeScreen(5, 21, 2, 33, car);
        break;
    case 3:
        placeScreen(5, 21, 22, 33, car);
        break;
    case 4:
        placeScreen(5, 21, 2, 63, car);
        break;
    case 5:
        placeScreen(5, 21, 22, 63, car);
        break;
    case 6:
        placeScreen(5, 21, 2, 108, car);
        break;
    case 7:
        placeScreen(5, 21, 22, 108, car);
        break;
    default:
        break;
    }
    printScreen();
}

void empty(int n) {
    switch (n)
    {
    case 0:
        placeScreen(5, 21, 2, 5, clearCar);
        break;
    case 1:
        placeScreen(5, 21, 22, 5, clearCar);
        break;
    case 2:
        placeScreen(5, 21, 2, 33, clearCar);
        break;
    case 3:
        placeScreen(5, 21, 22, 33, clearCar);
        break;
    case 4:
        placeScreen(5, 21, 2, 63, clearCar);
        break;
    case 5:
        placeScreen(5, 21, 22, 63, clearCar);
        break;
    case 6:
        placeScreen(5, 21, 2, 108, clearCar);
        break;
    case 7:
        placeScreen(5, 21, 22, 108, clearCar);
        break;
    default:
        break;
    }
}

int main() {
    /*
    inicialize();
    for(int i = 0; i < 126; i++){
        ride(i, i);
        usleep(50000);
        emptyRide(i);
    }
    for(int i = 0; i < 8; i++){
        park(i, i*10);
        sleep(1);
        empty(i);
    }
    end();
    */
    return 0;
}
