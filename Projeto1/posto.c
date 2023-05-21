
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_CARROS 20
#define N_BOMBAS 4
#define N_VAGAS_ESTAC 4

sem_t sem_n_vagas_desocupadas; /* Conta o numero de vagas desocupadas no estacionamento*/
sem_t sem_bombas_vazias[N_BOMBAS]; /* Indica quais bombas estão vazias (1=vazia, 0=cheia)*/
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

/* Por questoes de sincronizacao, apenas uma bomba pode indicar que esta vazia por vez, e apenas um carro pode ler essa informacao por vez. 
Na historinha do problema eh como se essa informacao fosse indicada num "painel"
Mas nao precisamos mostrar isso no terminal
*/
sem_t sem_escreve_painel, sem_le_painel; /* Semaforos que controlam quem pode ler o painel e quem pode escrever nele (1=ninguem esta fazendo isso, 0=alguem esta fazendo)*/
int painel; /* Inteiro que mostra o id da bomba que esta indicando que esta vazia*/

/* Funcao responsavel pela impressao do cineminha. */
void imprimeCineminha() {
  int i, j, qtosClientesArrived = 0, qtosClientesExit = 0, titulo = 0;
  int clientesArrived[N_CARROS], clientesExit[N_CARROS]; 
  
  /* Verifica quais clientes estao no estado "Arrived". */
  for (i = 0; i < N_CARROS; i++) {
    if (estado_carros[i] == A) {
      clientesArrived[qtosClientesArrived++] = i;
    }
  }
  
  /* Verifica quais clientes estao no estado "Exit". */
  for (i = 0; i < N_CARROS; i++) {
    if (estado_carros[i] == E) {
      clientesExit[qtosClientesExit++] = i;
    }
  }
  
  /*****************************************************************************
   **************************** IMPRESSAO DO TITULO ****************************
   ****************************************************************************/
  
  /* Imprime os espacos em branco da parte referente aos clientes Arrived. */
  for (i = 0; i < qtosClientesArrived; i++) {
    printf("     ");
  }
  
  /* Faz a conta para imprimir o tÃ­tulo 'BARBEARIA' no meio. */
  
  if (qtosClientesArrived == 0) {
    printf("     ");
  }
  
  titulo = (4 + 5 * N_VAGAS_ESTAC + 5 + 5 * N_BOMBAS) / 2;
  
  titulo -= 5;
  
  for (i = 0; i < titulo; i++) {
    printf(" ");
  }
  
  printf("BARBEARIA\n");
  
  /*****************************************************************************
   ************************** FIM IMPRESSAO DO TITULO **************************
   ****************************************************************************/
  
  /*****************************************************************************
   ************************** IMPRESSAO PRIMEIRA LINHA *************************
   ****************************************************************************/
  
  for (i = 0; i < qtosClientesArrived; i++) {
    printf(" ... ");
  }
  
  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }
  
  printf ("/ ");
  
  /* Imprimi as cadeiras dos clientes. */
  for (i = 0; i < N_VAGAS_ESTAC; i++) {
    if (estado_vagas[i] == B) {
      printf(" ... ");
    } else {
      printf("     ");
    }
  }
  
  /* EspaÃ§o entre as cadeiras dos clientes e as dos barbeiros. */ 
  printf("    ");
  
  for (i = 0; i < N_BOMBAS; i++) {
    switch (estado_bombas[i]) {
    case BAB:
      printf(" ... ");
      break;
    case D:
      printf(" zzz ");
      break;
    }
  }
  printf("\\ \n");
  
  /*****************************************************************************
   ********************** FIM DA IMPRESSAO PRIMEIRA LINHA **********************
   ****************************************************************************/
  
  /*****************************************************************************
   ************************** IMPRESSAO SEGUNDA LINHA **************************
   ****************************************************************************/
  
  for (i = 0; i < qtosClientesArrived; i++) {
    printf(" %2d  ", clientesArrived[i]);
  }
  
  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }
  
  printf ("| ");
  
  for (i = 0; i < N_VAGAS_ESTAC; i++) {
    if (estado_vagas[i] == B) {
      printf(" %2d  ", conteudo_vagas[i]);
    } else {
      printf("     ");
    }
  }
  
  /* EspaÃ§o entre as cadeiras dos clientes e dos barbeiros. */
  printf("    ");
  
  j = 0;
  for (i = 0; i < N_BOMBAS; i++) {
    switch (estado_bombas[i]) {
    case BAB:
      printf(" %2d  ", carros_em_bombas[i]);
      break;
    case D:
      printf("     ");
      break;
    }
  }
  
  printf("| ");
  
  for (i = 0; i < qtosClientesExit; i++) {
    printf(" %2d  ", clientesExit[i]);
  }
  
  printf("\n");
  
  /*****************************************************************************
   ********************** FIM DA IMPRESSAO SEGUNDA LINHA ***********************
   ****************************************************************************/
  
  /*****************************************************************************
   ************************* IMPRESSAO TERCEIRA LINHA **************************
   ****************************************************************************/
  
  for (i = 0; i < qtosClientesArrived; i++) {
    printf("     ");
  }
  
  /* Se nao chegou clientes, imprimimos espacamento para ficar alinhado com
   * outros momentos do cineminha.
   */
  if (qtosClientesArrived == 0) {
    printf("     ");
  }
  
  printf("\\ ");
  
  for (i = 0; i < N_VAGAS_ESTAC; i++) {
    printf(" C%02d ", i);
  }
  
  printf("    ");
  
  for (i = 0; i < N_BOMBAS; i++) {
    printf(" B%02d ", i);
  }
  
  printf("/ \n\n");
  
  /*****************************************************************************
   ********************** FIM DA IMPRESSAO TERCEIRA LINHA **********************
   ****************************************************************************/
  
  return;
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
    int i;
    int minha_bomba, minha_vaga;

    sleep(id % 3);
    
    sem_wait(&sem_estados); /* Pega o direito de alterar estados*/
    
    /* Carro chegou ao posto (mas nao necessariamente entra em uma vaga) */
    estado_carros[id] = A; /* Arrived no posto*/
    imprimeCineminha(); /* Imprime que o carro chegou ao posto, mas nao necessariamente entra em uma vaga */

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
        
        imprimeCineminha();  /* Imprime com carro dentro da vaga */


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
        imprimeCineminha(); /* Imprime que o carro esta na bomba e vai comecar a abastecer. */

        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
        
        sem_wait(&sem_carro_abastecido[minha_bomba]); /* Espera o carro o carro ficar abastecido e muda para indicar que nao esta mais abastecido (sem_carro_abastecido[minha_bomba] = 0)*/
        
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        
        /* Carro sai do posto */
        estado_carros[id] = E; /* Carro esta exiting*/
        estado_bombas[minha_bomba] = D; /* bomba esta desocupada*/
        
        imprimeCineminha(); /* Imprime que o carro esta saindo do posto*/
        
        sem_post(&sem_bombas_vazias[minha_bomba]); /* Passa a mostrar que a bomba agora esta vazia (sem_bombas_vazias[minha_bomba] = 1) SE COMUNICA COM OUTROS CARROS*/
        estado_carros[id] = L; /* Carro ja left*/

        sem_post(&sem_estados); /* Libera direito de alterar estados (sem_estados=1)*/
        
    } else {
        /* Carro desistiu de abastecer e passa reto */
        sem_wait(&sem_estados); /* Pega direito de alterar estados (sem_estados=0)*/
        imprimeCineminha(); /* Imprime que o carro passa reto e nao entra em nenhuma vaga*/
        estado_carros[id] = L;  /* Carro ja left*/
        sem_post(&sem_estados);  /* Libera direito de alterar estados (sem_estados=1)*/
    }
    
    return NULL;
}


int main() {
    pthread_t thr_carros[N_CARROS], thr_bombas[N_BOMBAS];
    int i, id_cl[N_CARROS], id_bar[N_BOMBAS];

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

    sem_destroy(&sem_n_vagas_desocupadas);
    sem_destroy(sem_bombas_vazias);
    sem_destroy(sem_carro_abastecido);
    sem_destroy(sem_bomba_ocupada);
    sem_destroy(&sem_escreve_painel);
    sem_destroy(&sem_le_painel);
    sem_destroy(&sem_estados);
  
  return 0;
}
