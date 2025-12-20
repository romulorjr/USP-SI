// Arquivo: InsertionSort.java

import java.util.List;
import java.util.Comparator;

/**
 * Implementação do algoritmo de ordenação Insertion Sort.
 * Esta classe segue o padrão Strategy para ser usada de forma intercambiável.
 * [cite_start]O Insertion Sort é eficiente para listas pequenas ou parcialmente ordenadas[cite: 11].
 */
public class InsertionSort implements AlgoritmoOrdenacao {

    @Override
    public void ordenar(List<Produto> produtos, Comparator<Produto> criterio) {

        for (int i = 1; i < produtos.size(); i++) {

            // Seleciona o elemento chave a ser inserido na posição correta
            Produto chave = produtos.get(i);
            int j = i - 1;

            // Move os elementos do sub-array ordenado (produtos[0..i-1]) que são
            // maiores que a chave, uma posição para frente de sua posição atual.
            // A comparação é feita aqui, usando o critério (Comparator).
            while (j >= 0 && criterio.compare(produtos.get(j), chave) > 0) {
                produtos.set(j + 1, produtos.get(j));
                j = j - 1;
            }

            // Insere a chave na sua posição correta
            produtos.set(j + 1, chave);
        }
    }
}