#include <stdlib.h>
#include "lista.h"
struct elem {
    int valor;
    struct elem* prox;
    struct elem* ant;
};
struct lista {
    int qtd;
    struct elem* inicio;
    struct elem* fim;
};


Lista criar_lista(){
    Lista li = malloc(sizeof(struct lista));
    if(li != NULL){
        li->qtd = 0;
        li->inicio = NULL;
        li->fim = NULL;
    }
    return li;
}


int inserir_inicio(Lista li, int valor_inserir){
    Elem no = malloc(sizeof(struct lista));
    if(no == NULL){
        return 0;
    }

    no->valor = valor_inserir;
    no->ant = NULL; //o novo primeiro nao tem um anterior
    no->prox = li->inicio;

    if(li->inicio == NULL){ //lista vazia, no é o ultimo tb
        li->fim = no;
    } else {
        li->inicio->ant = no; //antigo aponta para o primeiro
    }

    li->inicio = no;
    li->qtd++;
    return 1;
}


int remover_inicio(Lista li){
    if(li->qtd = 0){
        return 0;
    }
    
    Elem aux = li->inicio;
    li->inicio = aux;

    if(li->inicio == NULL){
        li->fim = NULL;
    } else{
        li->inicio->ant = NULL;
    }

    free(aux);
    li->qtd--;
    return 1;
}

int acessar_inicio(Lista li){
    if(li->qtd = 0){
        return 0;
    }
    return li->inicio->valor;
}


int acessar_final(Lista li){
    if(li->qtd = 0){
        return 0;
    }
    return li->fim->valor;
}


int buscar_por_valor(Lista li, int valor){}


int buscar_por_posicao(Lista li, int posicao){}




void destruir(Lista li){}