public class FiltroCategoriaIgualA  implements FiltroStrategy{
    private String categoria;

    public FiltroCategoriaIgualA(String categoria){
        this.categoria = categoria;
    }

    @Override
    public boolean verificar(Produto produto) {
        return produto.getCategoria().equalsIgnoreCase(this.categoria);
    }
}
