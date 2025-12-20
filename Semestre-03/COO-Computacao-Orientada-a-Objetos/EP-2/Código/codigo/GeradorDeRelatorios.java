import java.io.PrintWriter;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;

public class GeradorDeRelatorios {

	private List<Produto> produtos;
	private AlgoritmoOrdenacao algoritmoOrdenacao;
	private Comparator<Produto> criterioOrdenacao;
	private FiltroStrategy filtroStrategy;

	/**
	 * Construtor que recebe a lista de produtos e as estratégias a serem usadas.
	 */
	public GeradorDeRelatorios(List<Produto> produtos, AlgoritmoOrdenacao algoritmo, Comparator<Produto> criterio, FiltroStrategy filtro) {
		this.produtos = produtos;
		this.algoritmoOrdenacao = algoritmo;
		this.criterioOrdenacao = criterio;
		this.filtroStrategy = filtro;
	}

	/**
	 * Orquestra a geração do relatório, aplicando as estratégias e salvando em um arquivo.
	 * @param arquivoSaida O nome do arquivo HTML a ser gerado.
	 */
	public void gerarRelatorio(String arquivoSaida) throws IOException {

		// 1. Filtrar os produtos usando a estratégia de filtro
		List<Produto> produtosFiltrados = new ArrayList<>();
		for (Produto produto : this.produtos) {
			if (filtroStrategy.verificar(produto)) {
				produtosFiltrados.add(produto);
			}
		}

		// 2. Ordenar a lista de produtos filtrados usando as estratégias de ordenação
		algoritmoOrdenacao.ordenar(produtosFiltrados, criterioOrdenacao);

		// 3. Gerar o conteúdo HTML
		String html = gerarHtml(produtosFiltrados);

		// 4. Salvar o HTML em um arquivo
		try (PrintWriter out = new PrintWriter(arquivoSaida)) {
			out.println(html);
		}
	}

	/**
	 * Gera o código HTML a partir de uma lista de produtos.
	 * @param produtosParaRelatorio A lista de produtos a ser incluída no relatório.
	 * @return Uma string contendo o relatório em formato HTML.
	 */
	private String gerarHtml(List<Produto> produtosParaRelatorio) {
		StringBuilder sb = new StringBuilder();
		sb.append("<html><head><title>Relatorio de Produtos</title></head><body>");
		sb.append("<h1>Relatorio de Produtos</h1>");
		sb.append("<table border=\"1\"><tr>");
		sb.append("<th>Descricao</th><th>Preco</th><th>Estoque</th><th>Categoria</th>");
		sb.append("</tr>");

		for (Produto p : produtosParaRelatorio) {
			sb.append("<tr>");
			// A mágica do Decorator acontece aqui!
			// A chamada é a mesma para produtos decorados ou não.
			sb.append("<td>").append(p.formataParaImpressao()).append("</td>");
			sb.append("</tr>");
		}

		sb.append("</table></body></html>");
		return sb.toString();
	}
}