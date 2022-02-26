/* Merge Sort é uma técnica de classificação popular
 que divide uma matriz ou lista em duas metades e,
 em seguida, começa a mesclar quando a profundidade
 suficiente é atingida. A complexidade de tempo da
 ordenação por mesclagem é O(nlogn). */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "timer.h" //auxilia a tomada de tempo
#include <time.h> // srand - utilizado para fazer a aleatoridade do números gerados. 

// Variáveis globais
int N, NTHREAD ;
int* vetor;
int* esquerda;
int* direita;
pthread_mutex_t mutex ,mutex1;
pthread_cond_t cond_cons;

// função de mesclagem para combinar duas partes
void merge(int inferior, int meio, int superior)
{
	pthread_mutex_lock(&mutex);
	*esquerda = meio - inferior + 1;
	*direita = superior - meio;

	// n1 é o tamanho da parte esquerda e n2 é o tamanho da parte direita
	int n1 = meio - inferior + 1;
	int n2 = superior - meio, i, j;

	// armazenando valores na parte esquerda
	for (i = 0; i < n1; i++)
		esquerda[i] = vetor[i + inferior];

	// armazenando valores na parte direita
	for (i = 0; i < n2; i++)
		direita[i] = vetor[i + meio + 1];

	int k = inferior;
	i = j = 0;

	// combina esquerda e direita em ordem crescente
	while (i < n1 && j < n2) {
		if (esquerda[i] <= direita[j])
			vetor[k++] = esquerda[i++];
		else
			vetor[k++] = direita[j++];
	}

	// insire os valores restantes da esquerda
	while (i < n1) {
		vetor[k++] = esquerda[i++];
	}

	// insire os valores restantes da direita
	while (j < n2) {
		vetor[k++] = direita[j++];
	}
	pthread_mutex_unlock(&mutex);
}

// Função Merge sort
void mergeSort(int inferior, int superior){

	// calculando o ponto médio do vetor
	int meio = inferior + (superior - inferior) / 2;
	if (inferior < superior) {

		// chamando primeira metade 
		mergeSort(inferior, meio);

		// chamando segunda metade
		mergeSort(meio + 1, superior);

		// juntando as duas metades
		merge(inferior, meio, superior);
	}
	
}

// fluxo principal para multi-threading
void* merge_sort(void* arg){

	long int id = (long int) arg;
    //printf("\nA thread %d está executando", id);   
    int TAM = N / NTHREAD; //Tamanho do bloco em cada Thread

	//calculando parte inferior e superior
	int inferior = id * TAM; //Elemento inicial do bloco de cada Thread
	int superior; //Ele emnto final no bloco de cada Thread
	superior = (id + 1) * TAM -1;

	/* if(id == NTHREAD - 1) superior = N;
	else superior = inferior + TAM; //Trata o resto se houver
 */
	// avaliando o ponto médio
	int meio = inferior + (superior - inferior) / 2;
	if (inferior < superior) {
		mergeSort(inferior, meio);
		mergeSort(meio + 1, superior);
		merge(inferior, meio, superior);
	}

    pthread_exit(NULL); 
}

// Função main
int main(int argc, char* argv[]){

    pthread_t *tid;
	pthread_mutex_init(&mutex, NULL);

    if(argc < 3){
        fprintf(stderr, " Digite %s < tamanho do vetor > < valor de Threads >\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    NTHREAD = atoi(argv[2]);

    vetor = (int*)malloc(sizeof(int) * N);
    if(vetor == NULL){fprintf(stderr, "ERRO -- malloc");}
    esquerda = (int*)malloc(sizeof(int) * N);
    if(esquerda == NULL){fprintf(stderr, "ERRO -- malloc");}
    direita = (int*)malloc(sizeof(int) * N);
    if(direita == NULL){fprintf(stderr, "ERRO -- malloc");}
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREAD);
    if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
    }

	// preenche o vetor de entrada, gerando valores aleatórios para o vetor
    srand(time(NULL)); //objetiva inicializar o gerador de números aleatórios com o valor da função time(NULL) 
	for (int i = 0; i < N; i++){
		vetor[i] = rand() % 100;
    } 
	
    //Vetor criado
    /* printf("Vetor inicial: ");
    printf("[");
    for(int i = 0;i < N;i++){
        printf(" %d ",vetor[i]);
    }
    printf("]"); */

    double ini,fim;// variáveis para a tomada de tempo
    GET_TIME(ini);
 
	// criando threads
	for (long int i = 0; i < NTHREAD; i++)
		pthread_create(&tid[i], NULL, merge_sort,(void*) i);


	// juntando todas as threads
	for (int i = 0; i < NTHREAD; i++)
		pthread_join(tid[i], NULL);
	
	// unindo as threads
	merge(0, (N / 2 - 1) / 2, N / 2 - 1);
	merge(N / 2, N/2 + (N-1-N/2)/2, N - 1);
	merge(0, (N - 1)/2, N - 1);

    GET_TIME(fim);
    printf("\nVetor depois da ordenação : ");
    printf("\nTempo concorrente:  %f\n", fim - ini);
    printf("\n"); 

	/* // exibir o vetor ordenado
	printf("Vetor ordenado: ");
    printf("[");
    for(int i = 0;i < N;i++){
        printf(" %d ",vetor[i]);
    }
    printf("]");
    printf("\n");
 */
	for(int i = 0;i < N-1 ;i++){
		if(vetor[i] > vetor[i+1]){
        	printf(" ERRO NA POSICÃO %d ",i);
			break;
		}
    }
 
    free(vetor);
    free(tid); 
	return 0;
}
