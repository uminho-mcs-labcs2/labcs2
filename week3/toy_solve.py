#!/usr/bin/env python3
"""Exemplo resolvido: resolver o toy_server.py com pwntools.

    python3 toy_solve.py                 # arranca o toy_server.py localmente
    python3 toy_solve.py localhost 4444  # liga-se por TCP, como nc localhost 4444
"""
import re
import sys

from pwn import *

context.log_level = "info"  # mudar para "debug" para ver cada byte enviado e recebido

if len(sys.argv) == 3:
    io = remote(sys.argv[1], int(sys.argv[2]))
else:
    io = process(["python3", "toy_server.py"])

io.recvline()  # salta "Bem-vindos! ..."
for _ in range(30):
    question = io.recvuntil(b"Resposta: ").decode()  # "Ronda 1: quanto é 123 + 456?\nResposta: "
    a, b = re.search(r"(\d+) \+ (\d+)", question).groups()
    io.sendline(str(int(a) + int(b)).encode())

print(io.recvall(timeout=2).decode())
