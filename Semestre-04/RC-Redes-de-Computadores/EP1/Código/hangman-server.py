import socket
import sys
import random

VIDAS_INICIAIS = 7
PORTA_PADRAO = 6891
HOST = '0.0.0.0'



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
                raise ConnectionAbortedError("Cliente desconectou")
            mensagem += byte
            if mensagem.endswith(b"\r\n"):
                return mensagem[:-2].decode('ascii')
        except (ConnectionAbortedError, ConnectionResetError) as e:
            print(f"Erro de conexão ao ler mensagem: {e}")
            raise
def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print(f"Uso: python3 {sys.argv[0]} <num_players> [<port>]")
        print(f"Porta padrão: {PORTA_PADRAO}")
        sys.exit(1)

    try:
        NUM_PLAYERS = int(sys.argv[1])
        if NUM_PLAYERS < 2:
            print("Erro: O número de jogadores deve ser pelo menos 2 (1 mestre, 1 jogador).")
            sys.exit(1)

        PORTA = int(sys.argv[2]) if len(sys.argv) == 3 else PORTA_PADRAO
    except ValueError:
        print("Erro: Número de jogadores e porta devem ser inteiros.")
        sys.exit(1)

    print(f"Servidor inicializado na porta {PORTA}")

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((HOST, PORTA))
        server_socket.listen()

        while True:
            print("\nIniciando novo jogo...")
            jogadores = []

            try:
                #lobby e conexão
                for i in range(NUM_PLAYERS):
                    print(f"Aguardando jogador {i + 1}...")
                    conn, addr = server_socket.accept()
                    mensagem_cliente = receber_mensagem(conn)
                    print(f"Jogador {addr} disse: {mensagem_cliente}")

                    if mensagem_cliente.startswith("NEWPLAYER "):
                        nome_jogador = mensagem_cliente.split(" ", 1)[1]

                        if not nome_jogador or not nome_jogador.isalnum():
                            print(f"Nome de jogador inválido: '{nome_jogador}'. Rejeitando.")
                            conn.sendall(b"ERROR INVALID_PLAYER_NAME\r\n")
                            conn.close()
                            raise ConnectionAbortedError("Protocolo Inválido - Nome")

                        jogadores.append((conn, nome_jogador))
                        print(f"Jogador conectado: {nome_jogador}")

                        resposta = "STANDBY\r\n"
                        conn.sendall(resposta.encode())
                    else:
                        print(f"Mensagem inesperada de {addr}: {mensagem_cliente}")
                        conn.sendall(b"ERROR UNEXPECTED_MESSAGE\r\n")
                        conn.close()
                        raise ConnectionAbortedError("Protocolo Inválido - NEWPLAYER")

                print("Todos os jogadores conectados. Escolhendo o mestre...")

                # seleção do mestre e palavra
                mestre_conn, mestre_nome = random.choice(jogadores)
                print(f"Jogador mestre: {mestre_nome}")

                jogadores_comuns = [j for j in jogadores if j[0] != mestre_conn]
                mestre_conn.sendall(b"MASTER\r\n")
                msg_palavra = receber_mensagem(mestre_conn)

                palavra_secreta = ""
                if msg_palavra.startswith("WORD "):
                    palavra_secreta = msg_palavra.split(" ", 1)[1]
                    palavra_secreta = palavra_secreta.lower()

                    if not palavra_secreta or not palavra_secreta.isalpha():
                        print(f"Mestre enviou palavra inválida: {msg_palavra.split(' ', 1)[1]}")
                        msg_erro = b"ERROR INVALID_MASTER_MESSAGE\r\n"
                        mestre_conn.sendall(msg_erro)
                        for conn, _ in jogadores_comuns:
                            conn.sendall(msg_erro)
                        raise ConnectionAbortedError("Palavra inválida do mestre")
                    print(f"Jogador mestre forneceu a palavra: {palavra_secreta}")
                    mestre_conn.sendall(b"OK\r\n")
                else:
                    print(f"Mestre enviou mensagem inesperada: {msg_palavra}")
                    mestre_conn.sendall(b"ERROR INVALID_MASTER_MESSAGE\r\n")
                    raise ConnectionAbortedError("Mestre não enviou a palavra")

                # início do Jogo
                tamanho_palavra = len(palavra_secreta)
                msg_newgame = f"NEWGAME {VIDAS_INICIAIS} {tamanho_palavra}\r\n"
                print(f"Enviando NEWGAME para todos. Vidas: {VIDAS_INICIAIS}, Tamanho da Palavra: {tamanho_palavra}")
                for conn, nome in jogadores:
                    conn.sendall(msg_newgame.encode())

                print("Jogo iniciado com sucesso!")

                #loop Principal do Jogo
                vidas_restantes = VIDAS_INICIAIS
                estado_palavra = ["_"] * tamanho_palavra
                palpites_feitos = set()
                jogador_da_vez_idx = 0
                ultimo_jogador_nome = ""

                jogo_terminou = False
                resultado_jogo = "LOSE" 

                while not jogo_terminou:
                    jogador_conn, jogador_nome = jogadores_comuns[jogador_da_vez_idx]
                    ultimo_jogador_nome = jogador_nome

                    print(f"Vez do jogador {jogador_nome}.")

                    try:
                        jogador_conn.sendall(b"YOURTURN\r\n")
                        msg_palpite = receber_mensagem(jogador_conn)
                    except Exception as e:
                        print(f"Jogador {jogador_nome} desconectou. Removendo do jogo.")
                        jogador_conn.close()
                        jogadores_comuns.pop(jogador_da_vez_idx)

                        if not jogadores_comuns:
                            print("Não há mais jogadores comuns. Jogo encerrado.")
                            jogo_terminou = True
                            continue

                        jogador_da_vez_idx = jogador_da_vez_idx % len(jogadores_comuns)
                        continue

                    jogador_conn.sendall(b"OK\r\n")

                    palpite_processado = False

                    if msg_palpite == "QUIT":
                        print(f"Jogador {jogador_nome} enviou QUIT.")
                        jogador_conn.close()
                        jogadores_comuns.pop(jogador_da_vez_idx)
                        if not jogadores_comuns:
                            print("Não há mais jogadores comuns. Jogo encerrado.")
                            jogo_terminou = True
                        # mão avança o índice, pois a lista diminuiu
                        jogador_da_vez_idx = jogador_da_vez_idx % len(jogadores_comuns) if jogadores_comuns else 0
                        continue

                    elif msg_palpite.startswith("GUESS "):
                        partes = msg_palpite.split(" ", 2)
                        if len(partes) != 3:
                            jogador_conn.sendall(b"ERROR INVALID_FORMAT\r\n")
                            continue  # pede o palpite novamente para o mesmo jogador

                        tipo_palpite, palpite = partes[1], partes[2].lower()
                        ultimo_palpite_str = partes[2]

                        if palpite in palpites_feitos:
                            print(f"Palpite repetido: {palpite}")
                            jogador_conn.sendall(b"ERROR ALREADY_GUESSED\r\n")
                            continue

                        palpites_feitos.add(palpite)

                        if tipo_palpite == "LETTER":
                            print(f"Processando palpite de letra: '{palpite}'")
                            if not palpite.isalpha() or len(palpite) != 1:
                                jogador_conn.sendall(b"ERROR INVALID_LETTER\r\n")
                                palpites_feitos.remove(palpite)
                                continue

                            if palpite in palavra_secreta:
                                # acertou a letra
                                for i, letra in enumerate(palavra_secreta):
                                    if letra == palpite:
                                        estado_palavra[i] = letra
                            else:
                                # errou a letra
                                vidas_restantes -= 1
                            palpite_processado = True

                        elif tipo_palpite == "WORD":
                            print(f"Processando palpite de palavra: '{palpite}'")
                            if len(palpite) != tamanho_palavra:
                                jogador_conn.sendall(b"ERROR INVALID_WORD_LENGTH\r\n")
                                palpites_feitos.remove(palpite)
                                continue

                            if palpite == palavra_secreta:
                                # jogo ganho.
                                estado_palavra = list(palavra_secreta)
                            else:
                                # errou a palavra
                                vidas_restantes -= 1
                            palpite_processado = True

                        else:
                            jogador_conn.sendall(b"ERROR INVALID_FORMAT\r\n")
                            palpites_feitos.remove(palpite)
                            continue  # pede o palpite novamente

                    else:
                        jogador_conn.sendall(b"ERROR UNEXPECTED_MESSAGE\r\n")
                        continue 

                    estado_atual_str = "".join(estado_palavra)

                    if "_" not in estado_palavra:
                        # ganhou
                        jogo_terminou = True
                        resultado_jogo = "WIN"
                        print(f"Jogador {jogador_nome} adivinhou a palavra!")

                    elif vidas_restantes == 0:
                        # perdeu
                        jogo_terminou = True
                        resultado_jogo = "LOSE"
                        print("A palavra não foi adivinhada.")
                        print(f"Último jogador: {jogador_nome}")

                    if not jogo_terminou and palpite_processado:
                        print(f"Continuando jogo. Estado atual: {estado_atual_str}, vidas restantes: {vidas_restantes}")

                        msg_status = f"STATUS {vidas_restantes} {estado_atual_str} {jogador_nome} {ultimo_palpite_str}\r\n"

                        for conn, _ in jogadores:
                            try:
                                conn.sendall(msg_status.encode())
                            except Exception as e:
                                print(
                                    f"Erro ao enviar STATUS para jogador (será removido na próxima rodada se for o turno dele): {e}")

                    if palpite_processado:
                        jogador_da_vez_idx = (jogador_da_vez_idx + 1) % len(jogadores_comuns)

                print("Finalizando jogo....")

                msg_gameover = f"GAMEOVER {resultado_jogo} {ultimo_jogador_nome} {palavra_secreta}\r\n"

                for conn, _ in jogadores:
                    try:
                        conn.sendall(msg_gameover.encode())
                        conn.close()  # fecha a conexão
                    except Exception as e:
                        print(f"Erro ao fechar conexão: {e}")

            except (ConnectionAbortedError, ConnectionResetError) as e:
                print(f"Erro na fase de jogo: {e}")
                print("Reiniciando o lobby para um novo jogo.")
                # fecha todas as conexões ativas antes de reiniciar
                for conn, _ in jogadores:
                    try:
                        conn.close()
                    except:
                        pass

            except KeyboardInterrupt:
                print("\nServidor sendo desligado por comando (Ctrl + C).")
                for conn, _ in jogadores:
                    conn.close()
                break 


if __name__ == "__main__":
    main()