#define MAX 4
typedef struct fila* Fila;

Fila criar();
int enfileirar(Fila f, int valor);
int desenfileirar(Fila f);
int acessar_inicio(Fila f);
void imprimir_fila(Fila f);
void destruir(Fila f);