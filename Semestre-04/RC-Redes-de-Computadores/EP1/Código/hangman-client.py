import socket
import sys

PORTA_PADRAO = 6891
HOST_PADRAO = '127.0.0.1'


def receber_mensagem(conn):
    """
    Lê dados do socket até encontrar um '\r\n'.
    Isso é crucial para lidar com o TCP como um stream (Seção 6.1 do EP).
    """
    mensagem = b""
    while True:
        try:
            byte = conn.recv(1)
            if not byte:
                #conexão fechada inesperadamente
                raise ConnectionAbortedError("Servidor desconectou")
            mensagem += byte
            if mensagem.endswith(b"\r\n"):
                return mensagem[:-2].decode('ascii')
        except (ConnectionAbortedError, ConnectionResetError) as e:
            print(f"\nErro de conexão ao ler mensagem: {e}")
            raise

def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print(f"Uso: python3 {sys.argv[0]} <nome> [<IP>:<PORTA>]")
        print(f"Endereço padrão: {HOST_PADRAO}:{PORTA_PADRAO}")
        sys.exit(1)

    NOME_JOGADOR = sys.argv[1]
    HOST, PORTA = HOST_PADRAO, PORTA_PADRAO

    if len(sys.argv) == 3:
        try:
            HOST, PORTA_STR = sys.argv[2].split(':')
            PORTA = int(PORTA_STR)
        except ValueError:
            print("Endereço do servidor inválido. Use o formato IP:PORTA")
            sys.exit(1)

    # mapeia vidas restantes para o desenho.
    # 7 vidas = 0 erros. 1 vida = 6 erros.
    DESENHOS_FORCA = {
        7: """
           -----
           |   |
           |
           |
           |
           |
        -------
        """,
        6: """
           -----
           |   |
           |   O
           |
           |
           |
        -------
        """,
        5: """
           -----
           |   |
           |   O
           |   |
           |
           |
        -------
        """,
        4: """
           -----
           |   |
           |   O
           |  /|
           |
           |
        -------
        """,
        3: """
           -----
           |   |
           |   O
           |  /|\\
           |
           |
        -------
        """,
        2: """
           -----
           |   |
           |   O
           |  /|\\
           |  /
           |
        -------
        """,
        1: """
           -----
           |   |
           |   O
           |  /|\\
           |  / \\
           |
        -------
        """,
    }

    print("Conectando ao servidor...")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        try:
            client_socket.connect((HOST, PORTA))
            print("Conectado!")
            mensagem = f"NEWPLAYER {NOME_JOGADOR}\r\n"
            client_socket.sendall(mensagem.encode())

            resposta_servidor = receber_mensagem(client_socket)

            if resposta_servidor == "STANDBY":
                print("Aguardando o jogo começar...")
            else:
                print(f"Erro: Servidor não respondeu STANDBY. Resposta: {resposta_servidor}")
                sys.exit(1)

            eh_mestre = False
            jogo_iniciado = False
            vidas_atuais = 0

            while not jogo_iniciado:
                msg = receber_mensagem(client_socket)

                if msg == "MASTER":
                    eh_mestre = True
                    print("Você é o mestre do jogo!")
                    palavra = input("Digite a palavra: ")

                    msg_palavra = f"WORD {palavra}\r\n"
                    client_socket.sendall(msg_palavra.encode())

                    msg_ok = receber_mensagem(client_socket)
                    if msg_ok != "OK":
                        print(f"Erro: Servidor não confirmou a palavra. Resposta: {msg_ok}")
                        return

                    print("Servidor confirmou a palavra (OK). Aguardando início...")

                elif msg.startswith("NEWGAME"):
                    partes = msg.split(" ")
                    if len(partes) != 3:
                        print(f"Erro: Mensagem NEWGAME mal formatada: {msg}")
                        return

                    vidas, tamanho = partes[1], partes[2]
                    vidas_atuais = int(vidas)

                    print("\nJogo iniciado!")
                    print(f"Vidas para adivinhar: {vidas}")
                    print(f"Tamanho da palavra: {tamanho} letras.")
                    
                    print(DESENHOS_FORCA[vidas_atuais])
                    
                    jogo_iniciado = True

                elif msg.startswith("ERROR"):
                    print(f"Erro do servidor: {msg}")
                    return 

                else:
                    print(f"Mensagem inesperada do servidor: {msg}")

            letras_erradas = []
            palavras_erradas = []
            jogo_ativo = True

            while jogo_ativo:
                msg = receber_mensagem(client_socket)

                if msg == "YOURTURN":
                    if eh_mestre:
                        print("Erro: Mestre recebeu YOURTURN.")
                        continue

                    # pede a jogada
                    palpite = input("Digite sua jogada (letra ou palavra), ou \\q para sair: ")

                    if palpite == "\\q":
                        client_socket.sendall(b"QUIT\r\n")
                    elif len(palpite) == 1:
                        client_socket.sendall(f"GUESS LETTER {palpite}\r\n".encode())
                    else:
                        client_socket.sendall(f"GUESS WORD {palpite}\r\n".encode())

                elif msg.startswith("STATUS "):
                    try:
                        _, novas_vidas_str, estado_palavra, jogador, palpite = msg.split(" ", 4)
                        novas_vidas = int(novas_vidas_str)
                    except ValueError:
                        print(f"Erro ao processar STATUS: {msg}")
                        continue

                    if novas_vidas < vidas_atuais:
                        if len(palpite) == 1:
                            if palpite not in letras_erradas:
                                letras_erradas.append(palpite)
                        else:
                            if palpite not in palavras_erradas:
                                palavras_erradas.append(palpite)

                    vidas_atuais = novas_vidas

                    if vidas_atuais in DESENHOS_FORCA:
                        print(DESENHOS_FORCA[vidas_atuais])

                    print(f"{estado_palavra}")
                    print(f"Letras erradas: {' '.join(letras_erradas)}")
                    print(f"Palavras erradas: {' '.join(palavras_erradas)}")
                    print(f"Jogador {jogador} fez uma jogada: {palpite}.")
                    print(f"Restam {vidas_atuais} vidas..")

                elif msg.startswith("GAMEOVER "):
                    try:
                        _, resultado, jogador, palavra = msg.split(" ", 3)
                    except ValueError:
                        print(f"Erro ao processar GAMEOVER: {msg}")
                        continue
                    
                    if resultado == "LOSE":
                        print(DESENHOS_FORCA[1]) # imprime o boneco completo

                    print("\nO jogo terminou.")
                    if resultado == "WIN":
                        print(f"A palavra '{palavra}' foi adivinhada por {jogador}!")
                    else:
                        print(f"A palavra '{palavra}' não foi adivinhada.")
                        print(f"Último palpite por: {jogador}.")

                    jogo_ativo = False  # quebra o loop principal do jogo

                elif msg.startswith("ERROR "):
                    print(f"Servidor: {msg}")

                elif msg == "OK":
                    pass

                else:
                    print(f"Mensagem desconhecida recebida: {msg}")


        except ConnectionRefusedError:
            print(f"Não foi possível conectar ao servidor em {HOST}:{PORTA}")
        except (ConnectionAbortedError, ConnectionResetError):
            print("\nConexão com o servidor perdida.")
        except KeyboardInterrupt:
            print("\nFechando cliente por comando (Ctrl + C).")
        except Exception as e:
            print(f"Ocorreu um erro inesperado: {e}")

    print("Encerrando conexão com servidor...")


if __name__ == "__main__":
    main()