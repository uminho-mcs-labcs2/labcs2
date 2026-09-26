# Preparação para sessão da semana 3

A semana 3 será dedicada a uma sessão CTF de treino. Para o efeito, iremos
recorrer à plataforma [CyLab Security Academy](https://cylabacademy.org), razão
pela qual necessitamos de um *setup* inicial que deve ser assegurado **antes da aula**.

## Registo na plataforma *CyLab Security Academy*

Cada aluno deve dispor de um *login* na plataforma [CyLab Security Academy](https://cylabacademy.org). Admitindo que não o têm ainda, devem seguir o processo de **SignUp** acessíveis na página de entrada (se já dispuserem de *login*, podem usar esse nas sessões de LabCS2).

Uma vês cumprido esse passo, devem preencher a *form* disponibilizada [aqui](https://docs.google.com/forms/d/e/1FAIpQLScp53RZc2e79SULnffPP2vVL9mEeiNj784kJQT_dOIeo65yKg).

## Software de suporte

O guião vai poder ser resolvido quase integralmente recorrendo apenas aos recursos oferecidos pela p´ropria plataforma **CyLab** (e.g. [`WebShell`](webshell.cylabacademy.org)). Recomenda-se, de qualquer forma, a instalação prévia de software de suporte que permite agilizar a realização dos desafios.

> [!NOTE]
> Por regra o a instalação do *software* sugerido nos sistemas *Linux* e *MacOS* é directa. Para o sistema *Windows*, devem instalar o [Windows Subsystem for Linux (WSL)](https://learn.microsoft.com/en-us/windows/wsl/install), possibilitando assim a instalação de *software* como se de um *Linux* se tratasse.

### Ferramentas básicas

- [Python3](https://www.python.org) e [cryptography](https://cryptography.io/en/stable/): a linguagem e biblioteca criptográfica adoptadas (já utilizada durante o curso).
- [NetCat](https://en.wikipedia.org/wiki/Netcat) (ou um dos seus sucedâneos): um utilitário de rede fundamental.
- [wget](https://www.gnu.org/software/wget/) ou [curl](https://curl.se): para descarregar recursos da *web*.

### [pwntools](https://docs.pwntools.com/en/stable/)

O pwntools é uma biblioteca em *Python* vocacionada precisamente para dar suporte a CTFs. A utilização que iremos fazer na sessão 3 é muito limitada (restringe-se a algumas funções básicas), mas servirá também para se tomar contacto com a biblioteca que terá um impacto grande na resolução dos desafios colocados ao longo do semestre.

#### Instalação

```bash
python3 -m venv ~/lcs2-venv
source ~/lcs2-venv/bin/activate
pip install pwntools sympy
```

> [!NOTE]
> Em macOS, se o pip falhar ao instalar alguma dependência, executem `brew install cmake pkg-config` e voltem a tentar.

#### Função básicas

Para a sessão 3, as funções de `pwntools` que poderão ser relevantes restringem-se à interacção e decodificação de dados. Em concreto, podem-se referir (todas acessíveis do *package* `pwn`):

| Função | O que faz |
|---|---|
| `io = remote("host", port)` | Liga-se ao serviço, como `nc host port`. |
| `io = process(["python3", "server.py"])` | Arranca um programa local e comunica com ele da mesma forma. Serve para testar o ataque contra o código-fonte de um desafio, antes de o tentar no servidor real. |
| `io.recvuntil(b"texto")` | Lê até chegar `texto`. Devolve tudo o que leu, incluindo `texto`. |
| `io.recvline()` | Lê uma linha. |
| `io.sendline(b"dados")` | Envia `dados` seguidos de uma mudança de linha. |
| `io.sendlineafter(b"prompt", b"dados")` | Faz `recvuntil(b"prompt")` e depois `sendline(b"dados")`. |
| `io.recvall(timeout=2)` | Lê até o servidor fechar a ligação. |
| `io.interactive()` | Passa a ligação para o teclado. |
| `context.log_level = "debug"` | Mostra cada byte enviado e recebido. É a primeira coisa a ligar quando algo não funciona. |
| `xor(a, b)` | Faz o XOR de duas sequências de bytes. A mais curta repete-se, e o resultado tem o tamanho da mais longa. |

Algumas recomendações:
1. **Tudo são bytes.** Tal como na biblioteca `cryptography`, a regra é a utilização de *bytestrings* `b"..."`. Usem `.decode()` e `.encode()` para converter de/para *strings*.
2. **Primeiro, à mão.** Corram `nc host port` e copiem os prompts exatos. Os espaços e as mudanças de linha contam.
3. **Se o script bloquear,** por vezes pode `recvuntil` estar à espera de um texto que nunca chega. Tentem o modo `debug` e comparem com o que o `nc` mostrou. Com `timeout=5`, o `recvuntil` devolve `b""` em vez de bloquear, por isso verifiquem o que receberam.
4. **Analisem o texto com `re`**, para fazer o *parsing* do *input*, as expressões regulares (da biblioteca *standard* do *Python) são um recurso poderoso -- por exemplo `re.search(r"(\d+) \+ (\d+)", texto).groups()` para separar dois números.
5. Também as conversões de formatos do *Python* serão relevantes: `bytes.fromhex(h)`, `b.hex()`, `int.from_bytes(b, "big")` e `n.to_bytes((n.bit_length() + 7) // 8, "big")`.

#### Treino antes da sessão (10 minutos)

Um pequeno exemplo de utilização que elucida o padrão típico de utilização de `pwntools` em desafios CTF. A ideia consiste em definir uma script que possa resolver o desafio de forma programática, mesmo que este pressuponha interacção.

O exemplo consiste num serviço [`toy_server.py`](toy_server.py) que solicita que se resolvam 30 somas em 10 segundos. É humanamente quase impossível ultrapassa-lo, mas não é difícil definir uma *script* *Pyhton* que o faça. A *script* [`toy_solve.py`](toy_solve.py) faz isso mesmo recorrendo à funcionalidade de `pwntools`. Corram tudo a partir da pasta onde estão os dois ficheiros.

1. Executem `python3 toy_server.py` e joguem à mão. Não deverão conseguir a tempo.
2. Leiam/compreendam o `toy_solve.py` e corram `python3 toy_solve.py`.
3. Mudem para `context.log_level = "debug"`, corram outra vez e leiam o tráfego.
4. Sirvam o jogo por TCP. Num terminal, corram `python3 toy_server.py --port 4444`. Noutro, corram `python3 toy_solve.py localhost 4444`. Quando estiverem perante um desafio do **CyLab** irão fazer o mesmo (com o *host* e respectiva porta).
5. Estraguem propositadamente a script (e.g. mudem `b"Resposta: "` para `b"Resposta:\n"`) e vejam o que acontece. Depois acrescentem `timeout=3` a esse `recvuntil`. O erro que aparece, `'NoneType' object has no attribute 'groups'`, surge mais à frente, mas a causa é o `recvuntil` ter devolvido `b""`.

#### Para explorar...

- **Tubes**, que cobre tudo o que está acima: <https://docs.pwntools.com/en/stable/tubes.html>
- Um tutorial sobre **tubes**, com exemplos: <https://github.com/Gallopsled/pwntools-tutorial/blob/master/tubes.md>
- Primeiros passos: <https://docs.pwntools.com/en/stable/intro.html>

### [SymPy](https://docs.sympy.org/latest/index.html) (opcional)

O *SymPy* é um *package* *Python* para manipulação simbólica -- de alguma forma, oferece funcionalidades semelhantes ao [SageMath](https://www.sagemath.org), mas é mais orientado para ser utilizado como uma biblioteca *pure Python*, em oposição ao *SageMath* que é normalmente utilizado em *notebooks* dedicados.

A componente do *SymPy* que pode ter alguma relevância em desafios CTF é o módule de [Teoria de Números](https://docs.sympy.org/latest/modules/ntheory.html).


