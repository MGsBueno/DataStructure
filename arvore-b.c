#include "arvore-b.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


//alterei as chaves de int para char no cria()
TNo *cria() {
    TNo *novo = (TNo *) malloc(sizeof(TNo));
    novo->m = 0;
    novo->pont_pai = NULL;
    novo->s = (char *) malloc(sizeof(char *) * (d * 2)); //chaves
    novo->p = (TNo **) malloc(sizeof(TNo *) * (d * 2) + 1); //filhos
    for (int i = 0; i < (d * 2 + 1); i++) {
        novo->p[i] = NULL;
    }
    return novo;
}

TNo *libera(TNo *a) {
    if (a != NULL) {
        for (int i = 0; i <= d * 2 + 1; i++) {
            libera(a->p[i]);
        }
        free(a->s);
        free(a->p);
        free(a);
    }
    return NULL;
}

void imprime(TNo *a, int nivel) {
    if (a != NULL) {
        if (nivel >= 0)
            printf("Nível %d:\n", nivel);
        for (int i = 0; i < a->m; i++) {
            printf("%c", a->s[i]);
        }
        printf("\n");
        //Imprime filhos recursivamente, se a não for folha
        if (a->p[0] != NULL) {
            for (int i = 0; i <= a->m; i++) {
                imprime(a->p[i], nivel + 1);
            }
        }
    }
}

//alterei as chaves de int para char no busca
TNo *busca(TNo *no, char ch, int frequencia) {
    if (no != NULL) {
        int i = 0;
        while (i < no->m && ch > no->s[i]) {
            i++;
        }
        if (i < no->m && ch == no->s[i]) {
            return no;
        } else if (no->p[i] != NULL) {
            return busca(no->p[i], ch, frequencia);
        } else return no; //nó era folha -- não existem mais nós a buscar, então retorna o nó onde a chave deveria estar
    } else return NULL; //nó é NULL, não há como buscar
}

TNo *inicializa() {
    return NULL;
}

/*
 * Insere chave ch e ponteiro pt (inicialmente pt é NULL, pois inserção é na folha)
 * raiz é ponteiro para a raiz da árvore
 * Ponteiro pt será útil no particionamento, quando inserção for ser efetuada em nó não folha
 * folha indica se a inserção é para ser feita na folha (se for inserção por propagação de particionamento, folha vem como false e raiz aponta para local onde chave deve ser inserida
 * Retorna ponteiro para a raiz da árvore
 */

TNo *insere(TNo *raiz, int folha, char ch, TNo *pt, int frequencia ) {
    TNo *no;
    if (folha) {
        no = busca(raiz, ch, frequencia);
        if (no != NULL) {
            //chave buscada pode existir dentro do nó retornado
            //verificar se realmente existe
            for (int i = 0; i < no->m; i++) {
                if (no->s[i] == ch) {
                    //chave já existe, portanto não pode ser inserida.
                    //retorna raiz original sem alterar a árvore
                    return raiz;
                }
            }
        }
    } else {
        no = raiz;
    }

    if (raiz == NULL) {
        raiz = cria();
        raiz->s[0] = ch;
        raiz->m = 1;
        //atualiza raiz da arvore
        return raiz;
    } else {
        if (no->m == (2 * d)) {
            //Nó está cheio -- é necessário particionar
            //raiz da árvore pode ter mudado, então captura possível nova raiz
            raiz = particiona(raiz, no, ch, pt, frequencia);
        } else {
            //encontra posição de inserção
            int i = no->m;
            while (i >= 0 && ch < no->s[i - 1]) {
                no->s[i] = no->s[i - 1];
                no->p[i + 1] = no->p[i];
                i--;
            }
            if (i == -1) {
                i = 0;
            }
            no->s[i] = ch;
            no->p[i + 1] = pt;
            no->m++;
        }
    }
    return raiz;
}




/*
 * Particiona o nó p para adicionar a chave ch e ponteiro pt
 * Após o final da execução dessa função, a chave ch terá sido inserida no local correto
 * e um novo nó q terá sido criado como resultado do particionamento
 */

TNo *particiona(TNo *raiz, TNo *P, char ch, TNo *pt, int frequencia) {
    TNo *Q = cria();
    Q->m = d; //número de chaves na nova página Q é d
    Q->pont_pai = P->pont_pai;

    //Verifica a posição onde ch deve ser inserida
    int pos = -1;
    for (int i = 0; i < P->m; i++) {
        if (ch > P->s[i]) {
            pos = i + 1;
        }
    }

    if (pos == -1) {
        //chave a ser inserida é a menor de todas e deve ser inserida na primeira posição
        pos = 0;
    }

    //salva chave da posicao d para subir para o pai (note que d equivale a d+1 já que vetor começa em ZERO
    int chave_d;
    TNo *pt_chave_d = Q; //ponteiro que sobe é sempre ponteiro para o novo nó Q
    if (pos < d) {
        //chave vai ser inserida em posição menor que d, então tem que compensar diminuindo 1 no índice
        chave_d = P->s[d - 1];
    } else if (pos != d) {
        chave_d = P->s[d];
    } else {
        //chave que vai subir é a própria chave ch, e ela vai apontar para o novo nó Q
        chave_d = ch;
    }

    //Ajusta chaves em Q, copiando de P as últimas d chaves
    int i = d * 2 - 1;
    int j = d - 1;

    while (j >= 0) {
        if (j != pos - d - 1) {
            Q->s[j] = P->s[i];
            Q->p[j + 1] = P->p[i + 1];
            i--;
        } else {
            //insere ch em Q na posição correta
            Q->s[j] = ch;
            Q->p[j + 1] = pt;
        }
        j--;
    }
    //Ajusta ponteiro p[0] de Q, que tem que ser o ponteiro d de P
    Q->p[0] = P->p[d];

    //Ajusta chaves em P fazendo shift se necessário
    j = d;
    while (i > pos) {
        P->s[i] = P->s[i - 1];
        P->p[i + 1] = P->p[i];
        i--;
    }

    // Trata caso onde ch tem que ser inserida em P
    if (pos <= d) {
        //insere ch em P na posição pos
        P->s[pos] = ch;
        P->p[pos + 1] = pt;
    }

    //Ajusta quantidades de chaves em P
    P->m = d;

    //Insere chave d+1 no pai de P e ponteiro apontando para a nova página Q

    TNo *pai = insere(P->pont_pai, 0, chave_d, pt_chave_d, frequencia);
    //Se P->pai era NULL, significa que árvore tem uma nova raiz, então é preciso ajustar os ponteiros para o pai de P e Q para apontar para esse novo nó
    if (P->pont_pai == NULL) {
        P->pont_pai = pai;
        Q->pont_pai = pai;
        //Arruma ponteiros da nova raiz
        pai->p[0] = P;
        pai->p[1] = Q;
    }
    raiz = pai;
    return raiz;
}

int main(int argc, char *argv[]) {
    TNo *raiz = inicializa();
    int num = 0, escolha = 1 ;
    char letra = 'z';
    while (num != -1) {
        printf("Digite: \n 1 inserir em Arvore ordenada em ordem alfabetica\n"
               " 2 inserir em Arvore ordenada por frequencia\n"
               "-8 para busca por ordem alfabetica\n"
               "-9 para busca por frequencia\n"
               " 0 para imprimir\n"
               " e -1 para sair\n");
        scanf("%i", &num);

        //if de saída
        if (num == -1) {
            printf("\n");
            imprime(raiz, 0);
            libera(raiz);
            return 0;

            //impressao
        } else if (num == 0) {
            printf("\n");
            imprime(raiz, 0);

            //busca ordem alfabética
        } else if (num == -8) {
            printf("Digite a chave a ser buscada: ");
            fflush(stdin);
            scanf("%c", &letra);
            TNo *n = busca(raiz, letra, 0);
            printf("Resultado da busca (lembrando que busca retorna o nó onde a chave deveria estar): \n");
            imprime(n, -1);

            //busca or frequencia
        } else if (num == -9) {
            printf("Digite a chave a ser buscada: ");
            fflush(stdin);
            scanf("%c", &letra);
            TNo *n = busca(raiz, letra, 1);
            printf("Resultado da busca (lembrando que busca retorna o nó onde a chave deveria estar): \n");
            imprime(n, -1);

            //insercao
        } else if (num == 1) {
            while (escolha != -1) {
                printf("digite 1 para inserir letra, ou -1 para voltar ao menu anterior: \n");
                fflush(stdin);
                scanf("%d", &escolha);

                if (escolha == 1) {
                    printf("Digite a letra a adicionar a arvore: \n");

                    fflush(stdin);
                    letra = getchar();
                    if (!isdigit(letra))
                    raiz = insere(raiz, 1, letra, NULL, 0);
                    else printf(" caracter invalido tente novamente\n");
                    printf("Resultado da inserçao: \n");
                    imprime(raiz, 0);
                }
            }
        }else if(num == 2){
            while (escolha != -1) {
                printf("digite 1 para inserir letra, ou -1 para voltar ao menu anterior: \n");
                fflush(stdin);
                scanf("%d", &escolha);

                if (escolha == 1 ) {
                    printf("Digite a letra a adicionar a arvore: \n");

                    fflush(stdin);
                    letra = getchar();
                    if (!isdigit(letra))
                    raiz = insere(raiz, 1, letra, NULL, 1);
                    else printf(" caracter invalido tente novamente\n");
                    printf("Resultado da inserçao: \n");
                    imprime(raiz, 0);
                }
            }

        }else{
            printf("\n\n");
        }
    }
/* Cria arvore de 3 níveis
    raiz = insere(raiz, 1, 10, NULL);
    raiz = insere(raiz, 1, 20, NULL);
    raiz = insere(raiz, 1, 30, NULL);
    raiz = insere(raiz, 1, 40, NULL);
    raiz = insere(raiz, 1, 50, NULL);
    raiz = insere(raiz, 1, 15, NULL);
    raiz = insere(raiz, 1, 45, NULL);
    raiz = insere(raiz, 1, 25, NULL);
    raiz = insere(raiz, 1, 22, NULL);
    raiz = insere(raiz, 1, 42, NULL);
    raiz = insere(raiz, 1, 43, NULL);
    raiz = insere(raiz, 1, 5, NULL);
    raiz = insere(raiz, 1, 8, NULL);
    raiz = insere(raiz, 1, 7, NULL);
    raiz = insere(raiz, 1, 12, NULL);
    raiz = insere(raiz, 1, 18, NULL);
    raiz = insere(raiz, 1, 11, NULL);
    printf("Resultado final da inserção: \n");
    imprime(raiz, 0);
*/
}

