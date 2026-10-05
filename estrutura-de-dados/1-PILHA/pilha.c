#include <stdlib.h>
#include "pilha.h"

struct pilha{
    int dados[MAX];
    int topo;
};

Pilha criar(){
    Pilha p = malloc(sizeof(struct pilha)); // alocando memória para a pilha
    if(p != NULL){ //Verifica se a alocação deu certo (se não for NULL inica topo com 0)
        p->topo = 0; //O operador 'seta' acessa o campo {topo} da estrutura apontada por p
    }
    return p;
}


int empilhar(Pilha p, int valor){
    if(p->topo < MAX){ //se topo for menor que MAX, ainda há espaço na pilha
        p->dados[p->topo] = valor; //adiciona o valor no topo da pilha
        p->topo++; //topo é incrementado para apontar para a próxima posição livre
        return 1;
    }
    return 0;
} 


int acessar_topo(Pilha p){
    if(p->topo == 0){ // pilha vazia
        return -1; // pilha inválida  
    } //topo-1 é usado porque o topo aponta para a próxima posição livre, então o último elemento está em topo-1
    return p->dados[p->topo - 1]; //retorna o valor do topo da pilha
}


int desempilhar(Pilha p){
    if(p->topo == 0){ // pilha vazia
        return 0;
    }
    p->topo--; //diminui o topo (remove o último)
    return 1;
} 


void destruir(Pilha p) { //aqui liberamos a memoria
    if(p != NULL){ // verifica se a pilha não é nula antes de liberar a memória!
        free(p); //libera a memória alocada para a pilha
    }
}