# TAD Ponto

## Visão geral

O programa representa um **ponto no plano cartesiano (x, y)** como um TAD. Ele permite criar pontos, ler e alterar suas coordenadas, calcular a distância entre dois pontos e destruí-los (liberar memória) — tudo isso sem que quem usa o TAD (`main_ponto.c`) saiba como o `Ponto` é representado por dentro.

---

## A grande sacada: `ponto.h` vs `ponto.c`

Repara uma coisa importante nessa arquitetura:

```c
// ponto.h
typedef struct ponto* Ponto;
```

```c
// ponto.c
struct ponto{
    float x;
    float y;
};
typedef struct ponto* Ponto;
```

- O `.h` (que o `main` inclui) declara que `Ponto` é um **ponteiro para uma struct `ponto`**, mas **não diz quais campos essa struct tem**.
- Só o `.c` sabe que `ponto` tem `x` e `y`.
- Isso é chamado de **struct opaca** (*opaque struct/pointer*) — é o encapsulamento "na unha" em C.
- **Consequência prática**: dentro de `main_ponto.c` você **não pode** escrever `p1.x` ou `p1->x` diretamente — o compilador nem sabe que esse campo existe ali. Você é **obrigado** a usar as funções `criar()`, `acessar()`, `alterar()`. Isso é o TAD "forçando" o encapsulamento.

---

## Estrutura de dados

```c
struct ponto{
    float x;
    float y;
};
typedef struct ponto* Ponto;
```

| Campo | Tipo | Significado |
|---|---|---|
| `x` | `float` | Coordenada horizontal do ponto |
| `y` | `float` | Coordenada vertical do ponto |

- `Ponto` **não é** a struct em si, é um **ponteiro** para ela (`struct ponto*`).
- Ou seja, toda variável do tipo `Ponto` (como `p1`, `p2`) é, na prática, um **endereço de memória** apontando para onde os dados `x` e `y` estão alocados (no heap).

---

## Função por função (`ponto.c`)

### `criar(float x, float y)`
```c
Ponto criar(float x, float y){
    Ponto p = malloc(sizeof(struct ponto));
    if(p != NULL){
        p->x = x;
        p->y = y;
    }
    return p;
}
```
- Aloca dinamicamente (no **heap**, com `malloc`) espaço suficiente para guardar uma `struct ponto`.
- `sizeof(struct ponto)` calcula o tamanho necessário (aqui, 2 floats).
- `if(p != NULL)` é uma **verificação de segurança**: `malloc` pode falhar (memória insuficiente) e retornar `NULL`. Só preenche `x` e `y` se a alocação deu certo.
- `p->x = x` usa o operador seta (`->`) porque `p` é um ponteiro — equivale a `(*p).x = x`.
- Retorna o ponteiro `p`, que passa a ser "o ponto" para quem chamou.

### `acessar(Ponto p, char valor)`
```c
float acessar(Ponto p, char valor){
    if(valor == 'x')
        return p->x;
    if(valor == 'y')
        return p->y;
    return -1;    
}
```
- Como o `main` não tem acesso direto aos campos de `p`, essa função é o **"getter"** do TAD.
- Recebe um `char` (`'x'` ou `'y'`) dizendo qual coordenada você quer ler.
- Retorna `-1` se o caractere não for reconhecido (tratamento de erro simples).

### `alterar(Ponto p, float dado, char valor)`
```c
int alterar(Ponto p, float dado, char valor){
    if(valor == 'x'){
        p->x = dado;
        return 1;
    }
    if(valor == 'y'){
        p->y = dado;
        return 1;
    }
    return -1;
}
```
- É o **"setter"** do TAD: modifica `x` ou `y` conforme o `valor` passado.
- Retorna `1` em caso de sucesso, `-1` se `valor` não for `'x'` nem `'y'` — um jeito simples de indicar sucesso/erro (poderia ser um `enum` ou `bool`, mas aqui optaram por `int`).
- Repare: como `p` é um **ponteiro**, alterar `p->x` dentro da função altera o dado real (mesmo princípio da passagem por referência).

### `destruir(Ponto p)`
```c
void destruir(Ponto p){
    if(p != NULL)
        free(p);
}
```
- Libera a memória alocada por `criar()`.
- O `if(p != NULL)` evita chamar `free` num ponteiro nulo (embora `free(NULL)` seja seguro em C, é uma boa prática defensiva).
- **Todo `malloc` precisa de um `free` correspondente** — aqui é exatamente esse par que fecha o ciclo de vida do `Ponto`.

### `distancia(Ponto p1, Ponto p2)`
```c
float distancia(Ponto p1, Ponto p2){
    float dx = acessar(p1, 'x') - acessar(p2, 'x');
    float dy = acessar(p1, 'y') - acessar(p2, 'y');
    float h = sqrt(dx*dx + dy*dy);
    return h;
}
```
- Calcula a **distância euclidiana** entre dois pontos, usando a fórmula:

  d = √((x1 − x2)² + (y1 − y2)²)

- Repare que essa função **não acessa `p1->x` diretamente** — ela usa `acessar()`, respeitando o encapsulamento mesmo dentro do próprio módulo. Isso é uma boa prática (embora, tecnicamente, dentro do `.c` ela *poderia* acessar direto, já que a struct está visível ali).
- `dx` e `dy` = diferenças de coordenadas.
- `sqrt()` vem de `<math.h>` — raiz quadrada.
- Variável `h` guarda a hipotenusa (distância), que é retornada.

---

## Análise do `main_ponto.c`

```c
Ponto p1 = criar(2, 2);
Ponto p2 = criar(5, 17);
```
- `p1` e `p2` são ponteiros para pontos alocados no **heap**, com coordenadas (2,2) e (5,17).

```c
float d = distancia(p1, p2);
printf("A distancia entre os pontos é: %.2f\n", d);
```
- Calcula e imprime a distância entre os dois pontos, com 2 casas decimais (`%.2f`).

```c
destruir(p1);
destruir(p2);
```
- Libera a memória dos dois pontos — sem isso, teríamos **memory leak**.

---

## Variáveis (resumo)

| Variável | Onde | Tipo | Para que serve |
|---|---|---|---|
| `p1`, `p2` | `main` | `Ponto` (ponteiro) | Representam dois pontos alocados dinamicamente |
| `d` | `main` | `float` | Guarda a distância calculada entre p1 e p2 |
| `x`, `y` | `struct ponto` | `float` | Coordenadas do ponto (invisíveis fora de `ponto.c`) |
| `dx`, `dy`, `h` | `distancia()` | `float` | Diferenças de coordenadas e resultado da raiz quadrada |

---

## Pontos de atenção

1. **Struct opaca**: o `.h` esconde os campos da struct — só o `.c` sabe deles. Esse é o mecanismo de encapsulamento de TAD em C.
2. **`Ponto` é um ponteiro**: sempre que você vir `Ponto p1`, lembre que `p1` guarda um endereço, não os dados diretamente.
3. **Getter/Setter genéricos** (`acessar`/`alterar` com `char`) são uma forma (não a única) de simular acesso controlado aos campos — outra abordagem comum seria ter uma função por campo (`get_x`, `get_y`, `set_x`, `set_y`).
4. **Verificação de `malloc`**: sempre bom conferir se `malloc` não retornou `NULL` antes de usar o ponteiro.
5. **Todo `criar()` tem um `destruir()` correspondente** — ciclo de vida completo do TAD (alocar → usar → liberar).