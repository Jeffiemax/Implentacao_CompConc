/* Programa C Merge Sort */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"
  
int N;
int* vetor;
// ordena os dois sub-vetores do vetor[]
// O primeiro sub-vetor é o vetor[e..m]
// O segundo sub-vetor é o vetor[m+1..d]
void merge(int v[], int e, int m, int d)
{
    int i, j, k;
    int n1 = m - e + 1;
    int n2 = d - m;
  
    // cria os vetores
    int E[n1], D[n2];
  
    // copia o valor para o vetor temp  E[] and D[] 
    for (i = 0; i < n1; i++)
        E[i] = v[e + i];
    for (j = 0; j < n2; j++)
        D[j] = v[m + 1 + j];
  
    // combina o vetor temp de volta para o vetor origial vetor[e..d]
    i = 0; // Índice inicial do primeiro sub-vetor
    j = 0; // Índice inicial do segundo sub-vetor
    k = e; // Índice inicial do sub-vetor ordenado
    while (i < n1 && j < n2) {
        if (E[i] <= D[j]) {
            v[k] = E[i];
            i++;
        }
        else {
            v[k] = D[j];
            j++;
        }
        k++;
    }
  
    // Copie os elementos restantes de E[], se houver
    while (i < n1) {
        v[k] = E[i];
        i++;
        k++;
    }
  
    // Copie os elementos restantes de D[], se houver
    while (j < n2) {
        v[k] = D[j];
        j++;
        k++;
    }
}
  
/* e é para o índice esquerdo e d é o índice direito do
sub-vetor do vetor a ser ordenado */
void mergeSort(int v[], int e, int d)
{
    if (e < d) {
        // Igual a (e+d)/2, mas evita overflow para e & d grandes
        int m = e + (d - e) / 2;
  
        // ordena a primeira e a segunda metade
        mergeSort(v, e, m);
        mergeSort(v, m + 1, d);
  
        merge(v, e, m, d);
    }
}
  
// Exibir o array
void printArray(int *v)
{
    int i;
    for (i = 0; i < N; i++)
        printf("%d ", v[i]);
    printf("\n");
}
  
//Função main
int main(int argc, char* argv[]){
  double ini,fim;

  if(argc < 2){
    fprintf(stderr, " Digite %s < tamanho do vetor >\n", argv[0]);
    return 1;
  }
  N = atoi(argv[1]);

  vetor = (int*)malloc(sizeof(int) * N);
  if(vetor == NULL){fprintf(stderr, "ERRO -- malloc");}

  // gerando valores aleatórios para o vetor
  srand(time(NULL)); //objetiva inicializar o gerador de números aleatórios com o valor da função time(NULL) 
	for (int i = 0; i < N; i++){
		vetor[i] = rand() % 100;
  } 

  /* printf("O array original \n");
  printArray(vetor); */

  GET_TIME(ini);
  mergeSort(vetor, 0, N - 1);
  GET_TIME(fim);

  printf("\nVetor depois da ordenação : ");
  printf("\nTempo sequencial: %f\n", fim - ini);
  printf("\n");

 /*  printf("\nO vetor ordenado \n");
  printArray(vetor); */
  return 0;
}