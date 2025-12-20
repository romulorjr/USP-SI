
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class CarregadorDeProdutos {

    public static List<Produto> carregar(String caminhoArquivo) {
        List<Produto> produtos = new ArrayList<>();

        try (BufferedReader br = new BufferedReader(new FileReader(caminhoArquivo))) {

            br.readLine(); // Pular a linha do cabeçalho

            String linha;
            while ((linha = br.readLine()) != null) {

                String[] valores = linha.split(",");

                // ATUALIZAÇÃO: Agora esperamos 8 colunas!
                if (valores.length == 8) {
                    try {
                        // Mapeamento correto das colunas
                        int id = Integer.parseInt(valores[0].trim());
                        String descricao = valores[1].trim();
                        String categoria = valores[2].trim();
                        int qtdEstoque = Integer.parseInt(valores[3].trim()); // Coluna 4 é o estoque
                        double preco = Double.parseDouble(valores[4].trim());    // Coluna 5 é o preço

                        // Cria o produto base
                        Produto produto = new ProdutoPadrao(id, descricao, categoria, qtdEstoque, preco);

                        // --- LÓGICA DE DECORAÇÃO DINÂMICA ---
                        // Lê as flags de formatação do CSV
                        boolean negrito = Boolean.parseBoolean(valores[5].trim());
                        boolean italico = Boolean.parseBoolean(valores[6].trim());
                        String cor = valores[7].trim();

                        // Aplica os decoradores se necessário
                        if (negrito) {
                            produto = new ProdutoEmNegrito(produto);
                        }
                        if (italico) {
                            produto = new ProdutoEmItalico(produto);
                        }
                        // Aplica o decorador de cor apenas se não for a cor padrão/nula
                        if (!cor.equalsIgnoreCase("#000000") && !cor.equalsIgnoreCase("null")) {
                            produto = new ProdutoCor(produto, cor);
                        }

                        // Adiciona o produto (já decorado) à lista
                        produtos.add(produto);

                    } catch (NumberFormatException e) {
                        System.err.println("Erro ao converter um número na linha: " + linha);
                    }
                } else {
                    System.err.println("Linha com formato inesperado ("+ valores.length +" colunas): " + linha);
                }
            }
        } catch (IOException e) {
            System.err.println("Erro ao ler o arquivo CSV: " + e.getMessage());
            e.printStackTrace();
        }

        return produtos;
    }
}