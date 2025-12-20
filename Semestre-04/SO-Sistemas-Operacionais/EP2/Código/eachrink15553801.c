#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SIMBOLO_INICIAL 128
#define SIMBOLO_FINAL 255

typedef struct {
    unsigned char first;
    unsigned char second;
    int frequency;
} Par;

typedef struct {
    unsigned char *data;
    size_t size;
    int thread_id;
    int num_threads;
    int *frequencias;
    pthread_mutex_t *mutex;
    size_t start_index;
    size_t end_index;
} ThreadData;

// Função para threads contarem frequências
void* contar_frequencias_thread(void *arg) {
    ThreadData *td = (ThreadData*)arg;

    // Array local para evitar contenção
    int *freq_local = calloc(256 * 256, sizeof(int));
    if (!freq_local) return NULL;

    // Cada thread conta em seu chunk
    for (size_t i = td->start_index; i < td->end_index && i < td->size - 1; i++) {
        int index = td->data[i] * 256 + td->data[i + 1];
        if (index >= 0 && index < 256 * 256) {
            freq_local[index]++;
        }
    }

    // Combinar resultados no array global
    pthread_mutex_lock(td->mutex);
    for (int i = 0; i < 256 * 256; i++) {
        td->frequencias[i] += freq_local[i];
    }
    pthread_mutex_unlock(td->mutex);

    free(freq_local);
    return NULL;
}

Par encontrar_par_mais_frequente_paralelo(unsigned char *data, size_t size, int num_threads) {
    if (size < 2 || num_threads <= 1) {
        // Fallback sequencial para casos simples
        int *frequencias = malloc(256 * 256 * sizeof(int));
        Par resultado = {0, 0, 0};
        if (frequencias) {
            memset(frequencias, 0, 256 * 256 * sizeof(int));
            for (size_t i = 0; i < size - 1; i++) {
                int index = data[i] * 256 + data[i + 1];
                if (index < 256 * 256) frequencias[index]++;
            }

            for (int i = 0; i < 256; i++) {
                for (int j = 0; j < 256; j++) {
                    int idx = i * 256 + j;
                    if (frequencias[idx] > resultado.frequency) {
                        resultado.first = i;
                        resultado.second = j;
                        resultado.frequency = frequencias[idx];
                    }
                }
            }
            free(frequencias);
        }
        return resultado;
    }

    pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
    ThreadData *thread_data = malloc(num_threads * sizeof(ThreadData));
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    int *frequencias = calloc(256 * 256, sizeof(int));

    if (!threads || !thread_data || !frequencias) {
        // Fallback em caso de erro de alocação
        Par resultado = {0, 0, 0};
        free(threads);
        free(thread_data);
        free(frequencias);
        return resultado;
    }

    size_t chunk_size = size / num_threads;

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].data = data;
        thread_data[i].size = size;
        thread_data[i].start_index = i * chunk_size;
        thread_data[i].end_index = (i == num_threads - 1) ? size : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;
        thread_data[i].num_threads = num_threads;
        thread_data[i].frequencias = frequencias;
        thread_data[i].mutex = &mutex;

        pthread_create(&threads[i], NULL, contar_frequencias_thread, &thread_data[i]);
    }

    // Aguardar todas as threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Encontrar o par mais frequente nos resultados combinados
    Par melhor_par = {0, 0, 0};
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            int idx = i * 256 + j;
            if (frequencias[idx] > melhor_par.frequency) {
                melhor_par.first = i;
                melhor_par.second = j;
                melhor_par.frequency = frequencias[idx];
            } else if (frequencias[idx] == melhor_par.frequency && frequencias[idx] > 0) {
                if (i < melhor_par.first || (i == melhor_par.first && j < melhor_par.second)) {
                    melhor_par.first = i;
                    melhor_par.second = j;
                    melhor_par.frequency = frequencias[idx];
                }
            }
        }
    }

    free(threads);
    free(thread_data);
    free(frequencias);
    pthread_mutex_destroy(&mutex);

    return melhor_par;
}

// Função sequencial para comparação
Par encontrar_par_mais_frequente(unsigned char *data, size_t size, int *frequencias) {
    if (size < 2) {
        Par par_vazio = {0, 0, 0};
        return par_vazio;
    }

    memset(frequencias, 0, 256 * 256 * sizeof(int));

    for (size_t i = 0; i < size - 1; i++) {
        int index = data[i] * 256 + data[i + 1];
        if (index >= 0 && index < 256 * 256) {
            frequencias[index]++;
        }
    }

    Par melhor_par = {0, 0, 0};
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 256; j++) {
            int idx = i * 256 + j;
            if (frequencias[idx] > melhor_par.frequency) {
                melhor_par.first = i;
                melhor_par.second = j;
                melhor_par.frequency = frequencias[idx];
            } else if (frequencias[idx] == melhor_par.frequency && frequencias[idx] > 0) {
                if (i < melhor_par.first || (i == melhor_par.first && j < melhor_par.second)) {
                    melhor_par.first = i;
                    melhor_par.second = j;
                    melhor_par.frequency = frequencias[idx];
                }
            }
        }
    }

    return melhor_par;
}

size_t substituir_pares(unsigned char *data, size_t size, unsigned char alvo1,
                        unsigned char alvo2, unsigned char substituto) {
    if (size < 2 || !data) return size;

    size_t new_size = 0;

    for (size_t i = 0; i < size; i++) {
        if (i < size - 1 && data[i] == alvo1 && data[i + 1] == alvo2) {
            data[new_size++] = substituto;
            i++;
        } else {
            data[new_size++] = data[i];
        }
    }

    return new_size;
}

void compactar(unsigned char *data, size_t size, const char *output_filename, int num_threads) {
    if (!data || size == 0) {
        fprintf(stderr, "Erro: Dados inválidos para compactação\n");
        return;
    }

    // Se 0 threads, usar número de CPUs disponíveis
    if (num_threads == 0) {
        num_threads = sysconf(_SC_NPROCESSORS_ONLN);
        if (num_threads <= 0) num_threads = 1;
    }

    unsigned char simbolo = SIMBOLO_INICIAL;
    unsigned char *trabalho = malloc(size);
    if (!trabalho) {
        fprintf(stderr, "Erro: Falha na alocação de trabalho\n");
        return;
    }
    memcpy(trabalho, data, size);
    size_t trabalho_size = size;

    unsigned char pares_first[SIMBOLO_FINAL - SIMBOLO_INICIAL + 1];
    unsigned char pares_second[SIMBOLO_FINAL - SIMBOLO_INICIAL + 1];
    int num_substituicoes = 0;

    while (simbolo <= SIMBOLO_FINAL && trabalho_size >= 2) {
        Par melhor_par;

        // Usar versão paralela se tiver threads suficientes e dados suficientes
        if (num_threads > 1 && trabalho_size > 1000) {
            melhor_par = encontrar_par_mais_frequente_paralelo(trabalho, trabalho_size, num_threads);
        } else {
            // Usar versão sequencial para pequenos arquivos
            int *frequencias = malloc(256 * 256 * sizeof(int));
            if (frequencias) {
                melhor_par = encontrar_par_mais_frequente(trabalho, trabalho_size, frequencias);
                free(frequencias);
            } else {
                melhor_par.frequency = 0; // Forçar parada em caso de erro
            }
        }

        if (melhor_par.frequency <= 1) {
            break;
        }

        if (num_substituicoes >= (SIMBOLO_FINAL - SIMBOLO_INICIAL + 1)) {
            break;
        }

        pares_first[num_substituicoes] = melhor_par.first;
        pares_second[num_substituicoes] = melhor_par.second;
        num_substituicoes++;

        trabalho_size = substituir_pares(trabalho, trabalho_size,
                                       melhor_par.first, melhor_par.second, simbolo);
        simbolo++;

        if (trabalho_size < 2) break;
    }

    FILE *output = fopen(output_filename, "wb");
    if (!output) {
        perror("Erro ao criar arquivo de saída");
        free(trabalho);
        return;
    }

    for (int i = 0; i < num_substituicoes; i++) {
        fputc(pares_first[i], output);
        fputc(pares_second[i], output);
    }

    fputc(SIMBOLO_FINAL, output);

    if (trabalho_size > 0) {
        fwrite(trabalho, 1, trabalho_size, output);
    }

    fclose(output);
    free(trabalho);
}

// FUNÇÃO DESCOMPACTAR COMPLETA
void descompactar(const char *input_filename, const char *output_filename) {
    FILE *input = fopen(input_filename, "rb");
    if (!input) {
        perror("Erro ao abrir arquivo para descompactação");
        return;
    }

    unsigned char pares_first[SIMBOLO_FINAL - SIMBOLO_INICIAL + 1];
    unsigned char pares_second[SIMBOLO_FINAL - SIMBOLO_INICIAL + 1];
    int num_mapeamentos = 0;

    unsigned char c1, c2;
    while ((c1 = fgetc(input)) != SIMBOLO_FINAL) {
        c2 = fgetc(input);
        if (feof(input)) break;

        pares_first[num_mapeamentos] = c1;
        pares_second[num_mapeamentos] = c2;
        num_mapeamentos++;
    }

    fseek(input, 0, SEEK_END);
    long total_size = ftell(input);
    long header_size = 2 * num_mapeamentos + 1;

    if (total_size < header_size) {
        fprintf(stderr, "Erro: Arquivo corrompido ou vazio.\n");
        fclose(input);
        return;
    }

    long data_size = total_size - header_size;
    fseek(input, header_size, SEEK_SET);

    unsigned char *data = NULL;
    if (data_size > 0) {
        data = malloc(data_size);
        if (data) {
            size_t bytes_lidos = fread(data, 1, data_size, input);
            if (bytes_lidos != (size_t)data_size) {
                fprintf(stderr, "Aviso: Leitura incompleta. Lidos %zu de %ld bytes\n", bytes_lidos, data_size);
            }
        }
    }
    fclose(input);

    if (!data && data_size > 0) {
        fprintf(stderr, "Erro: Falha na alocação de memória\n");
        return;
    }

    for (int i = num_mapeamentos - 1; i >= 0; i--) {
        unsigned char *new_data = malloc(data_size * 2);
        if (!new_data) {
            fprintf(stderr, "Erro: Falha na alocação durante descompactação\n");
            free(data);
            return;
        }

        size_t new_size = 0;

        for (size_t j = 0; j < data_size; j++) {
            if (data[j] == SIMBOLO_INICIAL + i) {
                new_data[new_size++] = pares_first[i];
                new_data[new_size++] = pares_second[i];
            } else {
                new_data[new_size++] = data[j];
            }
        }

        free(data);
        data = new_data;
        data_size = new_size;
    }

    FILE *output = fopen(output_filename, "wb");
    if (output) {
        if (data_size > 0) {
            fwrite(data, 1, data_size, output);
        }
        fclose(output);
    } else {
        perror("Erro ao criar arquivo de saída");
    }
    free(data);
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Uso: %s <c|d> <entrada> <saida> <threads>\n", argv[0]);
        return 1;
    }

    char comando = argv[1][0];
    const char *input_filename = argv[2];
    const char *output_filename = argv[3];
    int num_threads = atoi(argv[4]);

    FILE *input = fopen(input_filename, "rb");
    if (!input) {
        perror("Erro ao abrir arquivo de entrada");
        return 1;
    }

    fseek(input, 0, SEEK_END);
    long file_size = ftell(input);
    fseek(input, 0, SEEK_SET);

    if (file_size <= 0) {
        fprintf(stderr, "Erro: Arquivo vazio ou inválido\n");
        fclose(input);
        return 1;
    }

    if (file_size > 100000000) {
        fprintf(stderr, "Erro: Arquivo muito grande\n");
        fclose(input);
        return 1;
    }

    unsigned char *data = malloc(file_size);
    if (!data) {
        fprintf(stderr, "Erro: Falha na alocação de memória\n");
        fclose(input);
        return 1;
    }

    size_t bytes_lidos = fread(data, 1, file_size, input);
    if (bytes_lidos != (size_t)file_size) {
        fprintf(stderr, "Aviso: Leitura incompleta. Lidos %zu de %ld bytes\n", bytes_lidos, file_size);
    }
    fclose(input);

    if (comando == 'c') {
        compactar(data, file_size, output_filename, num_threads);
    } else if (comando == 'd') {
        descompactar(input_filename, output_filename);
    } else {
        fprintf(stderr, "Comando inválido. Use 'c' para compactar ou 'd' para descompactar.\n");
        free(data);
        return 1;
    }

    free(data);
    return 0;
}