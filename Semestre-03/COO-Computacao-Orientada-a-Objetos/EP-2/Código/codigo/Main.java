import java.io.IOException;
import java.util.List;
import java.util.Comparator;

public class Main {

    public static void main(String[] args) {

        // --- CARREGAMENTO DOS DADOS ---
        // Carrega a lista de produtos do arquivo CSV [cite: 50]
        List<Produto> produtos = CarregadorDeProdutos.carregar("produtos.csv");

        if (produtos.isEmpty()) {
            System.out.println("Nenhum produto foi carregado. Verifique o arquivo produtos.csv.");
            return;
        }

        // --- DECORAÇÃO (EXEMPLO) ---
        // Vamos decorar o primeiro produto com Negrito e o segundo com Itálico e Cor
        // Isso demonstra como a formatação pode ser aplicada individualmente [cite: 32]
        if (produtos.size() > 0) {
            produtos.set(0, new ProdutoEmNegrito(produtos.get(0)));
        }
        if (produtos.size() > 1) {
            produtos.set(1, new ProdutoCor(new ProdutoEmItalico(produtos.get(1)), "blue"));
        }


        // --- CONFIGURAÇÃO DAS ESTRATÉGIAS ---
        AlgoritmoOrdenacao algoritmo = new QuickSort(); // [cite: 20]

        // Critério: Ordenar por preço em ordem crescente [cite: 23]
        Comparator<Produto> criterio = new CriterioPrecoCrescente();

        // Filtro: Filtrar produtos da categoria "informatica" [cite: 25]
        FiltroStrategy filtro = new FiltroCategoriaIgualA("informatica");


        // --- GERAÇÃO DO RELATÓRIO ---
        try {
            // Instancia o gerador com os produtos e as estratégias escolhidas
            GeradorDeRelatorios gerador = new GeradorDeRelatorios(produtos, algoritmo, criterio, filtro);

            // Gera o relatório
            gerador.gerarRelatorio("relatorio.html");

            System.out.println("Relatorio gerado com sucesso em relatorio.html!");

        } catch (IOException e) {
            System.err.println("Erro ao gerar o relatorio: " + e.getMessage());
            e.printStackTrace();
        }
    }
}