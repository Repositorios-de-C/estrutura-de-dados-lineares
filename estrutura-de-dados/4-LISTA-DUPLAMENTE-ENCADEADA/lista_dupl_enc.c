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


int buscar_por_valor(Lista li, int valor){
    int posicao = 0;
    Elem aux= li->inicio;
    while(aux != NULL){
        if(aux->valor == valor){
            return posicao;
        }
        aux = aux->prox;
        posicao++;
    }
    return -1;
}


int buscar_por_posicao(Lista li, int posicao){
    if(posicao < 0 || posicao >= li->qtd){
        return -1;
    }

    Elem aux;
    if(posicao < li->qtd / 2){
        aux = li->inicio;
        for(int i=0; i<posicao; i++){
            aux = aux->prox;
        }
    } else {
        aux = li->fim;
        for(int i = li->qtd - 1; i > posicao; i--){
            aux = aux->ant;
        }
    }
    return aux->valor;
}


void destruir(Lista li){
    if(li == NULL){
        return;
    }

    Elem aux = li->inicio;
    
    while(aux != NULL){
        Elem atual = aux;
        aux = aux->prox;
        free(atual);
    }
    
    free(li);
}