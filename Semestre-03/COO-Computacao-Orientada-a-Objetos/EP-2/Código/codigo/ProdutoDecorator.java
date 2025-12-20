// Arquivo: ProdutoDecorator.java (Versão Corrigida)

public abstract class ProdutoDecorator implements Produto {

    protected Produto produto;

    public ProdutoDecorator(Produto produto) {
        this.produto = produto;
    }

    // --- Delegação dos Getters ---
    @Override
    public int getId() {
        return this.produto.getId();
    }

    @Override
    public String getDescricao() {
        return this.produto.getDescricao();
    }

    @Override
    public String getCategoria() {
        return this.produto.getCategoria();
    }

    @Override
    public int getQtdEstoque() {
        return this.produto.getQtdEstoque();
    }

    @Override
    public double getPreco() {
        return this.produto.getPreco();
    }

    // --- ADICIONAR DELEGAÇÃO DOS SETTERS (A CORREÇÃO) ---
    @Override
    public void setQtdEstoque(int qtd) {
        this.produto.setQtdEstoque(qtd);
    }

    @Override
    public void setPreco(double preco) {
        this.produto.setPreco(preco);
    }

    // --- Método de formatação dos decoradores ---
    @Override
    public abstract String formataParaImpressao();
}