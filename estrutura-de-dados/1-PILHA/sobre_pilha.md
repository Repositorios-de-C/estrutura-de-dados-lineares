# TAD Pilha

## Visão geral

O programa implementa uma **Pilha (Stack)** com **tamanho fixo** (array estático dentro da struct), seguindo a lógica **LIFO** (Last In, First Out — o último que entra é o primeiro que sai). As operações são: `criar`, `empilhar` (push), `desempilhar` (pop), `acessar_topo` (peek) e `destruir`.

---

## Estrutura de dados

```c
struct pilha{
    int dados[MAX];
    int topo;
};
```

| Campo | Tipo | Significado |
|---|---|---|
| `dados` | `int[MAX]` | Array (vetor) fixo que guarda os elementos da pilha. `MAX` é uma constante definida no `.h` (provavelmente `#define MAX 4`, já que o `main` comenta que ela "não comporta mais de 4") |
| `topo` | `int` | Índice de controle: indica **quantos elementos** estão na pilha e, ao mesmo tempo, **a próxima posição livre** de `dados` |

Igual ao `Ponto`, `Pilha` é um `typedef struct pilha* Pilha` (deve estar no `.h`) — struct opaca, TAD com encapsulamento.

---

## Função por função (`pilha.c`)

### `criar()`
```c
Pilha criar(){
    Pilha p = malloc(sizeof(struct pilha));
    if(p != NULL){
        p->topo = 0;
    }
    return p;
}
```
- Aloca a struct inteira no heap — isso inclui o array `dados[MAX]` (já que ele faz parte da struct, não é um ponteiro separado).
- `p->topo = 0` inicializa a pilha como **vazia**: nenhum elemento e a próxima posição livre é o índice `0`.

### `empilhar(Pilha p, int valor)` — push
```c
int empilhar(Pilha p, int valor){
    if(p->topo < MAX){
        p->dados[p->topo] = valor;
        p->topo++;
        return 1;
    }
    return 0;
}
```
- Verifica se ainda há espaço (`topo < MAX`) — isso é o **teste de "pilha cheia"**.
- Se houver espaço: coloca `valor` na posição `dados[topo]` e depois **incrementa** `topo` (por isso o próximo `empilhar` vai para a posição seguinte).
- Retorna `1` (sucesso) ou `0` (pilha cheia, não inseriu).

### `acessar_topo(Pilha p)` — peek ⚠️ BUG
```c
int acessar_topo(Pilha p){
    if(p->topo == 0) // pilha vazia
    return p->dados[p->topo - 1]; // Retorna o último elemento
}
```

**Essa função tem um erro de lógica por falta de chaves `{}`.**

Em C, um `if` sem chaves só "abraça" a **próxima instrução única**. Como não há nenhuma instrução entre o `if` e o `return` (o comentário `// pilha vazia` não conta como código), o compilador entende assim:

```c
int acessar_topo(Pilha p){
    if(p->topo == 0)
        return p->dados[p->topo - 1];   // isso é o "corpo" do if
    // se topo != 0, a função não retorna nada -> comportamento indefinido!
}
```

Ou seja, o código **faz o contrário do que o comentário sugere**:
- **Se a pilha está vazia** (`topo == 0`), ele tenta acessar `dados[-1]` — **acesso fora dos limites do array**, comportamento indefinido.
- **Se a pilha não está vazia** (caso normal), a função **não tem `return`** — também comportamento indefinido, pois ela promete devolver um `int`.

**Correção sugerida:**
```c
int acessar_topo(Pilha p){
    if(p->topo == 0)
        return -1; // ou algum código de erro, pilha vazia
    return p->dados[p->topo - 1]; // retorna o topo normalmente
}
```

No `main.c` fornecido, a pilha nunca fica vazia quando `acessar_topo` é chamada, então o bug não se manifesta nos testes — mas é um erro real que só não "estourou" por sorte das entradas usadas.

> **Lição geral**: sempre use chaves `{}` em `if`/`else`, mesmo com uma linha só — evita esse tipo de erro de "engolir" só a próxima linha.

### `desempilhar(Pilha p)` — pop
```c
int desempilhar(Pilha p){
    if(p->topo == 0)
        return 0;
    p->topo--;
    return 1;
}
```
- Se a pilha está vazia (`topo == 0`), não há o que remover — retorna `0` (falha).
- Caso contrário, apenas **decrementa** `topo`.
- Os dados **não são apagados** de `dados[]` — o valor continua fisicamente lá na memória, só que agora está "fora do alcance lógico" da pilha (porque `topo` não aponta mais para ele). Isso é normal e eficiente: não precisa zerar a posição, só mover o índice de controle.
- Retorna `1` (sucesso).

### `destruir(Pilha p)`
```c
void destruir(Pilha p) {
    if(p != NULL)
        free(p);
}
```
- Libera a memória alocada por `criar()`.
- Como `dados[MAX]` está **dentro** da struct (não é um ponteiro separado alocado à parte), um único `free(p)` já libera tudo — não precisa de um `free` separado para o array.

---

## Simulando o `main.c` passo a passo

Considerando `MAX = 4` (dedução pelo comentário do `main`):

| Ação | Estado de `dados[]` | `topo` | Observação |
|---|---|---|---|
| `criar()` | `[_,_,_,_]` | 0 | pilha vazia |
| `empilhar(50)` | `[50,_,_,_]` | 1 | |
| `empilhar(60)` | `[50,60,_,_]` | 2 | |
| `empilhar(70)` | `[50,60,70,_]` | 3 | |
| `empilhar(20)` | `[50,60,70,20]` | 4 | pilha cheia |
| `acessar_topo()` | — | 4 | retorna `dados[3] = 20` → imprime **20** |
| `desempilhar()` | `[50,60,70,20]` (logicamente `[50,60,70]`) | 3 | remove 20 |
| `desempilhar()` | logicamente `[50,60]` | 2 | remove 70 |
| `empilhar(80)` | `[50,60,80,20]` | 3 | 80 sobrescreve a posição 2 (o "20" antigo era lixo lógico) |
| `empilhar(100)` | `[50,60,80,100]` | 4 | pilha cheia de novo |
| `empilhar(200)` | **sem alteração** | 4 | `topo < MAX` é falso → retorna `0`, 200 não entra |
| `acessar_topo()` | — | 4 | retorna `dados[3] = 100` → imprime **100** |

**Saída esperada do programa:**
```
1º TOPO DA PILHA: 20
2º TOPO DA PILHA: 100
```

---

## Variáveis do `main`

| Variável | Tipo | Para que serve |
|---|---|---|
| `p` | `Pilha` (ponteiro) | Representa a pilha alocada dinamicamente |

---