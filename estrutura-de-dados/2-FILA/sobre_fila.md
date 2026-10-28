# Fila Circular com Array (`fila.c`)

## Visão geral

Este README explica **apenas o arquivo `fila.c`**, que implementa uma **Fila** (estrutura **FIFO**: *First In, First Out*, o primeiro a entrar é o primeiro a sair) de inteiros usando um **array de tamanho fixo** com comportamento **circular**.

Funções implementadas: `criar`, `enfileirar`, `desenfileirar`, `acessar_inicio`, `imprimir_fila` e `destruir`.

### A ideia central

- Os elementos entram pelo **final** da fila e saem pelo **início**.
- Os dados ficam num array de `MAX` posições (aqui, `MAX = 4`). A fila **não cresce**: se encher, `enfileirar` falha.
- Para não desperdiçar posições conforme os elementos saem, o array é tratado como um **círculo**: depois da última posição (`MAX - 1`), o índice volta para `0`. Isso é feito com o operador módulo (`%`).

```
Índices do array:   [0] [1] [2] [3]
                     ↑               o índice depois do [3] volta para o [0]
                     └───────────────┘
```

---

## Estrutura de dados

```c
struct fila{
    int dados[MAX];
    int inicio;
    int final;
    int qtd;
};
```

| Campo | Tipo | Significado |
|---|---|---|
| `dados` | `int[MAX]` | O array que guarda os elementos da fila |
| `inicio` | `int` | Índice do **primeiro** elemento da fila (o próximo a sair) |
| `final` | `int` | Índice da **próxima posição livre** (onde o próximo elemento vai entrar) |
| `qtd` | `int` | Quantidade de elementos atualmente na fila |

> ⚠️ **Atenção ao nome `final`:** ele **não** aponta para o último elemento, e sim para a posição **logo depois** do último, ou seja, o lugar onde o próximo `enfileirar` vai escrever.

### Por que existe o campo `qtd`?

Se só tivéssemos `inicio` e `final`, ficaria impossível distinguir **fila vazia** de **fila cheia**, porque nos dois casos `inicio == final`. O `qtd` resolve a ambiguidade:

| Situação | `inicio == final`? | `qtd` |
|---|---|---|
| Fila vazia | sim | `0` |
| Fila cheia | sim | `MAX` |

### Relações que sempre devem valer (invariantes)

1. `0 <= qtd <= MAX`.
2. `inicio` e `final` estão sempre entre `0` e `MAX - 1`.
3. Os elementos da fila ocupam as posições `inicio`, `inicio+1`, ... (voltando a 0 ao passar de `MAX-1`), num total de `qtd` posições.
4. `final == (inicio + qtd) % MAX`.

---

## Includes e `#define`

```c
#include<stdlib.h>
#include<stdio.h>
#include "fila.h"
#define MAX 4
```

- `<stdlib.h>`: fornece `malloc` e `free`.
- `<stdio.h>`: fornece `printf` (usado em `imprimir_fila`).
- `"fila.h"`: traz as assinaturas das funções e o `typedef struct fila* Fila`.
- `#define MAX 4`: define a capacidade da fila.

> **Observação:** `MAX` também é definido em `fila.h`. Como os dois valores são idênticos (`4`), o compilador aceita sem reclamar, mas a definição fica **duplicada**. Se um dia você mudar o valor em um arquivo e esquecer do outro, aparecerá um aviso de redefinição de macro (e valeria o valor do último `#define` lido). O mais limpo é manter só uma, em `fila.h`, e remover a do `.c`.

### Sobre o `typedef struct fila* Fila`

O tipo `Fila` é um **ponteiro** para `struct fila`. Como a `struct` é **definida só no `.c`**, quem usa a fila (como o `main`) não consegue acessar `f->dados` ou `f->inicio` diretamente. Só pode usar as funções. Isso é o **encapsulamento** (TAD opaco).

---

## Função por função

### `criar()`

```c
Fila criar(){
    Fila f = malloc(sizeof(struct fila));
    if(f != NULL){
        f->inicio = 0;
        f->final = 0;
        f->qtd = 0;
    }
    return f;
}
```

**O que faz:** cria uma fila vazia.

**Lógica:**
- `malloc(sizeof(struct fila))` aloca no heap **uma única struct** que já contém o array `dados` dentro dela (por isso um bloco só basta).
- `if(f != NULL)`: `malloc` devolve `NULL` se faltar memória. Sem o teste, escrever em `f->inicio` derrubaria o programa.
- `inicio = 0`, `final = 0` e `qtd = 0` representam a fila vazia: o primeiro elemento a entrar vai para a posição `0`.
- Retorna `f`, que pode ser `NULL` em caso de falha, então **quem chama deve conferir**.

**Detalhe:** o `malloc` **não zera** a memória, então as posições de `dados` começam com lixo. Não é problema para o funcionamento da fila (só lemos posições que já foram escritas), mas afeta `imprimir_fila` (veja abaixo).

**Sugestão:** declarar como `Fila criar(void)`. Em C, parênteses vazios significam "argumentos não especificados", enquanto `void` diz explicitamente "nenhum argumento".

**Complexidade:** O(1).

---

### `enfileirar()`

```c
int enfileirar(Fila f, int valor){
    if(f->qtd < MAX){
        f->dados[f->final] = valor;
        f->final = (f->final + 1) % MAX;
        f->qtd++;
        return 1;
    }
    return 0;
}
```

**O que faz:** insere `valor` no **final** da fila.

**Retorno:** `1` se conseguiu inserir, `0` se a fila estava cheia.

**Lógica passo a passo:**
1. `if(f->qtd < MAX)`: só insere se ainda houver espaço. Se `qtd == MAX`, a fila está cheia e a função retorna `0` sem alterar nada.
2. `f->dados[f->final] = valor`: grava o valor na próxima posição livre.
3. `f->final = (f->final + 1) % MAX`: avança o `final` para a posição seguinte, **de forma circular**. O `% MAX` é o que faz o índice voltar a `0` quando passa do fim do array:
   - `final = 0` → `(0+1) % 4 = 1`
   - `final = 2` → `(2+1) % 4 = 3`
   - `final = 3` → `(3+1) % 4 = 0` ← volta ao começo
4. `f->qtd++`: atualiza a contagem.
5. `return 1`.

**Complexidade:** O(1).

---

### `desenfileirar()`

```c
int desenfileirar(Fila f){
    if(f->qtd > 0){
        f->inicio = (f->inicio + 1) % MAX;
        f->qtd--;
        return 1;
    }
    return 0;
}
```

**O que faz:** remove o elemento do **início** da fila (o mais antigo).

**Retorno:** `1` se removeu, `0` se a fila estava vazia. **Não devolve o valor removido**; para ler o valor, use `acessar_inicio` **antes** de desenfileirar.

**Lógica passo a passo:**
1. `if(f->qtd > 0)`: só remove se houver algum elemento.
2. `f->inicio = (f->inicio + 1) % MAX`: o início avança uma posição (circularmente). O elemento que estava ali passa a ser considerado "fora da fila".
3. `f->qtd--`: atualiza a contagem.
4. `return 1`.

**Detalhe importante:** a remoção é **lógica**, e o dado **não é apagado** do array. Ele continua fisicamente em `dados[...]` até ser sobrescrito por um futuro `enfileirar`. Isso é perfeitamente correto, pois só `inicio` e `qtd` definem o que "faz parte" da fila. É também mais rápido, já que nada precisa ser deslocado.

**Complexidade:** O(1).

---

### `acessar_inicio()`

```c
int acessar_inicio(Fila f){
    if(f->qtd > 0){
        return f->dados[f->inicio];
    }
    return 0;
}
```

**O que faz:** devolve o valor do primeiro elemento **sem removê-lo** (só consulta).

**Lógica:**
- Se a fila tem elementos, retorna `f->dados[f->inicio]`.
- Se está vazia, retorna `0` (indicador de erro).

---

### `imprimir_fila()`

```c
void imprimir_fila(Fila f){
    for(int i = 0; i < MAX; i++){
        printf("[%d]", f->dados[i]);
    }
    printf("\n");
}
```

**O que faz:** imprime o conteúdo do array `dados`, posição por posição, de `0` até `MAX - 1`.

**Lógica:** um `for` percorre todos os `MAX` índices e imprime cada valor entre colchetes, seguido de uma quebra de linha.

> ⚠️ **Esta função mostra o array físico, não a fila lógica.** Isso gera dois efeitos:
> 1. **Posições vazias aparecem com lixo.** Como o `malloc` não zera a memória, posições nunca escritas mostram valores aleatórios.
> 2. **Elementos já removidos continuam aparecendo**, pois `desenfileirar` não apaga o dado. E a ordem impressa é a das posições do array, não a ordem de saída da fila.
>
> **Exemplo:** depois de enfileirar 20, 40, 60, 30, desenfileirar e enfileirar 50, o array fica `[50][40][60][30]`, mas a fila (do início ao fim) é `40 → 60 → 30 → 50`.

**Versão que imprime só a fila lógica** (do início ao fim, `qtd` elementos):

```c
void imprimir_fila(Fila f){
    for(int i = 0; i < f->qtd; i++){
        int pos = (f->inicio + i) % MAX;
        printf("[%d]", f->dados[pos]);
    }
    printf("\n");
}
```

A posição real de cada elemento é `(inicio + i) % MAX`, e o `% MAX` cuida do caso em que a fila "dá a volta" no array.

**Complexidade:** O(MAX) na versão original, O(qtd) na versão sugerida.

---

### `destruir()`

```c
void destruir(Fila f){
    if(f != NULL){
        free(f);
    }
}
```

**O que faz:** libera a memória da fila.

**Lógica:**
- Como o array `dados` está **dentro** da struct, um único `malloc` foi feito em `criar`, então **um único `free`** libera tudo. (Numa lista encadeada seria preciso liberar cada nó.)
- O teste `f != NULL` protege contra destruir uma fila que não foi criada. Na prática, `free(NULL)` já é seguro em C (não faz nada), então o `if` é opcional, mas deixa a intenção explícita.

> **Depois de chamar `destruir`:** o ponteiro `f` do chamador continua guardando o endereço antigo (agora inválido). Não use a fila depois disso.

**Complexidade:** O(1).

---

## Exemplo: acompanhando o comportamento circular

Sequência de operações com `MAX = 4`, mostrando o estado interno (`?` = posição nunca escrita, com lixo):

| Operação | `dados[0..3]` | `inicio` | `final` | `qtd` | Retorno |
|---|---|---|---|---|---|
| `criar()` | `? ? ? ?` | 0 | 0 | 0 | |
| `enfileirar(20)` | `20 ? ? ?` | 0 | 1 | 1 | 1 |
| `enfileirar(40)` | `20 40 ? ?` | 0 | 2 | 2 | 1 |
| `enfileirar(60)` | `20 40 60 ?` | 0 | 3 | 3 | 1 |
| `enfileirar(30)` | `20 40 60 30` | 0 | **0** | 4 | 1 |
| `enfileirar(99)` | `20 40 60 30` | 0 | 0 | 4 | **0** (cheia) |
| `desenfileirar()` | `20 40 60 30` | 1 | 0 | 3 | 1 |
| `enfileirar(50)` | `50 40 60 30` | 1 | 1 | 4 | 1 |

Observe:
- No 4º `enfileirar`, o `final` **dá a volta** para `0` graças ao `% MAX`.
- Com a fila cheia, `inicio == final` (ambos `0`); quem diz que está cheia (e não vazia) é o `qtd == 4`.
- Depois do `desenfileirar`, o `20` continua em `dados[0]`, mas está fora da fila. O `enfileirar(50)` **reaproveita** essa posição, sobrescrevendo o `20`. É a vantagem da fila circular: o espaço liberado no início volta a ser usado.
- A fila lógica no final é `40 → 60 → 30 → 50`, mesmo o array físico sendo `[50][40][60][30]`.

---

## Resumo das complexidades

| Função | Complexidade | Retorno |
|---|---|---|
| `criar` | O(1) | ponteiro para a fila, ou `NULL` se falhar |
| `enfileirar` | O(1) | `1` = inseriu, `0` = fila cheia |
| `desenfileirar` | O(1) | `1` = removeu, `0` = fila vazia |
| `acessar_inicio` | O(1) | valor do início, ou `-1` se vazia |
| `imprimir_fila` | O(MAX) | nada |
| `destruir` | O(1) | nada |

## Fila circular (array) × Lista encadeada

| | Fila com array circular | Fila com lista encadeada |
|---|---|---|
| Capacidade | fixa (`MAX`) | dinâmica |
| Memória | um bloco só | um `malloc` por nó |
| Enfileirar / desenfileirar | O(1) | O(1) (com ponteiro para o fim) |
| Pode ficar cheia? | sim | só se faltar memória |
| `destruir` | um `free` | laço liberando nó a nó |