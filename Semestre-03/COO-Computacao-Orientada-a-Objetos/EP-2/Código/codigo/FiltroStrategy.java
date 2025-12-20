import java.util.List;
import java.util.ArrayList;

public interface FiltroStrategy {
    /**
     * Verifica se o produto dado satisfaz o critério de filtragem.
     * @param produto O produto a ser verificado.
     * @return true se o produto satisfaz o critério, false caso contrário.
     */
    boolean verificar(Produto produto);
}