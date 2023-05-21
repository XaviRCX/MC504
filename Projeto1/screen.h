#ifndef SCREEN_H
#define SCREEN_H
 
void inicialize();
  
void end();

//n = 0 - 3, estacionamento
//n = 4 - 7, posto
//n = 8 passa direto pelas vagas

//posiciona um carro em uma das 8 vagas;
void park(int n, int tank);
 
//remove o carro de uma das 8 vagas
void empty(int n);

//faz a animacao de andar pela rua
void riding(int n, int tank, int time);

//anima o carro enchendo em uma das vagas
void fill(int n, int tank, int time);

#endif
