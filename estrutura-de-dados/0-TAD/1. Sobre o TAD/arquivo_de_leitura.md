# TAD — Tipo Abstrato de Dados

## O que é?

**TAD** significa **Tipo Abstrato de Dados** (em inglês, *Abstract Data Type* — ADT).

A ideia central é separar duas coisas:

1. **O QUE o tipo faz** (sua interface / comportamento) — as operações que você pode realizar com ele.
2. **COMO ele é implementado por dentro** (sua representação / estrutura interna) — os detalhes de memória, campos, algoritmos.

Quem usa o TAD só precisa saber o **o quê**. O **como** fica escondido (encapsulado).

---

## Analogia

É com um controle remoto de TV: você sabe que o botão "vol +" aumenta o volume. Você **não precisa saber** como o circuito interno faz isso. Se amanhã trocarem o circuito por um totalmente diferente, mas o botão continuar fazendo a mesma coisa, você nem percebe a diferença. O TAD é exatamente isso aplicado a estruturas de dados: a "operação" (botão) continua igual, mesmo que a "implementação" (circuito) mude por dentro.

---

## Como isso é simulado em C

C não tem "classes" como Java ou C++. Por isso, o TAD é simulado combinando:

| Elemento | Papel |
|---|---|
| **`struct`** (geralmente no `.h`) | Define os dados que o tipo guarda (a representação interna) |
| **Funções** (protótipos no `.h`, implementação no `.c`) | Definem as operações permitidas sobre o tipo (a interface pública) |
| **Separação `.h` / `.c`** | Quem *usa* o TAD só inclui o `.h` e chama as funções — não deveria mexer diretamente nos campos da struct |

### Exemplo de organização de arquivos

```
ponto.h    -> declara "typedef struct Ponto" e os protótipos das funções
ponto.c    -> implementa as funções (criar_ponto, distancia, imprimir, etc.)

quadrado.h -> declara "struct Quadrado" (provavelmente usando Ponto por dentro)
quadrado.c -> implementa as operações do quadrado
```

---

## Por que isso importa em Estrutura de Dados Lineares?

Porque é exatamente esse conceito que será aplicado em:

- **Pilha (Stack)**: operações como `push`, `pop`, `topo`, sem o usuário mexer direto no array/lista interna.
- **Fila (Queue)**: `enfileirar`, `desenfileirar`, sem acessar os ponteiros de início/fim diretamente.
- **Lista Encadeada**: `inserir`, `remover`, `buscar`, escondendo a manipulação de `nó->próximo`.

Em todos esses casos, quem usa a estrutura chama as funções (a interface) e não precisa saber (nem deveria mexer) na forma como os dados estão organizados por dentro.

---

## Vantagens do TAD

1. **Encapsulamento** — protege os dados de manipulação indevida.
2. **Modularidade** — a implementação interna pode ser trocada sem quebrar quem usa o TAD.
3. **Reuso** — o mesmo TAD (ex: `Ponto`) pode ser usado dentro de outros TADs (ex: `Quadrado` usando 2 ou 4 `Ponto`s).
4. **Organização** — separa "o quê" de "como", facilitando entender, manter e testar o código.

---

## Resumo rápido

> TAD = conjunto de dados (struct) + conjunto de operações (funções) que manipulam esses dados, expostos através de uma interface (`.h`), escondendo os detalhes de implementação (`.c`).
