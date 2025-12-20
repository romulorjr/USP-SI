import java.util.Comparator;

public class CriterioEstoqueDecrescente implements Comparator<Produto> {
    @Override
    public int compare(Produto p1, Produto p2) {
        return Integer.compare(p2.getQtdEstoque(), p1.getQtdEstoque());
    }
}