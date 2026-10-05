typedef struct elem* Elem;
typedef struct lista* Lista;

Lista criar_lista(void);

int inserir_inicio(Lista li, int valor_inserir);
int inserir_final(Lista li, int valor_inserir);

int remover_inicio(Lista li);
int remover_final(Lista li);

int acessar_inicio(Lista li);
int acessar_final(Lista li);

int buscar_por_valor(Lista li, int valor);
int buscar_por_posicao(Lista li, int posicao);

void destruir(Lista li);