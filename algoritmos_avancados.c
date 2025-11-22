#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// --------------------- Árvore de Salas (Mapa) ----------------------
typedef struct Sala {
    char nome[50];
    char pista[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// ------------------ BST de Pistas Encontradas ---------------------
typedef struct NodoBST {
    char pista[50];
    struct NodoBST *esq;
    struct NodoBST *dir;
} NodoBST;

// ------------------- Tabela Hash de Suspeitos ---------------------
typedef struct Assoc {
    char pista[50];
    char suspeito[50];
    struct Assoc *prox;
} Assoc;

Assoc *tabelaHash[TAM_HASH];

// ------------------- Funções da BST --------------------
NodoBST* criarNodoBST(const char *pista) {
    NodoBST *novo = (NodoBST*) malloc(sizeof(NodoBST));
    strcpy(novo->pista, pista);
    novo->esq = novo->dir = NULL;
    return novo;
}

NodoBST* inserirBST(NodoBST *raiz, const char *pista) {
    if (!raiz) return criarNodoBST(pista);
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirBST(raiz->dir, pista);
    return raiz;
}

// ------------------- Funções da Tabela Hash ----------------------
int hashFunction(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

void inserirHash(const char *pista, const char *suspeito) {
    int index = hashFunction(pista);
    Assoc *novo = (Assoc*) malloc(sizeof(Assoc));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[index];
    tabelaHash[index] = novo;
}

const char* buscarSuspeito(const char *pista) {
    int index = hashFunction(pista);
    Assoc *atual = tabelaHash[index];
    while (atual) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return "Desconhecido";
}

// Contagem de suspeitos para achar culpado
typedef struct {
    char nome[50];
    int qtd;
} ContadorSuspeito;

ContadorSuspeito listaSuspeitos[10];
int totalSuspeitos = 0;

void contabilizarSuspeito(const char *suspeito) {
    for (int i = 0; i < totalSuspeitos; i++) {
        if (strcmp(listaSuspeitos[i].nome, suspeito) == 0) {
            listaSuspeitos[i].qtd++;
            return;
        }
    }
    strcpy(listaSuspeitos[totalSuspeitos].nome, suspeito);
    listaSuspeitos[totalSuspeitos].qtd = 1;
    totalSuspeitos++;
}

// ---------------- Mansão e Exploração ----------------
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
    salaJantar->direita = cozinha;

    return hall;
}

void mostrarPistas(NodoBST *raiz) {
    if (!raiz) return;
    mostrarPistas(raiz->esq);
    const char *suspeito = buscarSuspeito(raiz->pista);
    printf("🔹 Pista: %-30s → Suspeito: %s\n", raiz->pista, suspeito);
    contabilizarSuspeito(suspeito);
    mostrarPistas(raiz->dir);
}

void explorarSalas(Sala *atual, NodoBST **pistas) {
    char esc;

    while (atual) {
        printf("\n📍 Você está em: %s\n", atual->nome);

        if (strcmp(atual->pista, "") != 0) {
            printf("🕵️ Pista encontrada: %s\n", atual->pista);
            *pistas = inserirBST(*pistas, atual->pista);

            if (strcmp(atual->pista, "Página rasgada do diário") == 0)
                inserirHash(atual->pista, "Sr. Black");
            else if (strcmp(atual->pista, "Chave dourada") == 0)
                inserirHash(atual->pista, "Governanta Elsa");
            else if (strcmp(atual->pista, "Luvas sujas de sangue") == 0)
                inserirHash(atual->pista, "Chef Louis");
        }

        if (!atual->esquerda && !atual->direita) {
            printf("🚧 Sem saídas. Fim do caminho.\n");
            break;
        }

        printf("\nEscolha:\n");
        if (atual->esquerda) printf("(e) Esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita) printf("(d) Direita → %s\n", atual->direita->nome);
        printf("(p) Ver pistas até agora\n");
        printf("(s) Sair\n → ");

        scanf(" %c", &esc);

        if (esc == 'e' && atual->esquerda) atual = atual->esquerda;
        else if (esc == 'd' && atual->direita) atual = atual->direita;
        else if (esc == 'p') {
            printf("\n📜 Pistas encontradas:\n");
            for (int i = 0; i < TAM_HASH; i++)
                listaSuspeitos[i].qtd = 0;
            totalSuspeitos = 0;
            mostrarPistas(*pistas);
        }
        else if (esc == 's') break;
        else printf("❌ Opção inválida!\n");
    }
}

// ------------------- Final do Jogo -------------------
void revelarCulpado() {
    int maior = 0;
    char culpado[50] = "Desconhecido";

    for (int i = 0; i < totalSuspeitos; i++) {
        if (listaSuspeitos[i].qtd > maior) {
            maior = listaSuspeitos[i].qtd;
            strcpy(culpado, listaSuspeitos[i].nome);
        }
    }

    printf("\n🎯 Suspeito mais associado às pistas: **%s** 🩸\n", culpado);
}

int main() {
    Sala *mansao = montarMansao();
    NodoBST *pistas = NULL;

    printf("\n🏆 Detective Quest - NÍVEL MESTRE 🕵️‍♂️🔐\n");

    explorarSalas(mansao, &pistas);

    printf("\n📦 Relatório Final:\n");
    totalSuspeitos = 0;
    mostrarPistas(pistas);
    revelarCulpado();

    printf("\n🧩 Caso encerrado!\n");
    return 0;
}
