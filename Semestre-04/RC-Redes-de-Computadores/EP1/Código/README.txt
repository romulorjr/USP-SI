ACH2026 - Exercício Programa: Jogo da Forca em Rede

Linguagem: Python 3

Será necessário abrir ao menos três terminais para a execução do programa (um para o servidor e ao menos um jogador mestre e um jogador comum)

1. O servidor deve ser iniciado primeiro, ele irá esperar o número de jogadores especificado para iniciar a partida.
	Para isso, utilize o comando: python3 hangman-server.py <num_players> [<porta>]
	Onde: <num_players>: (Obrigatório) O número total de jogadores que participarão da partida. Deve ser no mínimo 2.
	       [<porta>]   : (Opcional) A porta TCP em que o servidor irá escutar. Se omitido, o padrão é 6891

2. Cliente: depois de rodar o servidor, os clientes podem se conectar com o comando: python3 hangman-client.py <nome> [<IP>:<PORTA>]
	Onde:    <nome>     : (Obrigatório) O nome que você usará no jogo. Não pode conter espaços ou caracteres especiais.
              [<IP>:<PORTA>]: (Opcional) O endereço e a porta do servidor. Se omitido, o padrão é 127.0.0.1:6891

3. O jogo começa assim que atingir o número mínimo de jogadores especificados.