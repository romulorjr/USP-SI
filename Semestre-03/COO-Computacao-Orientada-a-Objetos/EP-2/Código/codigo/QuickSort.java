// Arquivo: QuickSort.java

import java.util.List;
import java.util.Collections;
import java.util.Comparator;

/**
 * Implementação do algoritmo de ordenação QuickSort.
 * Esta classe segue o padrão Strategy para ser usada de forma intercambiável.
 */
public class QuickSort implements AlgoritmoOrdenacao {

    @Override
    public void ordenar(List<Produto> produtos, Comparator<Produto> criterio) {
        // Chama o método recursivo que realiza a ordenação
        quicksort(produtos, 0, produtos.size() - 1, criterio);
    }

    /**
     * O método recursivo principal do QuickSort.
     * @param produtos A lista de produtos a ser ordenada.
     * @param inicio O índice inicial da partição.
     * @param fim O índice final da partição.
     * @param criterio O critério de comparação a ser utilizado.
     */
    private void quicksort(List<Produto> produtos, int inicio, int fim, Comparator<Produto> criterio) {
        if (inicio < fim) {
            // Encontra o pivô tal que os elementos menores estão à esquerda
            // e os maiores à direita.
            int indicePivo = particionar(produtos, inicio, fim, criterio);

            // Ordena recursivamente as duas sub-listas
            quicksort(produtos, inicio, indicePivo - 1, criterio);
            quicksort(produtos, indicePivo + 1, fim, criterio);
        }
    }

    /**
     * O método de particionamento do QuickSort.
     * @param produtos A lista de produtos.
     * @param inicio O índice inicial.
     * @param fim O índice final.
     * @param criterio O critério de comparação.
     * @return O índice do pivô após o particionamento.
     */
    private int particionar(List<Produto> produtos, int inicio, int fim, Comparator<Produto> criterio) {
        // O pivô é escolhido como o último elemento
        Produto pivo = produtos.get(fim);
        int i = (inicio - 1); // Índice do menor elemento

        for (int j = inicio; j < fim; j++) {
            // Se o elemento atual for menor ou igual ao pivô
            // A comparação é feita aqui, usando o critério (Comparator)
            if (criterio.compare(produtos.get(j), pivo) <= 0) {
                i++;
                // Troca o elemento em i com o elemento em j
                Collections.swap(produtos, i, j);
            }
        }

        // Troca o pivô (produtos.get(fim)) para sua posição correta
        Collections.swap(produtos, i + 1, fim);

        return i + 1;
    }
}