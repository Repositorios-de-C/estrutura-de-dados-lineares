#include "lista.h"
#include <stdio.h>

int main() {
    int valor;

    printf("Valor: ");
    scanf("%d", &valor);

    Lista li = criar_lista();

    if (li == NULL) {
        printf("Erro ao criar a lista.\n");
        return 1;
    }

    inserir_inicio(li, valor);

    printf("Primeiro elemento: %d\n", acessar_inicio(li));

    destruir(li);
    return 0;
}