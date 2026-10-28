# Lista Duplamente Encadeada (de inteiros)

## Visão geral

Este README explica **apenas o arquivo `lista_dup.c`**, que contém a implementação de uma **Lista Duplamente Encadeada** de inteiros em C. Ela segue o mesmo conjunto de operações da lista simples: `criar_lista`, `inserir_inicio`, `inserir_final`, `remover_inicio`, `remover_final`, `acessar_inicio`, `acessar_final`, `buscar_por_valor`, `buscar_por_posicao` e `destruir`.

### A ideia central

Na lista **simplesmente encadeada**, cada nó só conhece o **próximo** (`prox`). Na **duplamente encadeada**, cada nó conhece o **próximo** e também o **anterior** (`ant`). Além disso, a struct de controle guarda um ponteiro direto para o **último nó** (`fim`).

```
Simples:   inicio → [10|•] → [20|•] → [30|NULL]

Dupla:     inicio → [NULL|10|•] ⇄ [•|20|•] ⇄ [•|30|NULL] ← fim
                       ant valor prox
```

Cada nó é alocado individualmente no heap (`malloc`), então a lista **cresce dinamicamente**, sem limite fixo.

### Simples × Dupla: comparação

| | Simples | Dupla (com `fim`) |
|---|---|---|
| Ponteiros por nó | 1 (`prox`) | 2 (`ant`, `prox`) |
| Memória por nó | menor | um pouco maior |
| `inserir_inicio` | O(1) | O(1) |
| `inserir_final` | O(n): percorre tudo | **O(1)** |
| `remover_inicio` | O(1) | O(1) |
| `remover_final` | O(n): precisa do `ant` andando junto | **O(1)** |
| `acessar_final` | O(n) | **O(1)** |
| `buscar_por_posicao` | sempre do início | começa pelo lado mais próximo |
| Andar para trás | impossível | possível |
| Dificuldade de implementar | menor | maior (mais ponteiros para ajustar) |

---

## `lista_dup.c`: a implementação

### Includes e structs

```c
#include <stdlib.h>
#include "lista.h"

struct elem {
    int valor;
    struct elem* ant;
    struct elem* prox;
};
struct lista {
    int qtd;
    struct elem* inicio;
    struct elem* fim;
};
```

- `<stdlib.h>` fornece `malloc` e `free`.
- `"lista.h"` traz os `typedef`s e as assinaturas das funções.
- As structs são **definidas aqui** (e não no `.h`), o que mantém o TAD **opaco**: quem usa a lista só enxerga os ponteiros `Elem` e `Lista` e não consegue acessar `li->inicio` ou `no->prox` diretamente, só através das funções.
- `Elem` (`struct elem*`) e `Lista` (`struct lista*`) são os apelidos declarados em `lista.h`.

| Campo | Struct | Tipo | Significado |
|---|---|---|---|
| `valor` | `elem` | `int` | O dado guardado no nó |
| `ant` | `elem` | `struct elem*` | Ponteiro para o nó **anterior** (`NULL` se for o primeiro) |
| `prox` | `elem` | `struct elem*` | Ponteiro para o nó **seguinte** (`NULL` se for o último) |
| `qtd` | `lista` | `int` | Quantidade de elementos na lista |
| `inicio` | `lista` | `struct elem*` | Primeiro nó (`NULL` se vazia) |
| `fim` | `lista` | `struct elem*` | Último nó (`NULL` se vazia) |

### Invariantes (regras que sempre devem valer)

Toda função precisa **deixar a lista consistente**. Estas regras são o "teste mental" para saber se uma operação está certa:

1. Lista vazia ⇒ `qtd == 0`, `inicio == NULL` **e** `fim == NULL`.
2. Lista com 1 elemento ⇒ `inicio == fim`, e esse nó tem `ant == NULL` e `prox == NULL`.
3. O `ant` do primeiro nó é sempre `NULL`; o `prox` do último nó é sempre `NULL`.
4. Para quaisquer dois nós vizinhos A e B: `A->prox == B` **se e somente se** `B->ant == A`.

A maioria dos bugs em listas duplas é violar a regra 4 (esquecer de ajustar um dos lados) ou as regras 1 e 2 (esquecer de atualizar `inicio`/`fim` nos casos extremos).

---

### `criar_lista()`

```c
Lista criar_lista(void) {
    Lista li = malloc(sizeof(struct lista));
    if (li != NULL) {
        li->qtd = 0;
        li->inicio = NULL;
        li->fim = NULL;
    }
    return li;
}
```

**O que faz:** cria uma lista vazia.

**Lógica:**
- `malloc(sizeof(struct lista))` aloca no heap **apenas a struct de controle**. Nenhum nó existe ainda.
- `if (li != NULL)`: `malloc` devolve `NULL` quando falta memória; sem esse teste, escrever em `li->qtd` derrubaria o programa.
- `qtd = 0`, `inicio = NULL` e `fim = NULL` estabelecem a **invariante 1** (lista vazia).
- Retorna `li`, que pode ser `NULL` em caso de falha, então **quem chama deve conferir**.
- A assinatura usa `(void)` (e não `()`) porque, em C, parênteses vazios significam "argumentos não especificados", enquanto `void` diz explicitamente "nenhum argumento".

**Complexidade:** O(1).

---

### `inserir_inicio()`

```c
int inserir_inicio(Lista li, int valor_inserir) {
    Elem no = malloc(sizeof(struct elem));
    if (no == NULL)
        return 0;

    no->valor = valor_inserir;
    no->ant = NULL;
    no->prox = li->inicio;

    if (li->inicio == NULL)
        li->fim = no;
    else
        li->inicio->ant = no;

    li->inicio = no;
    li->qtd++;
    return 1;
}
```

**O que faz:** insere um novo valor como **primeiro** elemento.

**Lógica passo a passo:**
1. `malloc` cria o novo nó. Se falhar, retorna `0` sem alterar nada.
2. `no->valor = valor_inserir`: guarda o dado.
3. `no->ant = NULL`: como será o primeiro, **não tem anterior**.
4. `no->prox = li->inicio`: o novo nó aponta para quem era o primeiro até agora.
5. Agora precisamos "avisar" o antigo primeiro (e a lista) da mudança. Há dois casos:
   - **Lista estava vazia** (`li->inicio == NULL`): o novo nó é, ao mesmo tempo, o primeiro e o último, então `li->fim = no`.
   - **Lista tinha elementos**: o antigo primeiro precisa apontar de volta para o novo: `li->inicio->ant = no`. **Este é o passo que não existe na lista simples** e que mais se esquece.
6. `li->inicio = no`: agora o novo nó é oficialmente o primeiro.
7. `li->qtd++` e `return 1`.

**Ordem importa:** o passo 5 usa `li->inicio` (o antigo primeiro), então **precisa vir antes** do passo 6, que sobrescreve `li->inicio`.

**Diagrama (lista `[20]` e inserindo 10):**
```
Antes:   inicio → [NULL|20|NULL] ← fim
Depois:  inicio → [NULL|10|•] ⇄ [•|20|NULL] ← fim
```

**Complexidade:** O(1).

---

### `inserir_final()`

```c
int inserir_final(Lista li, int valor_inserir) {
    Elem no = malloc(sizeof(struct elem));
    if (no == NULL)
        return 0;

    no->valor = valor_inserir;
    no->prox = NULL;
    no->ant = li->fim;

    if (li->fim == NULL)
        li->inicio = no;
    else
        li->fim->prox = no;

    li->fim = no;
    li->qtd++;
    return 1;
}
```

**O que faz:** insere um novo valor como **último** elemento.

**Lógica passo a passo:** é a **imagem espelhada** de `inserir_inicio`.
1. Aloca o nó (retorna `0` se falhar).
2. `no->prox = NULL`: será o último, não tem próximo.
3. `no->ant = li->fim`: aponta para o antigo último.
4. Dois casos:
   - **Lista vazia** (`li->fim == NULL`): o nó é também o primeiro, então `li->inicio = no`.
   - **Lista com elementos**: o antigo último aponta para o novo: `li->fim->prox = no`.
5. `li->fim = no`, `li->qtd++`, `return 1`.

**Comparação com a lista simples:** lá era preciso um `while (aux->prox != NULL)` para **caminhar até o final**, custando O(n). Aqui o `fim` já nos entrega o último nó diretamente.

**Complexidade:** **O(1)** (era O(n) na lista simples).

---

### `remover_inicio()`

```c
int remover_inicio(Lista li) {
    if (li->qtd == 0)
        return 0;

    Elem aux = li->inicio;
    li->inicio = aux->prox;

    if (li->inicio == NULL)
        li->fim = NULL;
    else
        li->inicio->ant = NULL;

    free(aux);
    li->qtd--;
    return 1;
}
```

**O que faz:** remove o **primeiro** elemento.

**Lógica passo a passo:**
1. `if (li->qtd == 0) return 0`: não há o que remover.
2. `aux = li->inicio`: guarda o nó a ser removido **antes** de perder a referência a ele.
3. `li->inicio = aux->prox`: o segundo nó passa a ser o primeiro.
4. Dois casos:
   - **Era o único elemento** (agora `li->inicio == NULL`): a lista ficou vazia, então `li->fim = NULL` também (**invariante 1**). Esquecer isso deixaria `fim` apontando para memória já liberada.
   - **Havia mais elementos**: o novo primeiro ainda aponta para o nó removido por `ant`, e isso precisa ser corrigido: `li->inicio->ant = NULL` (**invariante 3**).
5. `free(aux)` libera a memória do nó removido.
6. `li->qtd--` e `return 1`.

**Complexidade:** O(1).

---

### `remover_final()`

```c
int remover_final(Lista li) {
    if (li->qtd == 0)
        return 0;

    Elem aux = li->fim;
    li->fim = aux->ant;

    if (li->fim == NULL)
        li->inicio = NULL;
    else
        li->fim->prox = NULL;

    free(aux);
    li->qtd--;
    return 1;
}
```

**O que faz:** remove o **último** elemento.

**Lógica passo a passo:** espelho de `remover_inicio`.
1. Se vazia, retorna `0`.
2. `aux = li->fim`: guarda o nó a remover.
3. `li->fim = aux->ant`: o **penúltimo** vira o novo último. Só dá para fazer isso em O(1) porque o próprio último nó **sabe quem é o seu anterior**.
4. Dois casos:
   - **Era o único elemento** (`li->fim == NULL`): lista vazia, então `li->inicio = NULL`.
   - **Havia mais elementos**: o novo último ainda aponta para o removido por `prox`, então `li->fim->prox = NULL`.
5. `free(aux)`, `li->qtd--`, `return 1`.

**Comparação com a lista simples:** lá era preciso um `while` com **dois ponteiros andando juntos** (`aux` e `ant`), além de um tratamento separado para 1 elemento. Aqui o código é **menor, sem laço e sem variável `ant`**.

**Complexidade:** **O(1)** (era O(n) na lista simples).

---

### `acessar_inicio()`

```c
int acessar_inicio(Lista li) {
    if (li->qtd == 0)
        return -1;
    return li->inicio->valor;
}
```

**O que faz:** devolve o valor do primeiro nó **sem removê-lo** (só consulta).

**Lógica:**
- Lista vazia ⇒ `-1` (indicador de erro).
- Senão, `li->inicio->valor`: `li->inicio` é o ponteiro para o primeiro nó, e `->valor` lê o dado dele.

**Complexidade:** O(1).

---

### `acessar_final()`

```c
int acessar_final(Lista li) {
    if (li->qtd == 0)
        return -1;
    return li->fim->valor;
}
```

**O que faz:** devolve o valor do último nó **sem removê-lo**.

**Lógica:** idêntica à anterior, mas usando `li->fim`. Na lista simples isso exigia percorrer todos os nós.

**Complexidade:** **O(1)** (era O(n) na lista simples).

---

### `buscar_por_valor()`

```c
int buscar_por_valor(Lista li, int valor) {
    int posicao = 0;
    Elem aux = li->inicio;
    while (aux != NULL) {
        if (aux->valor == valor)
            return posicao;
        aux = aux->prox;
        posicao++;
    }
    return -1;
}
```

**O que faz:** procura um valor e devolve a **posição (índice, começando em 0)** da primeira ocorrência.

**Lógica:**
- `aux` começa no primeiro nó; `posicao` começa em `0`.
- A cada volta do laço, compara `aux->valor` com o valor procurado:
  - **Achou:** retorna `posicao`.
  - **Não achou:** avança (`aux = aux->prox`) e incrementa `posicao`.
- O laço termina quando `aux == NULL` (passou do último nó), e então retorna `-1` ("não encontrado").
- Em lista vazia, `aux` já começa `NULL`, o laço nem executa e retorna `-1` (sem precisar de teste especial).

**Complexidade:** O(n) no pior caso (busca sequencial). Os ponteiros `ant` não ajudam aqui, pois o valor pode estar em qualquer lugar.

---

### `buscar_por_posicao()`

```c
int buscar_por_posicao(Lista li, int posicao) {
    if (posicao < 0 || posicao >= li->qtd)
        return -1;

    Elem aux;
    if (posicao < li->qtd / 2) {
        aux = li->inicio;
        for (int i = 0; i < posicao; i++)
            aux = aux->prox;
    } else {
        aux = li->fim;
        for (int i = li->qtd - 1; i > posicao; i--)
            aux = aux->ant;
    }
    return aux->valor;
}
```

**O que faz:** devolve o valor que está na posição `posicao` (começando em 0).

**Lógica:**
1. **Validação:** a posição deve estar em `0 <= posicao < qtd`. Fora disso, retorna `-1`. Isso também cobre lista vazia (`qtd == 0`).
2. **Escolha do lado mais próximo** (a grande vantagem da lista dupla):
   - Se `posicao < qtd / 2`, está na **primeira metade**: começa em `inicio` e anda `posicao` vezes com `prox`.
   - Senão, está na **segunda metade**: começa em `fim` (índice `qtd - 1`) e anda **para trás** com `ant`, até chegar em `posicao`.
3. Retorna `aux->valor`.

**Exemplo** (lista `[10, 20, 30, 40, 50]`, `qtd = 5`, `qtd/2 = 2`):
- `posicao = 1`: `1 < 2`, começa do início, 1 passo → `20`.
- `posicao = 4`: `4 >= 2`, começa do fim (índice 4) e não anda nenhuma vez → `50`. Na lista simples seriam 4 passos.

**Complexidade:** O(n) no pior caso, mas com no máximo **n/2** passos (na simples, até n−1).

---

### `destruir()`

```c
void destruir(Lista li) {
    if (li == NULL)
        return;
    Elem aux = li->inicio;
    while (aux != NULL) {
        Elem atual = aux;
        aux = aux->prox;
        free(atual);
    }
    free(li);
}
```

**O que faz:** libera **toda** a memória: todos os nós e a struct de controle.

**Lógica:**
- `if (li == NULL) return`: protege contra destruir uma lista que nem foi criada.
- Percorre os nós um por um:
  - `atual = aux`: guarda o nó que vai ser liberado.
  - `aux = aux->prox`: **avança antes de liberar**. Se fizesse `free(aux)` e depois `aux = aux->prox`, estaria lendo memória já liberada (comportamento indefinido).
  - `free(atual)`: libera o nó.
- Por último, `free(li)` libera a struct de controle.

**Por que não precisa mexer em `ant`?** Estamos destruindo tudo, então não importa deixar ponteiros inconsistentes durante o processo. Só precisamos de **um** sentido de caminhada.

> **Atenção depois de chamar:** o `li` do chamador continua guardando o endereço antigo (agora inválido). Não use a lista depois de `destruir`.

**Complexidade:** O(n).

---

### Resumo das complexidades

| Função | Lista simples | Lista dupla |
|---|---|---|
| `criar_lista` | O(1) | O(1) |
| `inserir_inicio` | O(1) | O(1) |
| `inserir_final` | O(n) | **O(1)** |
| `remover_inicio` | O(1) | O(1) |
| `remover_final` | O(n) | **O(1)** |
| `acessar_inicio` | O(1) | O(1) |
| `acessar_final` | O(n) | **O(1)** |
| `buscar_por_valor` | O(n) | O(n) |
| `buscar_por_posicao` | O(n) | O(n), mas até 2× mais rápida |
| `destruir` | O(n) | O(n) |


### Variáveis auxiliares usadas nas funções

| Variável | Tipo | Para que serve |
|---|---|---|
| `no` | `Elem` | O novo nó recém-alocado, nas inserções |
| `aux` | `Elem` | Nó "andarilho" (percorrer a lista) ou nó que será removido |
| `atual` | `Elem` | Em `destruir`, o nó a liberar enquanto `aux` já avançou |
| `posicao` | `int` | Contador de índice em `buscar_por_valor` |
