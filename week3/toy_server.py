#!/usr/bin/env python3
"""Servidor de treino da S3: responder a 30 somas em 10 segundos.

    python3 toy_server.py              # jogar neste terminal
    python3 toy_server.py --port 4444  # servir por TCP; depois: nc localhost 4444
"""
import random
import socketserver
import sys
import time

ROUNDS, LIMIT = 30, 10


def game(read, write):
    write(f"Bem-vindos! Respondam a {ROUNDS} somas em {LIMIT} segundos.\n")
    start = time.time()
    for i in range(1, ROUNDS + 1):
        a, b = random.randint(100, 999), random.randint(100, 999)
        write(f"Ronda {i}: quanto é {a} + {b}?\nResposta: ")
        answer = read().strip()
        if time.time() - start > LIMIT:
            write("Demasiado lento!\n")
            return
        if answer != str(a + b):
            write("Errado!\n")
            return
    write("Muito bem: flag{pr4ct1c3_m4k3s_scr1pts}\n")


class Handler(socketserver.StreamRequestHandler):
    def handle(self):
        game(lambda: self.rfile.readline().decode(errors="replace"),
             lambda s: self.wfile.write(s.encode()))


if __name__ == "__main__":
    if len(sys.argv) == 3 and sys.argv[1] == "--port":
        socketserver.ThreadingTCPServer.allow_reuse_address = True
        with socketserver.ThreadingTCPServer(("127.0.0.1", int(sys.argv[2])), Handler) as srv:
            print(f"à escuta em 127.0.0.1:{sys.argv[2]} (Ctrl-C para parar)", flush=True)
            srv.serve_forever()
    else:
        game(sys.stdin.readline, lambda s: print(s, end="", flush=True))
