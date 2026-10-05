#include"fila.h"
#include<stdio.h>

int main(){
    Fila f = criar();

    enfileirar(f, 20);
    enfileirar(f, 40);
    enfileirar(f, 60);
    enfileirar(f, 30);

    printf("\nInicio da fila: %d", acessar_inicio(f));

    desenfileirar(f);
    printf("\nInicio da fila: %d\n", acessar_inicio(f));

    if(enfileirar(f, 50)){
        printf("Enfileirou\n");
    } else {
        printf("Nao enfileirou\n");
    }

    return 0;
}