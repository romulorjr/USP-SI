import java.util.List;
import java.util.Comparator;

public interface AlgoritmoOrdenacao {
    void ordenar(List<Produto> produtos, Comparator<Produto> criterio);
}