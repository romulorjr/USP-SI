public class FiltroPrecoDentroDeFaixa implements FiltroStrategy{
    private double precoMin;
    private double precoMax;

    public FiltroPrecoDentroDeFaixa(double precoMin, double precoMax){
        this.precoMin = precoMin;
        this.precoMax = precoMax;
    }
    @Override
    public boolean verificar(Produto produto) {
        double preco = produto.getPreco();
        return preco >= this.precoMin && preco <= this.precoMax;
    }
}
