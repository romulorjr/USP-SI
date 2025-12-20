/*Nome: Rômulo Maurício Fonseca Júnior*/
/*Número USP: 15553801*/

#include <stdio.h>
#include <stdlib.h>

// função para encontrar valores mínimos e máximos usando recursão e divisão e conquista
void encontrar_Min_Max(int **matriz, int n, int i_inicial, int j_inicial, int *menor_valor, int *menorLinha, int *menorColuna, int *maior_valor, int *maiorLinha, int *maiorColuna) {
    // CB: n=1, matriz 1x1
    if (n == 1) {
        int valor_atual = matriz[i_inicial][j_inicial];

        // atualizando valor máximo
        if (valor_atual > *maior_valor || (valor_atual == *maior_valor && (i_inicial < *maiorLinha || (i_inicial == *maiorLinha && j_inicial < *maiorColuna)))) {
            *maior_valor = valor_atual;
            *maiorLinha = i_inicial;
            *maiorColuna = j_inicial;
        }

        // atualizando valor mínimo
        if (valor_atual < *menor_valor || (valor_atual == *menor_valor && (i_inicial < *menorLinha || (i_inicial == *menorLinha && j_inicial < *menorColuna)))) {
            *menor_valor = valor_atual;
            *menorLinha = i_inicial;
            *menorColuna = j_inicial;
        }

        return;
    }

    // divisão da matriz em quadrantes
    int metade = n / 2;

    // chamada da recursão 
    encontrar_Min_Max(matriz, metade, i_inicial, j_inicial, menor_valor, menorLinha, menorColuna, maior_valor, maiorLinha, maiorColuna);                    // primeiro quadrante
    encontrar_Min_Max(matriz, metade, i_inicial, j_inicial + metade, menor_valor, menorLinha, menorColuna, maior_valor, maiorLinha, maiorColuna);         // segundo quadrante
    encontrar_Min_Max(matriz, metade, i_inicial + metade, j_inicial, menor_valor, menorLinha, menorColuna, maior_valor, maiorLinha, maiorColuna);         // terceiro quadrante
    encontrar_Min_Max(matriz, metade, i_inicial + metade, j_inicial + metade, menor_valor, menorLinha, menorColuna, maior_valor, maiorLinha, maiorColuna); // quarto quadrante
}

// alocação de memoria para matriz
int **alocar_matriz(int n) {
    int **matriz = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        matriz[i] = (int *)malloc(n * sizeof(int));
    }
    return matriz;
}
//funcao principal
int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Uso: %s <Dimensao da Matriz> <arquivo de entrada> <arquivo de saida>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); //converte o argumento passado em um inteiro
    if ((n & (n - 1)) != 0 || n > 64) {
        printf("A dimensão da matriz deve ser uma potência de 2 e no máximo 64.\n");
        return 1;
    }

        FILE *arquivo_de_entrada = fopen(argv[2], "r"); //ler arquivo de entrada (r = read)
        if (arquivo_de_entrada == NULL) {
            printf("Erro ao abrir o arquivo de entrada.\n");
            return 1;
        }

    fseek(arquivo_de_entrada, 0, SEEK_END);//conferencia de argumentos passados no arquivo de entrada (códigos anteriores com erros ao abrir arquivos vazios)
    long tamanho = ftell(arquivo_de_entrada); //pesquisado em "Curso de C - Departamento de Matemática UFSC" e "TurorialsPoint"
    if (tamanho == 0) {
        printf("Erro: o arquivo de entrada está vazio!\n");
        fclose(arquivo_de_entrada);
        return 1;
    }
    rewind(arquivo_de_entrada);

    int **matriz = alocar_matriz(n);

    // ler matriz do arquivo de entrada
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(arquivo_de_entrada, "%d", &matriz[i][j]) != 1) {
                printf("Erro ao ler o valor da matriz na posição (%d, %d)\n", i, j);
                fclose(arquivo_de_entrada);
                return 1;
            }
        }
    }
    fclose(arquivo_de_entrada);

    int menor_valor = 256; //passando valores máximos e mínimos com 1 de diferenca para garantir um intervalo [0, 255]
    int maior_valor = -1; // ""

    int menorLinha = 0, menorColuna = 0;
    int maiorLinha = 0, maiorColuna = 0;

    // chamando função para encontrar o mínimo e o máximo
    encontrar_Min_Max(matriz, n, 0, 0, &menor_valor, &menorLinha, &menorColuna, &maior_valor, &maiorLinha, &maiorColuna);

        // abrir o arquivo de saída para escrever os resultados
        FILE *arquivo_de_saida = fopen(argv[3], "w");
        if (arquivo_de_saida == NULL) {
            printf("Erro ao abrir o arquivo de saída.\n");
            return 1;
        }

    // escreve resultados
    fprintf(arquivo_de_saida, "min = %d; pos = (%d, %d)\n", menor_valor, menorLinha, menorColuna);
    fprintf(arquivo_de_saida, "max = %d; pos = (%d, %d)\n", maior_valor, maiorLinha, maiorColuna);

    fclose(arquivo_de_saida);

    // liberar a memória alocada para a matriz
    for (int i = 0; i < n; i++) {
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}
