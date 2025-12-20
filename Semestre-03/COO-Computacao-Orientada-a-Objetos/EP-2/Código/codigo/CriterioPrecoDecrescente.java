import java.util.Comparator;

public class CriterioPrecoDecrescente implements Comparator<Produto> {
    @Override
    public int compare(Produto p1, Produto p2) {
        // Invertemos p1 e p2 para obter a ordem decrescente
        return Double.compare(p2.getPreco(), p1.getPreco());
    }
}