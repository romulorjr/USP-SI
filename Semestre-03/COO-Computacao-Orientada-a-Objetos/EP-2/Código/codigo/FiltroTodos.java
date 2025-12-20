public class FiltroTodos implements FiltroStrategy {
    @Override
    public boolean verificar(Produto produto) {
        return true; // Sempre retorna true, incluindo todos os produtos.
    }
}