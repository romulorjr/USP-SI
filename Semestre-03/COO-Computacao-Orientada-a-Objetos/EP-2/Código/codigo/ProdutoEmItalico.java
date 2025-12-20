// Arquivo: ProdutoEmItalico.java
public class ProdutoEmItalico extends ProdutoDecorator {
    public ProdutoEmItalico(Produto p) { super(p); }
    @Override
    public String formataParaImpressao() {
        return "<i>" + produto.formataParaImpressao() + "</i>";
    }
}