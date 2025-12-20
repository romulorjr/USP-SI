public class ProdutoEmNegrito extends ProdutoDecorator {
    public ProdutoEmNegrito(Produto p) { super(p); }
    @Override
    public String formataParaImpressao() {
        return "<b>" + produto.formataParaImpressao() + "</b>";
    }
}