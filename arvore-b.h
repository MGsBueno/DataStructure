
const int d = 2;

typedef struct No {
    int m; //quantidade de chaves armazenadas no nó
    struct No *pont_pai;
    char *s; //array de chaves
    struct No **p; //ponteiro para array de ponteiros para os filhos
} TNo;

TNo *cria();
TNo *libera(TNo *a);
void imprime(TNo *a, int nivel);
TNo *busca(TNo *no, char ch);
TNo *inicializa();
TNo *particiona(TNo *raiz, TNo *P, char ch, TNo *pt);
TNo *insere(TNo *raiz, int folha, char ch, TNo *pt);
TNo *particiona(TNo *raiz, TNo *P, char ch, TNo *pt);



