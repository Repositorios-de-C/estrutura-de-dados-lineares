# Lista Encadeada (de inteiros)

## Visão geral

O programa implementa uma **Lista Encadeada Simples** de inteiros. Diferente da Pilha e da Fila (que usavam um array fixo dentro da struct), aqui **cada elemento é um nó alocado individualmente no heap**, e os nós são conectados entre si por ponteiros (`prox`). Isso permite que a lista **cresça dinamicamente**, sem um limite `MAX` fixo como nos TADs anteriores.

As operações disponíveis são: `criar_lista`, `inserir_inicio`, `inserir_final`, `remover_inicio`, `remover_final`, `acessar_inicio`, `acessar_final`, `buscar_por_valor`, `buscar_por_posicao` e `destruir`.

---

## Estrutura de dados

```c
struct elem {
    int valor;
    struct elem* prox;
};

struct lista {
    int qtd;
    struct elem* inicio;
};

typedef struct elem* Elem;
typedef struct lista* Lista;
```

| Campo | Onde | Tipo | Significado |
|---|---|---|---|
| `valor` | `struct elem` | `int` | O dado guardado naquele nó da lista |
| `prox` | `struct elem` | `struct elem*` | Ponteiro para o **próximo nó** da lista (ou `NULL` se for o último) |
| `qtd` | `struct lista` | `int` | Quantidade de elementos atualmente na lista |
| `inicio` | `struct lista` | `struct elem*` | Ponteiro para o **primeiro nó** da lista (ou `NULL` se a lista estiver vazia) |

Repare na diferença de conceito em relação à Pilha/Fila:
- Lá, existia **uma struct só**, com um array fixo dentro.
- Aqui, existem **duas structs**: `elem` (o nó individual) e `lista` (o "controlador", que só guarda o tamanho e o ponteiro para o primeiro nó).
- A lista, como um todo, é uma **cadeia de nós** ligados por `prox`, e não um bloco contíguo de memória — cada nó pode estar em um endereço de memória completamente diferente.

Assim como nos TADs anteriores, `Elem` e `Lista` são **structs opacas** (`typedef struct elem*`/`typedef struct lista*`) — quem usa o `.h` não enxerga os campos internos, só manipula através das funções.

---

## Função por função (`lista_int.c`)

### `criar_lista()`
```c
Lista criar_lista() {
    Lista li = malloc(sizeof(struct lista));

    if (li != NULL) {
        li->qtd = 0;
        li->inicio = NULL;
    }
    return li;
}
```
- Aloca no heap **apenas a struct de controle** (`lista`), não os nós — os nós só vão existir quando você inserir algo.
- `qtd = 0` e `inicio = NULL` representam uma lista vazia: nenhum elemento e nenhum "primeiro nó" para apontar.

### `inserir_inicio(Lista li, int valor_inserir)`
```c
int inserir_inicio(Lista li, int valor_inserir) {
    Elem no = malloc(sizeof(struct elem));

    if (no != NULL) {
        no->valor = valor_inserir;
        no->prox = li->inicio;
        li->inicio = no;
        li->qtd++;
        return 1;
    }
    return 0;
}
```
- Cria um **novo nó** (`no`) no heap.
- `no->valor = valor_inserir`: guarda o dado recebido.
- `no->prox = li->inicio`: o novo nó passa a apontar para quem **era** o primeiro nó da lista (antes da inserção).
- `li->inicio = no`: agora o novo nó **é** o primeiro da lista.
- Essa ordem (ligar o novo nó ao antigo início **antes** de atualizar `inicio`) é essencial — se fosse trocada, você perderia a referência ao resto da lista.
- `li->qtd++`: atualiza a contagem.
- Complexidade: **O(1)** — não importa o tamanho da lista, sempre é rápido, pois só mexe no início.

### `inserir_final(Lista li, int valor_inserir)`
```c
int inserir_final(Lista li, int valor_inserir) {
    Elem no = malloc(sizeof(struct elem));
    if (no != NULL) {
        no->valor = valor_inserir;
        no->prox = NULL;
        if (li->inicio == NULL) {
            li->inicio = no;
            li->qtd++;
            return 1;
        }

        Elem aux = li->inicio;

        while (aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = no;
        li->qtd++;
        return 1;
    }
    return 0;
}
```
- Cria o novo nó, com `prox = NULL` (já que ele será o último).
- **Caso especial**: se a lista está vazia (`li->inicio == NULL`), o novo nó passa a ser diretamente o `inicio`.
- **Caso geral**: usa uma variável auxiliar `aux` para **percorrer a lista** a partir do `inicio`, andando nó por nó (`aux = aux->prox`) até achar o **último nó** (aquele cujo `prox` é `NULL`).
- Uma vez encontrado o último nó, `aux->prox = no` conecta o novo nó ao final da cadeia.
- Complexidade: **O(n)** — diferente da inserção no início, aqui é necessário percorrer a lista inteira até o fim.

### `remover_inicio(Lista li)`
```c
int remover_inicio(Lista li) {
    if (li->qtd == 0)
        return 0;

    Elem aux = li->inicio;
    li->inicio = aux->prox;
    free(aux);
    li->qtd--;
    return 1;
}
```
- Se a lista está vazia (`qtd == 0`), não há o que remover — retorna `0`.
- Guarda o nó atual do início em `aux` (para não perder a referência antes de liberá-lo).
- `li->inicio = aux->prox`: o segundo nó passa a ser o novo início.
- `free(aux)`: libera a memória do nó removido.
- Complexidade: **O(1)**.

### `acessar_inicio(Lista li)`
```c
int acessar_inicio(Lista li) {
    if (li->qtd == 0)
        return -1;
    return li->inicio->valor;
}
```
- Se vazia, retorna `-1` (indicador de erro).
- Caso contrário, retorna o valor guardado no primeiro nó — repare no encadeamento `li->inicio->valor`: primeiro acessa o campo `inicio` da lista (que é um ponteiro para `elem`), depois acessa o campo `valor` desse nó.

### `remover_final(Lista li)`
```c
int remover_final(Lista li) {
    if (li->qtd == 0)
        return 0;
    Elem aux = li->inicio;

    // Caso exista apenas um elemento
    if (aux->prox == NULL) {
        free(aux);
        li->inicio = NULL;
        li->qtd--;
        return 1;
    }
    Elem ant;
    while (aux->prox != NULL) {
        ant = aux;
        aux = aux->prox;
    }
    free(aux);
    ant->prox = NULL;
    li->qtd--;
    return 1;
}
```
- Se vazia, retorna `0`.
- **Caso especial (só 1 elemento)**: se o próprio `inicio` já é o último nó (`aux->prox == NULL`), basta liberá-lo e zerar `li->inicio`.
- **Caso geral**: aqui é preciso manter **dois ponteiros andando juntos**: `ant` (o penúltimo nó) e `aux` (o último nó). Isso é necessário porque, numa lista **simplesmente encadeada**, não dá para "andar para trás" a partir do último nó — não existe um ponteiro `anterior`. Por isso, ao encontrar o último nó, você precisa já ter guardado uma referência a quem vem antes dele.
- Ao final do laço, `aux` é o último nó e `ant` é o penúltimo.
- `free(aux)` libera o último nó, e `ant->prox = NULL` faz o penúltimo nó (agora o novo último) apontar para `NULL`.
- Complexidade: **O(n)** — precisa percorrer a lista inteira.

> ⚠️ **Ponto de atenção sobre `ant`**: a variável `ant` é declarada (`Elem ant;`) mas **não inicializada**. Isso só não causa problema aqui porque o `if (aux->prox == NULL)` acima já trata o caso de 1 elemento separadamente — ou seja, se o laço `while` executa, é garantido que ele roda **pelo menos uma vez** antes de `aux` chegar ao fim, então `ant` sempre recebe um valor válido antes de ser usado. Ainda assim, é uma boa prática inicializar variáveis ao declará-las, para evitar esse tipo de raciocínio "funciona, mas por pouco".

### `acessar_final(Lista li)`
```c
int acessar_final(Lista li) {
    if (li->qtd == 0)
        return -1;
    Elem aux = li->inicio;
    while (aux->prox != NULL) {
        aux = aux->prox;
    }
    return aux->valor;
}
```
- Se vazia, retorna `-1`.
- Caso contrário, percorre a lista até o último nó (mesma lógica de "andar" vista em `inserir_final`) e retorna o valor dele.
- Complexidade: **O(n)** — é o preço de não ter um ponteiro direto para o "fim" da lista (diferente de `inicio`, que é guardado explicitamente na struct `lista`).

### `buscar_por_valor(Lista li, int valor)`
```c
int buscar_por_valor(Lista li, int valor) {
    Elem aux = li->inicio;
    int posicao = 0;

    while (aux != NULL) {
        if (aux->valor == valor) {
            return posicao;
        }
        aux = aux->prox;
        posicao++;
    }

    return -1;
}
```
- Percorre a lista do início ao fim, comparando `aux->valor` com o `valor` procurado.
- `posicao` é um contador que acompanha o índice do nó atual (começando em `0`).
- Se encontrar, retorna a **posição** (índice) onde o valor está.
- Se chegar ao fim (`aux == NULL`) sem encontrar, retorna `-1`.
- Complexidade: **O(n)** no pior caso (busca sequencial, sem atalhos).

### `buscar_por_posicao(Lista li, int posicao)`
```c
int buscar_por_posicao(Lista li, int posicao) {
    if (posicao < 0 || posicao >= li->qtd)
        return -1;

    Elem aux = li->inicio;

    for (int i = 0; i < posicao; i++) {
        aux = aux->prox;
    }

    return aux->valor;
}
```
- Primeiro valida se a posição pedida é válida (`0 <= posicao < qtd`) — evita andar além do fim da lista.
- Anda `posicao` vezes a partir do início, usando `aux = aux->prox`.
- Retorna o valor do nó encontrado nessa posição.
- Complexidade: **O(n)** — diferente de um array, onde acessar por posição é O(1), numa lista encadeada é sempre necessário "andar" nó por nó até chegar lá.

### `destruir(Lista li)`
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
- Verifica se `li` não é `NULL` (evita erro ao tentar destruir uma lista que nem foi criada).
- Percorre **todos os nós** da lista, liberando um por um:
  - `atual = aux` guarda o nó a ser liberado.
  - `aux = aux->prox` avanha para o próximo nó **antes** de liberar `atual` — essa ordem é crucial: se você chamasse `free(aux)` e só depois tentasse `aux = aux->prox`, estaria acessando memória já liberada (comportamento indefinido).
  - `free(atual)` libera o nó atual.
- Por fim, `free(li)` libera a struct de controle da lista.
- Diferente da Pilha/Fila (onde um único `free` bastava, pois tudo estava numa struct só), aqui é necessário **um laço de liberação**, porque cada nó foi alocado separadamente com seu próprio `malloc`.

---

## Simulando o `main.c` passo a passo

```c
int valor;
printf("Valor: ");
scanf("%d", &valor);          // usuário digita, por ex., 5

Lista li = criar_lista();     // li->qtd = 0, li->inicio = NULL

inserir_inicio(li, valor);    // cria nó {valor: 5, prox: NULL}
                               // li->inicio aponta para esse nó
                               // li->qtd = 1

printf("Primeiro elemento: %d\n", acessar_inicio(li));
                               // li->qtd != 0 -> retorna li->inicio->valor = 5

destruir(li);                 // libera o nó e a struct lista
```

**Saída esperada (para entrada `5`):**
```
Valor: 5
Primeiro elemento: 5
```

| Passo | `li->inicio` | `li->qtd` | Observação |
|---|---|---|---|
| `criar_lista()` | `NULL` | 0 | lista vazia |
| `inserir_inicio(li, 5)` | nó `{5, NULL}` | 1 | único nó, também é o início |
| `acessar_inicio(li)` | — | 1 | retorna `5` |
| `destruir(li)` | — | — | nó liberado, depois a struct `lista` liberada |

---

## Variáveis do `main`

| Variável | Tipo | Para que serve |
|---|---|---|
| `valor` | `int` | Guarda o número digitado pelo usuário |
| `li` | `Lista` (ponteiro) | Representa a lista encadeada alocada dinamicamente |

## Variáveis auxiliares mais usadas dentro das funções

| Variável | Tipo | Para que serve |
|---|---|---|
| `no` | `Elem` | O novo nó recém-alocado, a ser inserido na lista |
| `aux` | `Elem` | Ponteiro "andarilho", usado para percorrer a lista nó a nó |
| `ant` | `Elem` | Guarda o nó **anterior** ao `aux`, necessário em `remover_final` (lista simplesmente encadeada não anda "para trás") |
| `atual` | `Elem` | Em `destruir`, guarda o nó a ser liberado antes de avançar `aux` |
| `posicao` | `int` | Contador de índice, usado em `buscar_por_valor` |
