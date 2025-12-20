#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int chave; 
    int campodaestrutura[1]; 
} registro1;

typedef struct {
    int chave; 
    int campodaestrutura[1000]; 
} registro1000;


void insertionSort(registro1 *vetor, int N, int *comparacoes1, int *movimentacoes1) {
      *comparacoes1 = 0; 
      *movimentacoes1 = 0;

 for (int i = 1; i < N; i++) {
     registro1 chave = vetor[i];
     int j = i - 1;

     while (j >= 0 && chave.chave < vetor[j].chave) {
        (*comparacoes1)++; //compara++
         vetor[j + 1] = vetor[j];
         (*movimentacoes1)++; //moviementa++
         j--;
         }
         vetor[j + 1] = chave;
         (*movimentacoes1)++; //moviementa++
         }
}

void selectionSort(registro1 *vetor, int N, int *comparacoes2, int *movimentacoes2) {
    *comparacoes2 = 0;
    *movimentacoes2 = 0;

    for (int i = 0; i < N - 1; i++) {
        int minIndex = i;  

        
        for (int j = i + 1; j < N; j++) {
            (*comparacoes2)++;  //compara++
            if (vetor[j].chave < vetor[minIndex].chave) {
                minIndex = j;
            }
        }

       
        if (minIndex != i) {
            registro1 temp = vetor[i];
            vetor[i] = vetor[minIndex];
            vetor[minIndex] = temp;
            (*movimentacoes2) += 3;
        }
    }
}

void bubbleSort(registro1 *vetor, int N, int *comparacoes3, int *movimentacoes3) {
   *comparacoes3 = 0;
   *movimentacoes3 = 0;  

    int trocou;
    for (int i = 0; i < N - 1; i++) {
        trocou = 0;  

        // Passagem pelo vetor
        for (int j = 0; j < N - i - 1; j++) {
            (*comparacoes3)++;  //compara++
            if (vetor[j].chave > vetor[j + 1].chave) {
                // Realiza a troca
                registro1 temp = vetor[j];
                (*movimentacoes3)++;
                vetor[j] = vetor[j + 1];
                (*movimentacoes3)++;
                vetor[j + 1] = temp;
                (*movimentacoes3)++;  //moviementa++3
                trocou = 1;  
            }
        }
        if (trocou == 0) {
            break;
        }
    }
}

void shellSort(registro1 *vetor, int N, int *comparacoes4, int *movimentacoes4) {
     *comparacoes4 = 0;
     *movimentacoes4 = 0;

for (int gap = N / 2; gap > 0; gap /= 2) {
    for (int i = gap; i < N; i++) {
        registro1 chave = vetor[i];
        int j = i;
        (*comparacoes4)++;//compara++
        while (j >= gap && vetor[j - gap].chave > chave.chave) {
            (*comparacoes4)++; //compara++
            vetor[j] = vetor[j - gap];
            (*movimentacoes4)++; //moviementa++
            j -= gap;
        }
        vetor[j] = chave;
        (*movimentacoes4)++;//moviementa++
    }
        }
}

void merge(registro1 *vetor, int inicio, int meio, int fim, int *comparacoes5, int *movimentacoes5) {
    int n1 = meio - inicio + 1;
    int n2 = fim - meio;

    registro1 *esquerda = (registro1*) malloc(n1 * sizeof(registro1));
    registro1 *direita = (registro1*) malloc(n2 * sizeof(registro1));

    for (int i = 0; i < n1; i++) {
        esquerda[i] = vetor[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        direita[j] = vetor[meio + 1 + j];
    }

    int i = 0, j = 0, k = inicio;

    while (i < n1 && j < n2) {
        (*comparacoes5)++; //compara++
        if (esquerda[i].chave <= direita[j].chave) {
            vetor[k] = esquerda[i];
            i++;
        } else {
            vetor[k] = direita[j];
            j++;
        }
        (*movimentacoes5)++; //moviementa++
        k++;
    }
    while (i < n1) {
        vetor[k] = esquerda[i];
        i++;
        k++;
        (*movimentacoes5)++; //moviementa++
    }
    while (j < n2) {
        vetor[k] = direita[j];
        j++;
        k++;
        (*movimentacoes5)++; //moviementa++
    }

    free(esquerda);
    free(direita);
}

void mergeSort(registro1 *vetor, int inicio, int fim, int *comparacoes5, int *movimentacoes5) {
    if (inicio < fim) {
        int meio = inicio + (fim - inicio) / 2;

        mergeSort(vetor, inicio, meio, comparacoes5, movimentacoes5);
        mergeSort(vetor, meio + 1, fim, comparacoes5, movimentacoes5);
        merge(vetor, inicio, meio, fim, comparacoes5, movimentacoes5);
    }
}

void heapify(registro1 *vetor, int N, int i, int *comparacoes6, int *movimentacoes6);
void heapSort(registro1 *vetor, int N, int *comparacoes6, int *movimentacoes6) {
    for (int i = N / 2 - 1; i >= 0; i--) {
        heapify(vetor, N, i, comparacoes6, movimentacoes6);
    }
    for (int i = N - 1; i >= 0; i--) {
        registro1 aux = vetor[0];
        vetor[0] = vetor[i];
        vetor[i] = aux;
        (*movimentacoes6)+= 3;  //moviementa++
        heapify(vetor, i, 0, comparacoes6, movimentacoes6);
    }
}   

void heapify(registro1 *vetor, int N, int i, int *comparacoes6, int *movimentacoes6) {

    int maior = i; //maior como raíz
    int esquerda = 2 * i + 1; //filho esquerdo
    int direita = 2 * i + 2; //filho direito

    if (esquerda < N && vetor[esquerda].chave > vetor[maior].chave) {
        (*comparacoes6)++;  //compara++
        maior = esquerda;
    }

    if (direita < N && vetor[direita].chave > vetor[maior].chave) {
        (*comparacoes6)++;  //compara++
        maior = direita;
    }

    if (maior != i) {
        registro1 aux = vetor[i];
        vetor[i] = vetor[maior];
        vetor[maior] = aux;
        (*movimentacoes6)+= 3;  //moviementa++
        heapify(vetor, N, maior, comparacoes6, movimentacoes6);
    }
}

int partition(registro1 *vetor, int low, int high, int *comparacoes7, int *movimentacoes7);
void quickSort(registro1 *vetor, int low, int high, int *comparacoes7, int *movimentacoes7) {
    if (low < high) {
        int pi = partition(vetor, low, high, comparacoes7, movimentacoes7);

        quickSort(vetor, low, pi - 1, comparacoes7, movimentacoes7);
        quickSort(vetor, pi + 1, high, comparacoes7, movimentacoes7);
    }
}

int partition(registro1 *vetor, int low, int high, int *comparacoes7, int *movimentacoes7) {
    registro1 pivot = vetor[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        (*comparacoes7)++;  // Incrementa o valor apontado por comparacoes
        if (vetor[j].chave <= pivot.chave) {
            i++;
            registro1 temp = vetor[i];
            vetor[i] = vetor[j];
            vetor[j] = temp;
            (*movimentacoes7) += 3;  // Incrementa o valor apontado por movimentacoes
        }
    }

    registro1 temp = vetor[i + 1];
    vetor[i + 1] = vetor[high];
    vetor[high] = temp;
    (*movimentacoes7) += 3;  // Incrementa o valor apontado por movimentacoes

    return i + 1;
}


void salvaresultado(const char *resultados, const char *algoritmo, int comparacoes, int movimentacoes, double tempoexecucao) {
    FILE *arq = fopen(resultados, "a");
    if (arq == NULL) {
        printf("Erro na abertura do arquivo CSV!\n");
        return;
    }
    fprintf(arq, "%s, %d, %d, %.2f\n", algoritmo, comparacoes, movimentacoes, tempoexecucao);
    fclose(arq);
}

int main(int argc, char *argv[]) {
    FILE *arq;
    int N;
if (argc != 2) {
    printf("Uso: %s <arquivo de entrada>\n", argv[0]);
    return 1;
}
arq = fopen(argv[1], "r"); // lê a primeira linha do arquivo, ou seja a quantidade de registros
if (arq == NULL) {
    printf("Erro na abertura do arquivo!\n"); 
    return 1;
}
    fscanf(arq, "%d", &N);
registro1 *vetor1 = (registro1*) malloc(N * sizeof(registro1)); //alocação para registro1 
if (vetor1 == NULL) {        
    printf("Erro na alocacao de memoria!\n"); 
    free(vetor1);
    return 1;
}
registro1000 *vetor1000 = (registro1000*) malloc(N * sizeof(registro1000)); //alocação para registro1000
if (vetor1000 == NULL) {        
    printf("Erro na alocacao de memoria!\n"); 
    free(vetor1000);
    return 1;
}


for (int i = 0; i < N; i++) {
    fscanf(arq, "%d", &vetor1[i].chave);
    vetor1000[i].chave = vetor1[i].chave;
}
clock_t inicio, fim; 
double tempoexecucao;

int comparacoes1 = 0;
int movimentacoes1 = 0;
inicio = clock();
printf("\n");
insertionSort(vetor1, N, &comparacoes1, &movimentacoes1); //INSERTION 
printf("-----------------------INSERTION SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes1);
printf("Movimentacoes: %d\n", movimentacoes1);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n"); 
salvaresultado("resultados.csv", "INSERTION SORT", comparacoes1, movimentacoes1, tempoexecucao);

int comparacoes2 = 0;
int movimentacoes2 = 0;
 inicio = clock();
selectionSort(vetor1, N, &comparacoes2, &movimentacoes2); //SELECTION 
printf("-----------------------SELECTION SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes2);
printf("Movimentacoes: %d\n", movimentacoes2);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
salvaresultado("resultados.csv", "SELECTION SORT", comparacoes2, movimentacoes2, tempoexecucao);
printf("\n"); 

int comparacoes3 = 0;
int movimentacoes3 = 0;
inicio = clock();
bubbleSort(vetor1, N, &comparacoes3, &movimentacoes3); //BUBBLE 
printf("-----------------------BUBBLE SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes3);
printf("Movimentacoes: %d\n", movimentacoes3);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n");
salvaresultado("resultados.csv", "BUBBLE SORT", comparacoes3, movimentacoes3, tempoexecucao);

int comparacoes4 = 0;
int movimentacoes4 = 0;
inicio = clock();
shellSort(vetor1, N, &comparacoes4, &movimentacoes4); //SHELL 
printf("-----------------------SHELL SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes4);
printf("Movimentacoes: %d\n", movimentacoes4);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n");
salvaresultado("resultados.csv", "SHELL SORT", comparacoes4, movimentacoes4, tempoexecucao);

int comparacoes5 = 0;
int movimentacoes5 = 0;
inicio = clock();
mergeSort(vetor1, 0, N - 1, &comparacoes5, &movimentacoes5); //MERGE 
printf("-----------------------MERGE SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes5);
printf("Movimentacoes: %d\n", movimentacoes5);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n");
salvaresultado("resultados.csv", "MERGE SORT", comparacoes5, movimentacoes5, tempoexecucao);

int comparacoes6 = 0;
int movimentacoes6 = 0;
inicio = clock();
heapSort(vetor1, N, &comparacoes6, &movimentacoes6); //HEAP 
printf("-----------------------HEAP SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes6); 
printf("Movimentacoes: %d\n", movimentacoes6);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n");
salvaresultado("resultados.csv", "HEAP SORT", comparacoes6, movimentacoes6, tempoexecucao);


int comparacoes7 = 0;   
int movimentacoes7 = 0;
inicio = clock();
quickSort(vetor1, 0, N - 1, &comparacoes7, &movimentacoes7); //QUICK 
printf("-----------------------QUICK SORT:-----------------------\n");
printf("\n");
printf("Comparacoes: %d\n", comparacoes7);
printf("Movimentacoes: %d\n", movimentacoes7);
fim = clock();
tempoexecucao = (double)(fim - inicio) * 1000 / CLOCKS_PER_SEC;
printf("Tempo de execucao: %f\n", tempoexecucao);
printf("\n");
salvaresultado("resultados.csv", "QUICK SORT", comparacoes7, movimentacoes7, tempoexecucao);

free(vetor1);
free(vetor1000);
return 0;
 } 