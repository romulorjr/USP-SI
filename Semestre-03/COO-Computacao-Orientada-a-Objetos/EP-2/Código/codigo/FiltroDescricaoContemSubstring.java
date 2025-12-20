public class FiltroDescricaoContemSubstring implements FiltroStrategy {
    private String substring;

    public FiltroDescricaoContemSubstring(String substring) {
        this.substring = substring;
    }

    @Override
    public boolean verificar(Produto produto) {
        return produto.getDescricao().toLowerCase().contains(this.substring.toLowerCase());
    }
}