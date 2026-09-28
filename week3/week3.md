# Semana 3

A semana 3 será dedicada a uma sessão CTF de treino com recurso à plataforma
[CyLab Security Academy](https://cylabacademy.org). 
Pressupõe-se que já foram realizados os passos descritos em [week3-prep.md](week3-pred.md).

Note que nesta sessão se pretende valorizar muito mais o "como se faz" do que propriamente o "o que se faz" -- __a obtenção de uma *flag* descontextualizada do processo conduzido que culmina na sua obtenção acabará por ser muito pouco valorizado__.

## Plano

- 14:00 - 14:20: *briefing*
- 14:20 - 16:30: Resolução dos desafios
- 16:30 - 16:50: *debriefing*

## Regras

- Cada aluno faz uso da sua própria conta do **CyLab**. Contributos individuais são contabilizados no *team*.
- Um vez realizado o *login* no **CyLab**, devem selecionar o menu **Classroom**, e a opção **Join with invite code**. O código da sessão é `CkxfPzPGY`.
- A cada desafio realizado, deve corresponder uma directoria do repositório GITHUB do *team*, com nome `week3/<Bloco><Num>` (e.g. `week3/A3`, para o 3º desafio do bloco A). Nessa directoria devem incluir:
  + Um ficheiro `README.md` contendo informação do autor(es) dessa resolução e notas (estratégia, tentativas falhadas, interacções com IA, lições aprendidas, etc.).
  + **Todo o código** desenvolvido de suporte à resolução
- Devem fazer `commit` e `push` sempre que fecham um desafio (não esquecer de submeter a *flag* no **CyLab**!)
- A utilização da IA **não é proibida nem desaconselhada** -- mas pressupõe o _ónus da demonstração do valor acrescentado_ para a vossa formação resultante dessa interacção (e.g. detalhando no `README.md` essa interacção). Em termos práticos:
  + **Nunca delegar na IA a resolução completa do desafio** -- mesmo que se preocupem em perceber a resolução *à posteriori*, o valor formativo do exercício fica sempre muito aquém do esperado.
  + Preocupem-se em delimitar as *prompts* por forma a manterem "do vosso lado" os *insights* principais.
  + Uma boa estratégia é optarem por delegar tarefas na IA que entendam "pouco interessantes" relativamente ao tempo que vos demoraria a realizar (e.g. o *parsing* de texto com uma estrutura intricada).
  + **Nunca** deleguem na IA a escrita do `README.md` (é verdadeiramente *self defeating*!).

## Notas adicionais

- No desafio `B2`, há informação relevante que está omissa no enunciado -- é sabido que o formato da *flag* impõe o prefixo `picoCTF{` (o conhecimento deste caracteres acaba por ser tudo que precisam para possibilitar o ataque requerido).