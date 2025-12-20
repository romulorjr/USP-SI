public class ProdutoCor extends ProdutoDecorator {
    private String cor;

    public ProdutoCor(Produto p, String cor) {
        super(p);
        this.cor = cor;
    }

    @Override
    public String formataParaImpressao() {
        return "<font color=\"" + this.cor + "\">" + produto.formataParaImpressao() + "</font>";
    }
}