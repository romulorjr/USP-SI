public class FiltroEstoqueMenorIgualA implements FiltroStrategy {
    private int estoqueMaximo;

    public FiltroEstoqueMenorIgualA(int estoqueMaximo) {
        this.estoqueMaximo = estoqueMaximo;
    }

    @Override
    public boolean verificar(Produto produto) {
        return produto.getQtdEstoque() <= this.estoqueMaximo;
    }
}