#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

#define screenX 147
#define screenY 29

//matriz usada para animar o posto
char screen[screenY][screenX];

//modelo da bomba de combustivel
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

//modelo do carro
char car[5][21] = {
    {0,0,0,0,0,0,0,0,' ',' ',' ',' ',' ',' ',0,0,0,0,0,0,0},
    {0,0,0,0,0,' ','.','-','-','-','-','-','-','-','-','.',' ',0,0,0,0},
    {' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '},
    {'O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'},
    {' ','\'','-','-','(','_',')','-','-','-','-','-','-','-','(','_',')','-','-','\'',' '}
};

//objeto para remover o carro da tela
char clearCar[5][21] = {
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '}
};

//modelo de P do estacionamento
char P[5][7] = {
    {'#','#','#','#','#','#','#'},
    {'#','#',' ',' ',' ','#','#'},
    {'#','#','#','#','#','#','#'},
    {'#','#',' ',' ',' ',' ',' '},
    {'#','#',' ',' ',' ',' ',' '}
};

//modelos de vagas para o carro
char parking_pump[9][27];
char parking[9][27];

//coloca um objeto na matriz da tela
void placeScreen(int sizeY, int sizeX, int sY, int sX, char O[sizeY][sizeX]) {
    for (int y = 0; y < sizeY; y++) {
        for (int x = 0; x < sizeX; x++) {
            if (O[y][x] != 0) {
                screen[sY + y][sX + x] = O[y][x];
            }
        }
    }
}

//posiciona alguns elementos basicos do design da tela
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
    //cria o design das vagas
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

//imprime a matriz da tela com a biblioteca ncurses
void printScreen() {
    erase();
    for (int y = 0; y < screenY; y++) {
        for (int x = 0; x < screenX; x++) {
            move(y, x);
            printw("%c", screen[y][x]);
        }
    }
    refresh();
};

//inicializa a tela
void inicialize() {
    restock();    
    initscr();
    printScreen();
}

//finaliza a tela
void end() {
    printScreen();
    getch();
    endwin();
}

//troca o valor do tanque mostrada pelo carro
void changeLevel(int tank) {
    int c = tank/100;
    int d = tank/10 - c*10;
    int u = tank - c*100 - d*10;

    car[0][9] = c + '0';
    car[0][10] = d + '0';
    car[0][11] = u + '0';
    car[0][12] = 37;
}

//anda o carro na rua na posicao x pelo tempo time
void ride(int x, int tank, int time) {
    changeLevel(tank);
    placeScreen(5, 21, 12, x, car);
    usleep(time);
    placeScreen(5, 21, 12, x, clearCar);
    //reposiciona alguns elementos basicos do design da tela
    restock();
}

//estaciona o carro em uma vaga
//0 - 3, estacionamento
//4 - 7, posto
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
}

//remove o carro de uma vaga
//0 - 3, estacionamento
//4 - 7, posto
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

//anima o carro andando pela rua
void riding(int n, int tank, int time) {
    //ate 2 primeiras vagas
    for (int x = 0; x < 6; x++) {
        ride(x, tank, time);
    }
    //ate 4 primeiras vagas
    if (n > 1) {
        for (int x = 6; x < 34; x++) {
            ride(x, tank, time);
        }
    }
    //ate 6 primeiras vagas
    if (n > 3) {
        for (int x = 34; x < 64; x++) {
            ride(x, tank, time);
        }
    }
    //ate 8 primeiras vagas
    if (n > 5) {
        for (int x = 64; x < 109; x++) {
            ride(x, tank, time);
        }
    }
    //ate o fim da rua
    if (n > 7) {
        for (int x = 109; x < 127; x++) {
            ride(x, tank, time);
        }
    }
}

//anima o carro enchendo
void fill(int n, int tank, int time) {
    for (int i = tank; i < 101; i++) {
        park(n, i);
        usleep(time);
        empty(n);
    }
}

//funcao de teste
void screenTest() {
    inicialize();
    for (int n = 0; n < 9; n++) {
        riding(n, 44, 50000);
        if (n < 4) {
            park(n, 44);
            sleep(1);
            empty(n);
        }
        else if (n < 8) {
            fill(n, 44, 50000);
        }
    }
    end();
}
