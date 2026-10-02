<div align="center">

# 📚 Estrutura de Dados em <img src="https://skillicons.dev/icons?i=c" />

**Minha jornada de aprendizado em Estrutura de Dados com a linguagem C**

[![GitHub](https://img.shields.io/badge/GitHub-Amanda--Aziz-181717?style=flat-square&logo=github)](https://github.com/Amanda-Aziz)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://opensource.org/licenses/MIT)
[![Made with C](https://img.shields.io/badge/Made%20with-C-blue?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))

</div>

---

## 📚 Sobre a Disciplina

**Estrutura de Dados Lineares** aborda os principais conceitos de organização, armazenamento e manipulação de dados na memória. A disciplina utiliza a linguagem **C** para trabalhar conceitos como **ponteiros, memória, structs e Tipos Abstratos de Dados (TADs)**, além da implementação de estruturas como **pilhas, filas e listas encadeadas**.

O objetivo é compreender como diferentes estruturas podem ser utilizadas para armazenar e manipular dados de forma organizada e eficiente, desenvolvendo também o raciocínio sobre memória, operações e gerenciamento dos dados.

## 📖 Sobre o Repositório

Este repositório documenta minha jornada de aprendizado em **Estrutura de Dados Lineares** utilizando a linguagem **C**. Aqui você encontrará implementações, exercícios e anotações sobre os principais conceitos da disciplina.

Todos os códigos foram desenvolvidos durante as aulas e estudos pessoais, servindo como material de consulta e prática. Vários módulos possuem também um arquivo `.md` próprio (`sobre_*.md`) com explicações detalhadas, linha a linha, da lógica por trás do código.

---

## 🗂️ Estrutura do Repositório

```
estrutura-de-dados-lineares/
└── estrutura-de-dados/
    ├── exercicios-resolvidos/
    │   ├── fila/
    │   ├── pilha/
    │   └── tad/
    ├── FILA/
    │   ├── fila.c
    │   ├── fila.h
    │   ├── main.c
    │   └── sobre_fila.md
    ├── LISTA-ENCADEADA/
    │   ├── lista_int.c
    │   ├── lista.h
    │   ├── main.c
    │   └── sobre_lista_encadeada.md
    ├── PILHA/
    │   ├── main.c
    │   ├── pilha.c
    │   ├── pilha.h
    │   └── sobre_pilha.md
    ├── TAD/
    │   ├── 1. Sobre o TAD/
    │   │   ├── arquivo_de_leitura.md
    │   │   ├── sobre_ponto.md
    │   │   └── sobre_quadrado.md
    │   ├── PONTO/
    │   │   ├── main_ponto.c
    │   │   ├── ponto.c
    │   │   └── ponto.h
    │   └── QUADRADO/
    │       ├── main_quadrado.c
    │       ├── main_vetor.c
    │       ├── quadrado.c
    │       └── quadrado.h
    └── revisao.c
```

## 📂 Conteúdo do Repositório

| Pasta | Descrição | Status |
| :--- | :--- | :---: |
| `FILA/` | Implementação de Fila (Queue) circular com vetor estático | ✅ |
| `PILHA/` | Implementação de Pilha (Stack) com vetor estático | ✅ |
| `LISTA-ENCADEADA/` | Implementação de Lista Encadeada de inteiros | ✅ |
| `TAD/PONTO/` | Tipo Abstrato de Dados (TAD) - Ponto no plano cartesiano | ✅ |
| `TAD/QUADRADO/` | Tipo Abstrato de Dados (TAD) - Quadrado, composto por Pontos | ✅ |
| `TAD/1. Sobre o TAD/` | Anotações e explicações conceituais sobre TADs | ✅ |
| `exercicios-resolvidos/` | Exercícios resolvidos de fila, pilha e TAD | ✅ |
| `revisao.c` | Revisão de conceitos base: stack x heap, ponteiros, passagem por valor/referência | ✅ |

> **Status:** ✅ = Concluído | 🚧 = Em andamento | 📝 = Planejado

---

<div align="center">

### 🛠️ Tecnologias Utilizadas

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-00599C?style=for-the-badge&logo=gnu&logoColor=white)
![VS Code](https://img.shields.io/badge/VS%20Code-007ACC?style=for-the-badge&logo=visual-studio-code&logoColor=white)
![Git](https://img.shields.io/badge/Git-F05032?style=for-the-badge&logo=git&logoColor=white)

</div>

---

## ▶️ Como Executar

### Pré-requisitos

- Compilador C (GCC recomendado)
- Git (opcional, para clonar o repositório)

### Passo a passo

```bash
# 1. Clone o repositório
git clone https://github.com/Amanda-Aziz/estrutura-de-dados-lineares.git

# 2. Entre na pasta do projeto
cd estrutura-de-dados-lineares/estrutura-de-dados

# 3. Entre na pasta desejada (exemplo: PILHA)
cd PILHA

# 4. Compile o código
gcc main.c pilha.c -o programa

# 5. Execute
# Linux/Mac:
./programa
# Windows:
programa.exe
```

> 💡 Cada pasta segue esse mesmo padrão: compile o `main.c` junto com o(s) arquivo(s) `.c` do módulo correspondente (ex: `fila.c`, `pilha.c`, `ponto.c` + `quadrado.c`, `lista_int.c`).

---

## 📝 Conceitos Estudados

- 🧠 Stack vs Heap, ponteiros, passagem por valor e por referência
- 🎯 TAD (Tipo Abstrato de Dados) e encapsulamento com structs opacas
- 📦 Pilha (Stack) — LIFO
- 🔄 Fila (Queue) circular — FIFO
- 🔗 Lista Encadeada

---

<div align="center">

Feito por [Amanda-Aziz](https://github.com/Amanda-Aziz) | disciplina de **Estrutura de Dados Lineares**

</div>
