#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Sala {
    char nome[50];
    char pista[50]; // Pista opcional
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct NodoBST {
    char pista[50];
    struct NodoBST *esq;
    struct NodoBST *dir;
} NodoBST;

// -------- Funções da BST --------
NodoBST* criarNodoBST(const char *pista) {
    NodoBST *novo = (NodoBST*) malloc(sizeof(NodoBST));
    strcpy(novo->pista, pista);
    novo->esq = novo->dir = NULL;
    return novo;
}

NodoBST* inserir(NodoBST *raiz, const char *pista) {
    if (raiz == NULL)
        return criarNodoBST(pista);

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserir(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserir(raiz->dir, pista);

    return raiz;
}

void emOrdem(NodoBST *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf("🔹 %s\n", raiz->pista);
    emOrdem(raiz->dir);
}

// -------- Funções da Mansão --------
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

Sala* montarMansao() {
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *biblioteca = criarSala("Biblioteca", "Página rasgada do diário");
    Sala *salaJantar = criarSala("Sala de Jantar", "");
    Sala *escritorio = criarSala("Escritório", "Chave dourada");
    Sala *cozinha = criarSala("Cozinha", "Luvas sujas de sangue");

    hall->esquerda = biblioteca;
    hall->direita = salaJantar;

    biblioteca->esquerda = escritorio;
    biblioteca->direita = NULL;

    salaJantar->esquerda = NULL;
    salaJantar->direita = cozinha;

    return hall;
}

void explorarSalas(Sala *atual, NodoBST **pistas) {
    char escolha;

    while (atual != NULL) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        // Se tiver pista, adiciona à BST
        if (strcmp(atual->pista, "") != 0) {
            printf("🕵️ Você encontrou uma pista: %s\n", atual->pista);
            *pistas = inserir(*pistas, atual->pista);
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🚧 Sala sem saídas. Caminho encerra aqui.\n");
            break;
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda)
            printf("  (e) → Ir para a esquerda (%s)\n", atual->esquerda->nome);
        if (atual->direita)
            printf("  (d) → Ir para a direita (%s)\n", atual->direita->nome);
        printf("  (p) → Ver pistas coletadas\n");
        printf("  (s) → Sair da exploração\n");

        printf("\nDigite sua escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL)
            atual = atual->esquerda;
        else if (escolha == 'd' && atual->direita != NULL)
            atual = atual->direita;
        else if (escolha == 'p') {
            printf("\n📜 Pistas coletadas:\n");
            emOrdem(*pistas);
        }
        else if (escolha == 's') {
            printf("🔚 Investigação pausada.\n");
            break;
        }
        else {
            printf("❌ Caminho inválido!\n");
        }
    }
}

int main() {
    Sala *mansao = montarMansao();
    NodoBST *pistas = NULL;

    printf("🔎 Detective Quest - Nível Aventureiro 🛡️\n");
    explorarSalas(mansao, &pistas);

    printf("\n📦 Relatório Final de Pistas:\n");
    emOrdem(pistas);

    printf("\n🧠 Fim da exploração!\n");
    return 0;
}
