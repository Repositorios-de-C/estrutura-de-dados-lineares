# TAD Fila (Circular)

## Visão geral

O programa implementa uma **Fila (Queue)** com tamanho fixo, seguindo a lógica **FIFO** (First In, First Out — o primeiro que entra é o primeiro que sai). A particularidade aqui é que ela é **circular**: usa aritmética de módulo (`%`) para "dar a volta" no array quando chega ao final, em vez de desperdiçar espaço.

---

## Estrutura de dados

```c
struct fila{
    int dados[MAX];
    int inicio;
    int final;
    int qtd;
};
typedef struct fila* Fila;
```

| Campo | Tipo | Significado |
|---|---|---|
| `dados` | `int[MAX]` | Array fixo (`MAX = 4`) que guarda os elementos da fila |
| `inicio` | `int` | Índice do **primeiro** elemento da fila (o próximo a sair) |
| `final` | `int` | Índice da **próxima posição livre** (onde o próximo elemento vai entrar) |
| `qtd` | `int` | Quantidade atual de elementos na fila |

### Por que 3 variáveis de controle (a pilha só precisava de uma)?

Na pilha, só se mexe numa ponta (o topo). Na fila, mexe-se em **duas pontas diferentes**: insere no final e remove do início. Isso já exige `inicio` e `final`. O `qtd` entra como variável extra para resolver uma ambiguidade clássica das filas circulares: quando `inicio == final`, isso pode significar tanto "fila vazia" quanto "fila cheia" — dependendo de como os índices andaram. Guardar `qtd` separadamente resolve essa ambiguidade de forma simples e direta.

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
- Aloca a struct no heap.
- Inicializa `inicio = 0`, `final = 0` e `qtd = 0` → fila vazia, ambos os índices apontando para a posição 0.

### `enfileirar(Fila f, int valor)` — insere no final
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
- `if(f->qtd < MAX)`: verifica se ainda há espaço — usa `qtd` (não compara `inicio` com `final` diretamente), evitando a ambiguidade mencionada acima.
- `f->dados[f->final] = valor`: coloca o novo valor na posição indicada por `final`.
- `f->final = (f->final + 1) % MAX`: **parte "circular"**. Em vez de só `final++`, usa `%` (resto da divisão) para "voltar ao início" do array quando `final` chega em `MAX`.
  - Exemplo com `MAX = 4`: se `final = 3` e soma 1, dá 4. Mas `4 % 4 = 0`. Em vez de `final` virar 4 (posição inválida), ele "dá a volta" e vira 0 de novo.
- `f->qtd++`: incrementa a contagem de elementos.
- Retorna `1` (sucesso) ou `0` (fila cheia).

### `desenfileirar(Fila f)` — remove do início
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
- Verifica se a fila não está vazia (`qtd > 0`).
- `f->inicio = (f->inicio + 1) % MAX`: avança o índice de início, também de forma circular.
- O valor **não é apagado fisicamente** de `dados[]`. Só o índice `inicio` "anda para frente", fazendo aquele valor sair do alcance lógico da fila.
- `f->qtd--`: decrementa a contagem.
- Retorna `1` (sucesso) ou `0` (fila vazia, nada a remover).

### `acessar_inicio(Fila f)` — peek (consulta sem remover)
```c
int acessar_inicio(Fila f){
    if(f->qtd > 0){
        return f->dados[f->inicio];
    }
    return -1;
}
```
- Lógica **correta** (diferente do `acessar_topo` da pilha, que tinha bug por falta de chaves): chaves bem colocadas, `if` cobrindo o `return` certo.
- Se a fila tem elementos, retorna o valor na posição `inicio` (o próximo a sair).
- Se está vazia, retorna `-1` como indicador de fila vazia.

### `destruir(Fila f)`
```c
void destruir(Fila f){
    if(f != NULL) free(f);                      
}
```
- Libera a memória alocada por `criar()`.

### `imprimir_fila(Fila f)`
```c
void imprimir_fila(Fila f){
    for(int i = 0; i < MAX; i++){
        printf("[%d]", f->dados[i]);
    }
    printf("\n");
}
```
- Percorre **todo o array `dados`** (do índice 0 até `MAX-1`), independente de `inicio`, `final` ou `qtd`.
- **Atenção**: imprime **todas as posições do array**, incluindo posições que podem conter "lixo lógico" (valores antigos já desenfileirados, mas ainda fisicamente presentes). Não é uma "view" fiel da fila em ordem lógica (do `inicio` até o `final`) — imprime o array cru.
- Não é chamada no `main` fornecido — está definida, mas não usada nesse trecho.

---

## Análise do `main()`

```c
Fila f = criar();

enfileirar(f, 20);
enfileirar(f, 40);
enfileirar(f, 60);
enfileirar(f, 30);
```
- Enfileira 4 valores. Como `MAX = 4`, a fila fica completamente cheia.

```c
printf("\nInicio da fila: %d", acessar_inicio(f));
```
- Imprime o início da fila, o primeiro elemento inserido (FIFO): `20`.

```c
desenfileirar(f);
printf("\nInicio da fila: %d\n", acessar_inicio(f));
```
- Remove o `20` (o primeiro que entrou).
- Novo início é o `40` (segundo elemento inserido) — imprime `40`.

```c
if(enfileirar(f, 50)){
    printf("Enfileirou\n");
} else {
    printf("Nao enfileirou\n");
}
```
- Como acabamos de desenfileirar, a fila tem espaço de novo (`qtd = 3 < MAX = 4`).
- `enfileirar(f, 50)` retorna `1` (sucesso) → imprime **"Enfileirou"**.
- A lógica circular entra em ação: antes desse `enfileirar`, `final` estava em `0` (avançou `0→1→2→3→0` após inserir 20, 40, 60, 30). O `50` é escrito em `dados[0]`, **sobrescrevendo** a posição onde estava o `20` antigo (já removido logicamente).

---

## Simulando passo a passo (com `MAX = 4`)

| Ação | `dados[]` (índices 0,1,2,3) | `inicio` | `final` | `qtd` |
|---|---|---|---|---|
| `criar()` | `[_,_,_,_]` | 0 | 0 | 0 |
| `enfileirar(20)` | `[20,_,_,_]` | 0 | 1 | 1 |
| `enfileirar(40)` | `[20,40,_,_]` | 0 | 2 | 2 |
| `enfileirar(60)` | `[20,40,60,_]` | 0 | 3 | 3 |
| `enfileirar(30)` | `[20,40,60,30]` | 0 | 0 (deu a volta: `(3+1)%4=0`) | 4 (cheia) |
| `acessar_inicio()` | — | 0 | — | — | retorna `dados[0] = 20` |
| `desenfileirar()` | (lógico: `40,60,30`) | 1 | 0 | 3 |
| `acessar_inicio()` | — | 1 | — | — | retorna `dados[1] = 40` |
| `enfileirar(50)` | `[50,40,60,30]` (50 sobrescreve posição 0) | 1 | 1 (`(0+1)%4=1`) | 4 (cheia de novo) |

**Saída esperada do programa:**
```

Inicio da fila: 20
Inicio da fila: 40
Enfileirou
```

---

## Variáveis do `main`

| Variável | Tipo | Para que serve |
|---|---|---|
| `f` | `Fila` (ponteiro) | Representa a fila alocada dinamicamente |

---