import java.util.Comparator;

public class CriterioDescricaoCrescente implements Comparator<Produto> {
    @Override
    public int compare(Produto p1, Produto p2) {
        return p1.getDescricao().compareToIgnoreCase(p2.getDescricao());
    }
}