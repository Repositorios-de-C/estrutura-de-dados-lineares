#include"pilha.h"
#include<stdio.h>

int main(){
    Pilha p = criar();
    empilhar(p, 50);
    empilhar(p, 60);
    empilhar(p, 70);
    empilhar(p, 20);

    printf("\n1º TOPO DA PILHA: %d\n", acessar_topo(p));
    
    desempilhar(p);
    desempilhar(p);
    
    empilhar(p, 80);
    empilhar(p, 100);
    empilhar(p, 200); //A pilha nao comporta mais de 4, entao ele não entra
    
    printf("2º TOPO DA PILHA: %d\n", acessar_topo(p));
    destruir(p);
}