#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "screen.h"  
#include <time.h>

//parametros n e valor do tank

#define N_CARROS 20
#define N_BOMBAS 4
#define N_VAGAS_ESTAC 4


sem_t sem_n_vagas_desocupadas; /* Conta o numero de vagas desocupadas no estacionamento*/
sem_t sem_bombas_vazias[N_BOMBAS]; /* Indica quais bombas estÃ£o vazias (1=vazia, 0=cheia)*/
sem_t sem_carro_abastecido[N_BOMBAS]; /* Indica se o carro atualmente parado em determinada bomba acabou de abastecer (1=abastecido, 0=nao)*/
sem_t sem_bomba_ocupada[N_BOMBAS]; /* Indica se determinada bomba esta atualmente ocupada por um carro (1=ocupada, 0=vazia)*/

/*
 * Variaveis para guardar estado dos carros/bombas.
 * A - arrived           (carro);  chegou ao posto, mas nao necessariamente entrou em uma vaga
 * CAB - abastecendo  (carro); carro abastecendo
 * E - exit  		 (carro); saindo do posto
 * I - idle          (carro); nao chegou ao posto ainda
 * L - left    		 (carro); ja foi embora do posto completamente
 * W - waiting		 (carro); esta na vaga esperando uma bomba
 * 
 * BAB - abastecendo (bomba); bomba abastecendo
 * D - desocupada 	 (bomba); esta desocupada
 */
typedef enum {A, CAB, E, I, L, W} estado_c;
typedef enum {BAB, D} estado_b;

estado_c estado_carros[N_CARROS];
estado_b estado_bombas[N_BOMBAS];

/*
 * Variaveis para guardar estado das vagas.
 * B - busy
 * F - free
 */
typedef enum {B, F} estado_v;

estado_v estado_vagas[N_VAGAS_ESTAC];
int conteudo_vagas[N_VAGAS_ESTAC]; /* indica qual carro esta em qual vaga*/
int carros_em_bombas[N_BOMBAS]; /* indica qual carro esta em qual bomba*/

/* Por questoes de sincronizacao, apenas uma bomba ou carro pode alterar o estado dos componentes por vez, que sera controlado pelo semaforo a seguir. */
sem_t sem_estados; /* Semaforo para troca de estados. */
int printa = 1;

/* Por questoes de sincronizacao, apenas uma bomba pode indicar que esta vazia por vez, e apenas um carro pode ler essa informacao por vez. 
Na historinha do problema eh como se essa informacao fosse indicada num "painel"
Mas nao precisamos mostrar isso no terminal
*/
sem_t sem_escreve_painel, sem_le_painel; /* Semaforos que controlam quem pode ler o painel e quem pode escrever nele (1=ninguem esta fazendo isso, 0=alguem esta fazendo)*/
int painel; /* Inteiro que mostra o id da bomba que esta indicando que esta vazia*/


int random_number(int lower_limit, int upper_limit) {
    // Obter uma semente (seed) aleatória baseada no tempo atual
    srand(time(NULL));
    
    // Gerar e retornar um número aleatório dentro do intervalo
    return (rand() % (upper_limit - lower_limit + 1)) + lower_limit;
}

void* f_bomba(void *v) {
    int id = *(int*)v;

    while(1) {
        sem_wait(&sem_escreve_painel); /* Espera ate ter o direito de escreveer no painel e pega esse direito (sem_escreve_painel = 0)*/
        painel = id; /* Escreve no painel que esta desocupada*/
        sem_post(&sem_le_painel); /* Permite que algum carro leia o painel (sem_le_painel = 1)*/
        sem_wait(&sem_bomba_ocupada[id]); /* Monitora para ver se alguem entrou na bomba, */
        sem_post(&sem_carro_abastecido[id]); /* Indica que o carro nesta bomba acabou de ser abastecido (sem_carro_abastecido[id] = 1)*/
        sem_wait(&sem_estados); /* Pega o direito de alterar estados*/
        sem_post(&sem_estados); /* Libera direito de alterar estados*/
    }
    return NULL;
}

/* Thread dos carros. */
void* f_carro(void* v) {
    int id = *(int*) v;
    int i, gas;
    int minha_bomba, minha_vaga;
    gas = random_number(10, 70);
    sleep(id % 3);
    
    sem_wait(&sem_estados); /* Pega o direito de alterar estados*/
    /* Carro chegou ao posto (mas nao necessariamente entra em uma vaga) */
    estado_carros[id] = A; /* Arrived no posto*/

    /* Imprime que o carro chegou ao posto, mas nao necessariamente entra em uma vaga */

    //faz o caminho ate a entrada do estacionamento
    
    sem_post(&sem_estados); /* Libera direito de alterar estados*/

    /* Para ser mais justo, chegar ao posto, carros nao vao direto as bombas, mas sim tentam entrar em alguma vaga de espera antes*/
    if (sem_trywait(&sem_n_vagas_desocupadas) == 0) { /* Ve se tem alguma vaga desocupada*/

        sem_wait(&sem_estados);	/* Pega o direito de alterar estados (sem_estados = 0)*/

        estado_carros[id] = W; /* Carro na vaga esperando uma bomba*/
        
        /* Achando uma vaga para o carro */
        for (i = 0; i < N_VAGAS_ESTAC; i++) {
            if (estado_vagas[i] == F) { /* Se vaga esta free*/
                estado_vagas[i] = B; /* Vaga esta busy*/
                conteudo_vagas[i] = id; /* Coloca que ele esta ocupando aquela vaga*/
                minha_vaga = i;
                break;
            }
        }

        /* Imprime com carro dentro da vaga */
        riding(minha_vaga, gas, 30000);
        
        park(minha_vaga, gas);


        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/

        sem_wait(&sem_le_painel); /* Pega o direito de ler o painel (sem_le_painel = 0)*/
        minha_bomba = painel;
        sem_post(&sem_escreve_painel); /* Libera para uma bomba poder mudar o valor do painel (sem_escreve_painel = 1) */

        sem_wait(&sem_bombas_vazias[minha_bomba]); /* Passa a indicar que a bomba nao esta mais vazia (sem_bombas_vazias[minha_bomba] = 0)   SE COMUNICA COM OUTROS CARROS*/
        sem_post(&sem_bomba_ocupada[minha_bomba]); /* Passa a indicar que aquela bomba esta ocupada (sem_bomba_ocupada[minha_bomba] = 1)   SE COMUNICA COM BOMBAS*/
    
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        
        sem_post(&sem_n_vagas_desocupadas); /* Indica que tem mais uma vaga desocupada (sem_n_vagas_desocupadas ++)*/
        /* Altera os estados para o carro abastecer */
        estado_carros[id] = CAB; /* Carro abastecendo*/
        estado_bombas[minha_bomba] = BAB; /* Bomba abastecendo*/
        carros_em_bombas[minha_bomba] = id; /* Guarda que o carro com determinado id esta nesta bomba*/
        estado_vagas[minha_vaga] = F; /* Indica que a vaga esta free*/

        
        /* Imprime que o carro esta na bomba e vai comecar a abastecer. */
        sleep(1);
        empty(minha_vaga);
        
        riding(minha_bomba + 4, gas, 30000);

        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/

        fill(minha_bomba + 4, gas, 200000);

        sem_wait(&sem_carro_abastecido[minha_bomba]); /* Espera o carro o carro ficar abastecido e muda para indicar que nao esta mais abastecido (sem_carro_abastecido[minha_bomba] = 0)*/
        
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        
        /* Carro sai do posto */
        estado_carros[id] = E; /* Carro esta exiting*/
        estado_bombas[minha_bomba] = D; /* bomba esta desocupada*/
        
        /* Imprime que o carro esta saindo do posto*/
        
        sem_post(&sem_bombas_vazias[minha_bomba]); /* Passa a mostrar que a bomba agora esta vazia (sem_bombas_vazias[minha_bomba] = 1) SE COMUNICA COM OUTROS CARROS*/
        estado_carros[id] = L; /* Carro ja left*/

        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
        
    } else {
        /* Carro desistiu de abastecer e passa reto */
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        /* Imprime que o carro passa reto e nao entra em nenhuma vaga*/
        
        riding(8, gas, 30000);

        estado_carros[id] = L;  /* Carro ja left*/
        sem_post(&sem_estados);  /* Libera direito de alterar estados (sem_estados=1)*/
    }
  
    return NULL;
}

void* f_printa() {
    while (printa) {
        printScreen();
    }
}


int main() {
    inicialize();
    pthread_t thr_carros[N_CARROS], thr_bombas[N_BOMBAS], thr_printa[1];
    int i, id_cl[N_CARROS], id_bar[N_BOMBAS], id_printa[1];

    sem_init(&sem_n_vagas_desocupadas, 0, N_VAGAS_ESTAC);
    sem_init(&sem_escreve_painel, 0, 1);
    sem_init(&sem_le_painel, 0, 0);
    sem_init(&sem_estados, 0, 1);

    for (i = 0; i < N_BOMBAS; i++) {
        sem_init(&sem_bombas_vazias[i], 0, 1);
        sem_init(&sem_bomba_ocupada[i], 0, 0);
        sem_init(&sem_carro_abastecido[i], 0, 0);

        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        estado_bombas[i] = D;
        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
    }

     for (i = 0; i < N_CARROS; i++) {
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        estado_carros[i] = I;
        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
    }

    for (i = 0; i < N_VAGAS_ESTAC; i++) {
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        estado_vagas[i] = F;
        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
    }
    
    pthread_create(&thr_printa[0], NULL, f_printa, (void*) &id_printa[0]);

    for (i = 0; i < N_CARROS; i++) {
        id_cl[i] = i;
        pthread_create(&thr_carros[i], NULL, f_carro, (void*) &id_cl[i]);
    }

    for (i = 0; i < N_BOMBAS; i++) {
        id_bar[i] = i;
        pthread_create(&thr_bombas[i], NULL, f_bomba, (void*) &id_bar[i]);
    }
    
    for (i = 0; i < N_CARROS; i++) {
        pthread_join(thr_carros[i], NULL);
    }

    printa = 0;
    sleep(1);

    sem_destroy(&sem_n_vagas_desocupadas);
    sem_destroy(sem_bombas_vazias);
    sem_destroy(sem_carro_abastecido);
    sem_destroy(sem_bomba_ocupada);
    sem_destroy(&sem_escreve_painel);
    sem_destroy(&sem_le_painel);
    sem_destroy(&sem_estados);
    end();
  return 0;
}
