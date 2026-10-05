#include<stdlib.h>
#include<stdio.h>
#define MAX 4

struct fila{
    int dados[MAX]; //dados é um array de int com tamanho MAX, que armazena os [elementos da fila]
    int inicio;
    int final;
    int qtd;
};
typedef struct fila* Fila;

Fila criar(){
    Fila f = malloc(sizeof(struct fila));
    if(f != NULL){
        f->inicio = 0;
        f->final = 0;
        f->qtd = 0; // inicializa a QUANTIDADE de elementos na fila como 0
    }
    return f;
}


int enfileirar(Fila f, int valor){
    if(f->qtd < MAX){
        f->dados[f->final] = valor; //aqui adicionamos o valor no final da fila
        f->final = (f->final + 1) % MAX; //aqui usamos o operador módulo para que, quando final chegar a MAX, ele volte para 0 (circular)
        f->qtd++;
        return 1;
    }
    return 0;
}

int desenfileirar(Fila f){
    if(f->qtd > 0){
        f->inicio = (f->inicio + 1) % MAX;
        f->qtd--;
        return 1;
    }
    return 0;       
}

int acessar_inicio(Fila f){
    if(f->qtd > 0){
        return f->dados[f->inicio];
    }
    return -1;
}


void imprimir_fila(Fila f){
    for(int i = 0; i < MAX; i++){
        printf("[%d]", f->dados[i]);
    }
    printf("\n");
}


void destruir(Fila f){
    if(f != NULL){
        free(f);
    }                      
}