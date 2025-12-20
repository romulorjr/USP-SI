//--------------------------------------------------------------
// COLOQUE OS NOMES DOS RESPONSÁVEIS AQUI: Rômulo Maurício Fonseca Júnior
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

// Retorna o número do grupo conforme planilha divulgada
int grupo() {
  return 10;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 15553801;
}

// E O SEGUNDO NROUSP AQUI, SE HOUVER
int nroUSP2() {
    return 0;
}

// Estrutura para representar um nó na lista de adjacência
typedef struct estr {
      int adj; // número do vértice adjacente
      struct estr *prox; // ponteiro para o próximo nó
} NO;

// Estrutura para representar um nó na fila
typedef struct fila {
    int vertice; // número do vértice
    struct fila *prox; // ponteiro para o próximo nó na fila
} Fila;

// Estrutura para representar um grafo
typedef struct {
    NO **adj; // lista de adjacência
    int V; // número de vértices
} Grafo;

// Função para adicionar um vértice à fila
void enfileira(Fila **inicio, Fila **fim, int v) {
    Fila *novo = (Fila*)malloc(sizeof(Fila));
    novo->vertice = v;
    novo->prox = NULL;
    if(*fim) (*fim)->prox = novo;
    else *inicio = novo;
    *fim = novo;
}

// Função para remover um vértice da fila
int desenfileira(Fila **inicio, Fila **fim) {
    if(!*inicio) return -1; // fila vazia
    int v = (*inicio)->vertice;
    Fila *aux = *inicio;
    *inicio = (*inicio)->prox;
    if(*inicio == NULL) *fim = NULL;
    free(aux);
    return v;
}

// Função para realizar a busca em largura (BFS) no grafo
void bfs(Grafo *g, int origem, int *dist) {
    for(int i = 1; i <= g->V; i++) dist[i] = -1; // inicializa distâncias
    Fila *inicio = NULL, *fim = NULL;
    enfileira(&inicio, &fim, origem);
    dist[origem] = 0; // distância da origem para ela mesma é 0
    
    while(inicio) {
        int v = desenfileira(&inicio, &fim);
        NO *aux = g->adj[v];
        while(aux) {
            if(dist[aux->adj] == -1) { // vértice ainda não visitado
                dist[aux->adj] = dist[v] + 1;
                enfileira(&inicio, &fim, aux->adj);
            }
            aux = aux->prox;
        }
    }
}

// Função principal para encontrar vértices equidistantes
NO *equidistantes(int V, int A, int ij[], int v1, int v2) {
    NO* resp = NULL;

    // Construir grafo
    Grafo g;
    g.V = V;
    g.adj = (NO**)malloc((V+1)*sizeof(NO*));
    for(int i = 1; i <= V; i++) g.adj[i] = NULL;

    // Adicionar arestas ao grafo
    for(int i = 0; i < A * 2; i += 2) {
        NO *novo = (NO*)malloc(sizeof(NO));
        novo->adj = ij[i+1];
        novo->prox = g.adj[ij[i]];
        g.adj[ij[i]] = novo;
    
        novo = (NO*)malloc(sizeof(NO));
        novo->adj = ij[i];
        novo->prox = g.adj[ij[i+1]];
        g.adj[ij[i+1]] = novo;
    }

    // Calcular distâncias a partir de v1 e v2
    int *dist1 = (int*)malloc((V+1)*sizeof(int));
    int *dist2 = (int*)malloc((V+1)*sizeof(int));
    bfs(&g, v1, dist1);
    bfs(&g, v2, dist2);

    // Encontrar vértices equidistantes de v1 e v2
    for(int i = 1; i <= V; i++) {
        if(dist1[i] == dist2[i] && i != v1 && i != v2) {
            NO *novo = (NO*)malloc(sizeof(NO));
            novo->adj = i;
            novo->prox = resp;
            resp = novo;
        }
    }

    // Liberar memória alocada para o grafo
    for(int i = 1; i <= V; i++) {
        NO *aux = g.adj[i];
        while(aux) {
            NO *tmp = aux;
            aux = aux->prox;
            free(tmp);
        }
    }
    free(g.adj);
    free(dist1);
    free(dist2);

    return resp;
}

//---------------------------------------------------------
// Função main para testar o programa
//---------------------------------------------------------
int main() {
    // Teste exemplo
    int V = 5;  // Número de vértices
    int A = 4;  // Número de arestas
    int ij[] = {1,2, 2,3, 3,4, 4,5};  // Arestas
    int v1 = 1, v2 = 5;  // Vértices de origem
    
    NO* teste = equidistantes(V, A, ij, v1, v2);
    
    // Liberar memória da lista de resposta
    while(teste) {
        NO* temp = teste;
        teste = teste->prox;
        free(temp);
    }
    
    return 0;
}