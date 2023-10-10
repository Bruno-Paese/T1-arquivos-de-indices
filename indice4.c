#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct a
{
    char value[256];
    int address;
    struct a *esq, *dir, *pai;
    // Valor que cresce conforme se aproxima da raiz (Funciona quase de forma inversa ao nível da árvore sendo um em todas as folhas)
    int altura;
};

typedef struct a Avl;

// Retorna a altura ou zero caso o nodo esteja nulo
int getAltura(Avl *node)
{
    if (node == NULL)
        return 0;
    return node->altura;
}

// Calcula o fator de balanceamento de um nodo
int getFatorBalanceamento(Avl *node)
{
    if (node == NULL)
        return 0;
    return getAltura(node->dir) - getAltura(node->esq);
}

// Rotação para a esquerda
void rpe(Avl **nodo, Avl **letra)
{
    Avl *no = *nodo;
    Avl *dir = no->dir;

    // Rotaciona
    no->dir = dir->esq;
    if (dir->esq != NULL)
    {
        // Atualiza pai do antigo nodo da esquerda da nova raiz para a raiz antiga
        dir->esq->pai = no;
    }
    dir->esq = (*nodo);

    // Atualiza os pais
    dir->pai = no->pai;
    no->pai = dir;

    // Atualiza alturas
    no->altura = 1 + (getAltura(no->dir) > getAltura(no->esq) ? getAltura(no->dir) : getAltura(no->esq));
    dir->altura = 1 + (no->altura > getAltura(dir->dir) ? no->altura : getAltura(dir->dir));

    // Atualiza a referência do vô
    if (dir->pai != NULL)
    {
        if (strcmp(dir->pai->value, dir->value) < 0)
        {
            dir->pai->dir = dir;
        }
        else
        {
            dir->pai->esq = dir;
        }
    }
    else
    {
        // Atualiza o vetor de letras
        *letra = dir;
    }
}

// Rotação para a direita
void rpd(Avl **nodo, Avl **letra)
{
    Avl *no = *nodo;
    Avl *esq = no->esq;

    // Rotaciona
    no->esq = esq->dir;
    if (esq->dir != NULL)
    {
        // Atualiza pai do antigo nodo da direita da nova raiz para a raiz antiga
        esq->dir->pai = no;
    }
    esq->dir = no;

    // Atualiza os pais
    esq->pai = no->pai;
    no->pai = esq;

    // Atualiza alturas
    no->altura = 1 + (getAltura(no->esq) > getAltura(no->dir) ? getAltura(no->esq) : getAltura(no->dir));
    esq->altura = 1 + (no->altura > getAltura(esq->esq) ? no->altura : getAltura(esq->esq));

    // Atualiza a referência do vô
    if (esq->pai != NULL)
    {
        if (strcmp(esq->pai->value, esq->value) < 0)
        {
            esq->pai->dir = esq;
        }
        else
        {
            esq->pai->esq = esq;
        }
    }
    else
    {
        // Atualiza o vetor de letras
        *letra = esq;
    }
}

// Determina a rotação necessária para balancear a árvore
void rebalancearArvore(Avl **pai, Avl **letra)
{
    int fbPai = getFatorBalanceamento(*pai);
    int fbFilho;

    if (fbPai == 2)
    {
        // Braço da direita mais pesado: Rotação para a esquerda

        fbFilho = getFatorBalanceamento((*pai)->dir);
        if (fbFilho < 0)
        {
            // Sinais opostos: Rotação dupla
            rpd(&(*pai)->dir, letra);
            rpe(&(*pai)->pai, letra);
        }
        else
        {
            // Sinais iguais: Rotação simples
            rpe(pai, letra);
        }
    }
    else
    {
        // Braço da esquerda mais pesado: Rotação para a direita

        fbFilho = getFatorBalanceamento((*pai)->esq);
        if (fbFilho > 0)
        {
            // Sinais opostos: Rotação dupla
            rpe(&(*pai)->esq, letra);
            rpd(&(*pai)->pai, letra);
        }
        else
        {
            // Sinais iguais: Rotação simples
            rpd(pai, letra);
        }
    }
}

// Calcula altura dos pais do nodo recursivamente
void calcularAlturaPai(Avl **pai, Avl **letra)
{
    // Atualiza altura do nodo utilizando a altura do filho mais alto
    if (getAltura((*pai)->esq) > getAltura((*pai)->dir))
    {
        // Filho mais alto na esquerda
        (*pai)->altura = getAltura((*pai)->esq) + 1;
    }
    else
    {
        // Filho mais alto na direita
        (*pai)->altura = getAltura((*pai)->dir) + 1;
    }

    // Caso encontre um Fator de Balanceamento igual a 2 ou -2, rebalanceia

    int fb = getFatorBalanceamento(*pai);
    if (fb == 2 || fb == -2)
        rebalancearArvore(pai, letra);

    // Chama recursivamente
    if ((*pai) != NULL && (*pai)->pai != NULL)
        calcularAlturaPai(&((*pai)->pai), letra);
}

void recalcularAltura(Avl **nodo, Avl **letra)
{
    if (*nodo != NULL && (*nodo)->pai != NULL)
        calcularAlturaPai(&((*nodo)->pai), letra);
}

// Cria e insere o nodo na árvore
void inserirNodo(Avl **pai, char *val, int address, Avl **letra)
{
    Avl *novo = (Avl *)malloc(sizeof(Avl));
    strcpy(novo->value, val);
    novo->address = address;
    novo->altura = 1;
    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = *pai;
    if (*pai != NULL && strcmp(val, (*pai)->value) < 0)
    {
        (*pai)->esq = novo;
    }
    else if (*pai != NULL)
    {
        (*pai)->dir = novo;
    }

    recalcularAltura(&novo, letra);
}

// Busca o local para inserir o nodo e insere na árvore com a função de inserirNodo
void inserirValorNaArvore(char *val, int address, Avl **raiz, Avl **indice)
{

    // Primeira inserção na árvore
    if (*raiz == NULL)
    {
        *raiz = (Avl *)malloc(sizeof(Avl));
        strcpy((*raiz)->value, val);
        (*raiz)->address = address;
        (*raiz)->altura = 1;
        (*raiz)->esq = NULL;
        (*raiz)->dir = NULL;
        (*raiz)->pai = NULL;
    }
    else
    {
        // Segue busca para a esquerda
        if (strcmp(val, (*raiz)->value) < 0)
        {
            if ((*raiz)->esq == NULL)
            {
                inserirNodo(raiz, val, address, indice);
            }
            else
            {
                inserirValorNaArvore(val, address, &((*raiz)->esq), indice);
            }
            // Segue busca para a direita
        }
        else if (strcmp(val, (*raiz)->value) > 0)
        {
            if ((*raiz)->dir == NULL)
            {
                inserirNodo(raiz, val, address, indice);
            }
            else
            {
                inserirValorNaArvore(val, address, &((*raiz)->dir), indice);
            }
        }
    }
}

Avl *gerarIndice4(char path[])
{
    Avl *indice4 = NULL;
    App temp;
    FILE *bin;
    bin = fopen(path, "rb");
    int i = 0;

    if (!bin)
    {
        printf("Arquivo não encontrados\n");
        return NULL;
    }

    while (!feof(bin))
    {
        fread(&temp, sizeof(App), 1, bin);
        inserirValorNaArvore(temp.name, sizeof(App) * i, &indice4, &indice4);
        i++;
    }

    return indice4;
}

Avl *buscarPorNomeAppRecursivo(char *name, Avl *avl)
{
    if (avl != NULL)
    {
        if (strcmp(avl->value, name) == 0)
        {
            return avl;
        }

        else if (strcmp(name, avl->value) < 0)
        {
            buscarPorNomeAppRecursivo(name, avl->esq);
        }
        else
        {
            buscarPorNomeAppRecursivo(name, avl->dir);
        }
    }
    else
    {
        return NULL;
    }
}

App *buscarPorNomeApp(char *name, char *path, Avl *avl)
{
    FILE *bin = fopen(path, "rb");
    App *temp = (App *)malloc(sizeof(App));
    Avl *avlTemp = buscarPorNomeAppRecursivo(name, avl);
    if (avlTemp == NULL)
    {
        return NULL;
    }
    else
    {
        fseek(bin, avlTemp->address, SEEK_SET);
        fread(temp, sizeof(App), 1, bin);

        return temp;
    }
}

Avl *executeIndex4()
{
    return gerarIndice4("file.dat");
}
